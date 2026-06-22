#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Platform.generated.h"

class UStaticMeshComponent;
class USplineComponent;
class UBoxComponent;

UCLASS()
class AVTASK_API APlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatform();

protected:
	virtual void BeginPlay() override;

	void UpdateMovement();
	
	UFUNCTION()
	void CheckInitialOverlaps();

	UFUNCTION()
	void OnAttachmentZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelVolume")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* PlatformPath;

	// The overlap zone to get items on the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* AttachmentZone;

	// platform speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 200.f;

	// How often the movement timer fires
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementUpdateRate = 0.016f;

	float CurrentDistanceAlongSpline;
	bool bMovingForwardOnSpline = true;
	float SplineLength;

	FTimerHandle MovementTimerHandle;
};
