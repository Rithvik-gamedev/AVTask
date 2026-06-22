#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Potion.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UGameplayEffect;

UCLASS()
class AVTASK_API APotion : public AActor
{
	GENERATED_BODY()
	
public:	
	APotion();
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdateHover();

	UFUNCTION()
	void OnWidgetZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWidgetZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PotionMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* PotionLabelWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* WidgetZone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<UGameplayEffect> PotionEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hover")
	float HoverAmplitude = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hover")
	float HoverSpeed = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	FText PotionText;

private:
	FTimerHandle HoverTimerHandle;
	float CurrentHoverTime = 0.f;
	FVector StartMeshLocation;
};
