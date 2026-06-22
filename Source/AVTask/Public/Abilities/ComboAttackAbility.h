// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ComboAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class AVTASK_API UComboAttackAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UComboAttackAbility();
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Setup")
	TArray<UAnimMontage*> AttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Setup")
	TSubclassOf<UGameplayEffect> CostGameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Setup")
	TSubclassOf<UGameplayEffect> MoveSpeedSlowdownEffect;
	
	FActiveGameplayEffectHandle ActiveMoveSpeedEffectHandle;
	
private:
	
	int32 CurrentComboIndex = 0;
	bool bTransitioning = false;
	bool bInputBuffer = false;
	
	void PlayAttackCombo();
	void DropTransitioning();
	
	FTimerHandle DropTransitionTimer;
	
	UFUNCTION()
	void OnInputReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnComboWindowReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageFinished();

	UFUNCTION()
	void OnMontageInterrupted();
	
};
