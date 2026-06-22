// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsOverlay.generated.h"

class UProgressBar;
class UWidgetController;
/**
 * 
 */
UCLASS()
class AVTASK_API UPlayerStatsOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void SetWidgetController(UWidgetController* WC);
	
protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaProgressBar;
	
	void InitializeWidget();
	
	UFUNCTION()
	void UpdateHealth(float NewValue);
    
	UFUNCTION()
	void UpdateMaxHealth(float NewValue);

	UFUNCTION()
	void UpdateStamina(float NewValue);
    
	UFUNCTION()
	void UpdateMaxStamina(float NewValue);
	
private:
	
	UPROPERTY()
	TObjectPtr<UWidgetController> WidgetController;
	
	float CurrentHealth = 0.f;
	float MaxHealth = 0.f;
    
	float CurrentStamina = 0.f;
	float MaxStamina = 0.f;
};
