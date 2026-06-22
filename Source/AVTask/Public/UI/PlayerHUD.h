// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UPlayerStatsOverlay;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UWidgetController;
/**
 * 
 */
UCLASS()
class AVTASK_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UWidgetController* GetWidgetController(const FWidgetControllerParams& Params);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UPlayerStatsOverlay> PlayerOverlay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPlayerStatsOverlay> PlayerOverlayClass;
	
	UPROPERTY()
	TObjectPtr<UWidgetController> WidgetController;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UWidgetController> WidgetControllerClass;
};
