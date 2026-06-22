// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Core/CustomGameMode.h"

#include "Enemy/EnemyCharacter.h"
#include "GameFramework/Character.h"

void ACustomGameMode::PlayerDead(AController* Controller)
{
	if (Controller)
	{
		FTimerHandle RespawnHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ACustomGameMode::RespawnPlayer, Controller);
		GetWorldTimerManager().SetTimer(RespawnHandle, TimerDelegate, 6.f, false);
	}
}

void ACustomGameMode::RequestEnemyRespawn(TSubclassOf<AEnemyCharacter> EnemyClass, FTransform SpawnTransform, float Delay, AEnemyPatrolRoute* PatrolRoute)
{
	if (!EnemyClass) return;
	
	FTimerDelegate RespawnDelegate;
	RespawnDelegate.BindUObject(this, &ACustomGameMode::RespawnEnemy, EnemyClass, SpawnTransform, PatrolRoute);

	FTimerHandle RespawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, Delay, false);
}

void ACustomGameMode::RespawnPlayer(AController* Controller)
{
	if (Controller)
	{
		RestartPlayer(Controller);
	}
}

void ACustomGameMode::RespawnEnemy(TSubclassOf<AEnemyCharacter> EnemyClass, FTransform SpawnTransform, AEnemyPatrolRoute* PatrolRoute)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnTransform, SpawnParams);
	if (Enemy)
	{
		Enemy->SetPatrolRoute(PatrolRoute);
		Enemy->SpawnDefaultController();
	}
}
