// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ComboAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UComboAttackAbility::UComboAttackAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UComboAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (MoveSpeedSlowdownEffect)
	{
		ActiveMoveSpeedEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, MoveSpeedSlowdownEffect.GetDefaultObject(), GetAbilityLevel());
	}
	
	CurrentComboIndex = 0;
	bTransitioning = false;
	bInputBuffer = false;
	
	UAbilityTask_WaitGameplayEvent* InputTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.Input.Attack"),
		nullptr,
		false,
		true
		);
	InputTask->EventReceived.AddDynamic(this, &UComboAttackAbility::OnInputReceived);
	InputTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ComboWindowTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.ComboWindow"),
		nullptr,
		false,
		true
		);
	ComboWindowTask->EventReceived.AddDynamic(this, &UComboAttackAbility::OnComboWindowReceived);
	ComboWindowTask->ReadyForActivation();
	
	PlayAttackCombo();
}

void UComboAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActiveMoveSpeedEffectHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveMoveSpeedEffectHandle);
		ActiveMoveSpeedEffectHandle.Invalidate();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UComboAttackAbility::PlayAttackCombo()
{
	if (AttackMontages.Num() == 0 || CurrentComboIndex >= AttackMontages.Num())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC && CostGameplayEffect)
	{
		// check for stamina
		if (!ASC->CanApplyAttributeModifiers(CostGameplayEffect.GetDefaultObject(), GetAbilityLevel(), MakeEffectContext(CurrentSpecHandle, CurrentActorInfo)))
		{
			bInputBuffer = false;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			return;
		}
	}
	
	(void)ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, CostGameplayEffect.GetDefaultObject(), GetAbilityLevel());
	
	bTransitioning = true;
	
	UAnimMontage* MontagePlay = AttackMontages[CurrentComboIndex];
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontagePlay
		);
	
	MontageTask->OnCompleted.AddDynamic(this, &UComboAttackAbility::OnMontageFinished);
	MontageTask->OnBlendOut.AddDynamic(this, &UComboAttackAbility::OnMontageFinished);
	MontageTask->OnCancelled.AddDynamic(this, &UComboAttackAbility::OnMontageFinished);
	MontageTask->OnInterrupted.AddDynamic(this, &UComboAttackAbility::OnMontageInterrupted);
	MontageTask->ReadyForActivation();
	
	GetWorld()->GetTimerManager().SetTimer(DropTransitionTimer, this, &UComboAttackAbility::DropTransitioning, 0.1f, false);
	
}

void UComboAttackAbility::DropTransitioning()
{
	bTransitioning = false;
}

void UComboAttackAbility::OnInputReceived(FGameplayEventData Payload)
{
	bInputBuffer = true;
}

void UComboAttackAbility::OnComboWindowReceived(FGameplayEventData Payload)
{
	if (bInputBuffer)
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		if (ASC && CostGameplayEffect)
		{
			// check for stamina
			if (!ASC->CanApplyAttributeModifiers(CostGameplayEffect.GetDefaultObject(), GetAbilityLevel(), MakeEffectContext(CurrentSpecHandle, CurrentActorInfo)))
			{
				bInputBuffer = false;
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
				return;
			}
		}
		
		bInputBuffer = false;
		CurrentComboIndex++;
		PlayAttackCombo();
	}
}

void UComboAttackAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UComboAttackAbility::OnMontageInterrupted()
{
	if (!bTransitioning)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
