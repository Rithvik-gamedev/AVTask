// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/EnemyAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Enemy/EnemyCharacter.h"

UEnemyAttackAbility::UEnemyAttackAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEnemyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	if (!EnemyAttackMontage) return;
	
	int32 NumSections = EnemyAttackMontage->GetNumSections();
	int32 RandomIndex = FMath::RandRange(0, NumSections - 1);
	FName SectionToPlay = EnemyAttackMontage->GetSectionName(RandomIndex);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		EnemyAttackMontage,
		1.f,
		SectionToPlay
		);
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UEnemyAttackAbility::OnMontageFinished);
		MontageTask->OnBlendOut.AddDynamic(this, &UEnemyAttackAbility::OnMontageFinished);
		MontageTask->OnCancelled.AddDynamic(this, &UEnemyAttackAbility::OnMontageFinished);
		MontageTask->OnInterrupted.AddDynamic(this, &UEnemyAttackAbility::OnMontageInterrupted);
		MontageTask->ReadyForActivation();
	}
	
}

void UEnemyAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	
	if (AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(ActorInfo->AvatarActor.Get()))
	{
		EnemyChar->OnAttackFinishedDelegate.Broadcast();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEnemyAttackAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyAttackAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
