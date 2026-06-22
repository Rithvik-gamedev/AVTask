// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

class AWeaponBase;
class UCustomAttributeSet;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class AVTASK_API ACustomPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	ACustomPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCustomAttributeSet* GetAttributeSet() const {return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	
	// Holds weapon when player is dead and waiting for respawn
	UPROPERTY()
	AWeaponBase* RetainedWeapon = nullptr;
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCustomAttributeSet> AttributeSet;

private:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
};
