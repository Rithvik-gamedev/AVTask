// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

class ACustomPlayerState;
class UPlayerHUD;
class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AVTASK_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	void InitializeHUD(UAbilitySystemComponent* ASC, UAttributeSet* AS, ACustomPlayerState* CPS);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UPlayerHUD> PlayerHUD;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	
};
