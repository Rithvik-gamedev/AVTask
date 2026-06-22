// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPatrolRoute.generated.h"

class USplineComponent;

UCLASS()
class AVTASK_API AEnemyPatrolRoute : public AActor
{
	GENERATED_BODY()

public:
	AEnemyPatrolRoute();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void IncreasePatrolPoint();
	
	UFUNCTION(BlueprintCallable)
	void GetSplinePointsInWorld(FVector& OutVector);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline;

	int32 PatrolIndex;
	
	// -1 backward and 1 forward
	int32 Direction = 1;
	
	FVector SplinePointLocation;

};
