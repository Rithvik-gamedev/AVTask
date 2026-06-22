// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Character/CustomAttributeSet.h"
#include "UObject/Object.h"
#include "WeaponDamageCalc.generated.h"

struct FWeaponDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage);
	
	FWeaponDamageStatics()
	{
		// Capture the Source's Attack
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustomAttributeSet, AttackPower, Source, false);
		// Capture the Target's Defence
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustomAttributeSet, Defence, Target, false);
		// Capture the Target's IncomingDamage
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCustomAttributeSet, IncomingDamage, Target, false);
	}
};

static const FWeaponDamageStatics& DamageStatics()
{
	static FWeaponDamageStatics Statics;
	return Statics;
}


UCLASS()
class AVTASK_API UWeaponDamageCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UWeaponDamageCalc();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
