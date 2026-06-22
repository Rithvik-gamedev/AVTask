// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

class AEnemyCharacter;
class AEnemyPatrolRoute;
/**
 * 
 */
UCLASS()
class AVTASK_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	void PlayerDead(AController* Controller);
	
	void RequestEnemyRespawn(TSubclassOf<AEnemyCharacter> EnemyClass, FTransform SpawnTransform, float Delay, AEnemyPatrolRoute* PatrolRoute);
	
protected:
	
	void RespawnPlayer(AController* Controller);
	
	void RespawnEnemy(TSubclassOf<AEnemyCharacter> EnemyClass, FTransform SpawnTransform, AEnemyPatrolRoute* PatrolRoute);
	
	
};
