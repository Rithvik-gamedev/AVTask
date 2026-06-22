// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

UDashAbility::UDashAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character && Character->GetCharacterMovement())
	{
		// dash at last input vector direction
		FVector DashDirection = Character->GetCharacterMovement()->GetLastInputVector();
		
		if (DashDirection.IsNearlyZero())
		{
			DashDirection = Character->GetActorForwardVector();
		}
		else
		{
			DashDirection.Z = DashDirectionZ; 
			DashDirection.Normalize();
		}

		Character->LaunchCharacter(DashDirection * DashStrength, true, true);
		
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(DashTimerHandle, this, &UDashAbility::OnDashDelayComplete, DashDuration, false);
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashTimerHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDashAbility::OnDashDelayComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
