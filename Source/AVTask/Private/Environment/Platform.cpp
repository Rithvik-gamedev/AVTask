#include "Environment/Platform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Potion.h"
#include "Engine/World.h"
#include "TimerManager.h"

APlatform::APlatform()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	PlatformMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlatformMesh->SetCollisionResponseToAllChannels(ECR_Block);

	PlatformPath = CreateDefaultSubobject<USplineComponent>(TEXT("PlatformPath"));
	PlatformPath->SetupAttachment(RootComponent);

	AttachmentZone = CreateDefaultSubobject<UBoxComponent>(TEXT("AttachmentZone"));
	AttachmentZone->SetupAttachment(PlatformMesh);
	AttachmentZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttachmentZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttachmentZone->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void APlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AttachmentZone->OnComponentBeginOverlap.AddDynamic(this, &APlatform::OnAttachmentZoneOverlap);

		PlatformPath->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		SplineLength = PlatformPath->GetSplineLength();

		GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &APlatform::UpdateMovement, MovementUpdateRate, true);
        
		// Delay initial overlap check to ensure all actors have finished their BeginPlay
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlatform::CheckInitialOverlaps);
	}
}

void APlatform::UpdateMovement()
{
	if (SplineLength <= 0.f)
	{
		return;
	}
	
	float DistanceToMove = MovementSpeed * MovementUpdateRate;
	CurrentDistanceAlongSpline += (bMovingForwardOnSpline ? DistanceToMove : -DistanceToMove);

	if (CurrentDistanceAlongSpline >= SplineLength)
	{
	
		bMovingForwardOnSpline = false;
		CurrentDistanceAlongSpline = SplineLength;
		
	}
	else if (CurrentDistanceAlongSpline <= 0.f)
	{
		bMovingForwardOnSpline = true;
		CurrentDistanceAlongSpline = 0.f;
	}

	FTransform TransformAtDistance = PlatformPath->GetTransformAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
	SetActorLocation(TransformAtDistance.GetLocation());
}

void APlatform::OnAttachmentZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (Cast<APotion>(OtherActor))
		{
			OtherActor->AttachToComponent(PlatformMesh, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void APlatform::CheckInitialOverlaps()
{
	TArray<AActor*> OverlappingActors;
	AttachmentZone->GetOverlappingActors(OverlappingActors, APotion::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		Actor->AttachToComponent(PlatformMesh, FAttachmentTransformRules::KeepWorldTransform);
	}
}
