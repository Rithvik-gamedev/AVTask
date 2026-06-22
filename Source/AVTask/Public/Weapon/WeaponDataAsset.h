// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class AWeaponBase;
/**
 * 
 */
UCLASS(BlueprintType)
class AVTASK_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeaponBase> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EquipEffect;
	
	// Add Attack Combo Ability here in array to grant player Attack Combo Ability when equipping weapon. Each Weapon can have its own Attack Combo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
