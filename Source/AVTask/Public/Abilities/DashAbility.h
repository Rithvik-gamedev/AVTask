// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DashAbility.generated.h"

/**
 *
 */
UCLASS()
class AVTASK_API UDashAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UDashAbility();
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnDashDelayComplete();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float DashStrength = 2500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float DashDuration = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float DashDirectionZ = 5.f;

private:

	FTimerHandle DashTimerHandle;
};
