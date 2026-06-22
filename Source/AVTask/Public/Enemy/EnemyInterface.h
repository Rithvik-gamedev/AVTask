// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

class AEnemyPatrolRoute;
// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AVTASK_API IEnemyInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AEnemyPatrolRoute* GetPatrolRoute();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetCharacterMovementSpeed();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetEnemyMoveSpeed(float Value);
};
