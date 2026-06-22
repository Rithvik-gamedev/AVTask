// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyPatrolRoute.h"

#include "Components/SplineComponent.h"


// Sets default values
AEnemyPatrolRoute::AEnemyPatrolRoute()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyPatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyPatrolRoute::IncreasePatrolPoint()
{

	PatrolIndex = PatrolIndex + Direction;
	
	if (PatrolIndex == Spline->GetNumberOfSplinePoints())
	{
		Direction = -1;
	}
	else
	{
		if (PatrolIndex == 0)
		{
			Direction = 1;
		}
	}
}

void AEnemyPatrolRoute::GetSplinePointsInWorld(FVector& OutVector)
{
	if (Spline)
	{
		OutVector = Spline->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
	}
}

