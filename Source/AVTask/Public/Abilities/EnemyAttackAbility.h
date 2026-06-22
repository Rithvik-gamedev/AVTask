// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnemyAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class AVTASK_API UEnemyAttackAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEnemyAttackAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnMontageFinished();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Attack")
	TObjectPtr<UAnimMontage> EnemyAttackMontage;
	
};
