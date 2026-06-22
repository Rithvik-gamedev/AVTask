// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "LevelVolumeBase.generated.h"

class UWeaponDataAsset;
class UGameplayEffect;
class UGameplayAbility;
class UBoxComponent;
class UWidgetComponent;

UCLASS()
class AVTASK_API ALevelVolumeBase : public AActor
{
	GENERATED_BODY()

public:

	ALevelVolumeBase();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelVolume")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelVolume")
	TObjectPtr<UWidgetComponent> InfoWidget;
	
	// Text To set in LevelVolumeBaseWidget. Use this to set the widget's Text in BP  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelVolume", meta = (MultiLine="true"))
	FText InfoWidgetText = FText::FromString(TEXT("Put Text Here"));
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelVolume")
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelVolume")
	TSubclassOf<UGameplayAbility> AbilityToApply;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelVolume")
	TSubclassOf<UGameplayEffect> EffectToApply;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelVolume")
	TObjectPtr<UWeaponDataAsset> WeaponToEquip;
	
	// Effects to keep track when characters enter the volume,to remove when exit
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelVolume")
	TMap<AActor*, FActiveGameplayEffectHandle> ActiveVolumeEffects;
};
