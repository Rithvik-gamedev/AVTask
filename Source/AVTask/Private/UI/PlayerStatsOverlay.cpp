// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatsOverlay.h"

#include "Components/ProgressBar.h"
#include "UI/WidgetController.h"

void UPlayerStatsOverlay::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStatsOverlay::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (WidgetController)
	{
		WidgetController->OnHealthChange.RemoveDynamic(this, &UPlayerStatsOverlay::UpdateHealth);
		WidgetController->OnMaxHealthChange.RemoveDynamic(this, &UPlayerStatsOverlay::UpdateMaxHealth);
    
		WidgetController->OnStaminaChange.RemoveDynamic(this, &UPlayerStatsOverlay::UpdateStamina);
		WidgetController->OnMaxStaminaChange.RemoveDynamic(this, &UPlayerStatsOverlay::UpdateMaxStamina);
	}
}

void UPlayerStatsOverlay::SetWidgetController(UWidgetController* WC)
{
	WidgetController = WC;
	if (WidgetController)
	{
		InitializeWidget();
	}
}

void UPlayerStatsOverlay::InitializeWidget()
{
	WidgetController->OnHealthChange.AddDynamic(this, &UPlayerStatsOverlay::UpdateHealth);
	WidgetController->OnMaxHealthChange.AddDynamic(this, &UPlayerStatsOverlay::UpdateMaxHealth);
    
	WidgetController->OnStaminaChange.AddDynamic(this, &UPlayerStatsOverlay::UpdateStamina);
	WidgetController->OnMaxStaminaChange.AddDynamic(this, &UPlayerStatsOverlay::UpdateMaxStamina);
}

void UPlayerStatsOverlay::UpdateHealth(float NewValue)
{
	CurrentHealth = NewValue;
	
	if (HealthProgressBar && MaxHealth != 0.f)
	{
		HealthProgressBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UPlayerStatsOverlay::UpdateMaxHealth(float NewValue)
{
	MaxHealth = NewValue;
    
	if (HealthProgressBar && MaxHealth != 0.f)
	{
		HealthProgressBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UPlayerStatsOverlay::UpdateMaxStamina(float NewValue)
{
	MaxStamina = NewValue;
    
	if (StaminaProgressBar && MaxStamina != 0.f)
	{
		StaminaProgressBar->SetPercent(CurrentStamina / MaxStamina);
	}
}

void UPlayerStatsOverlay::UpdateStamina(float NewValue)
{
	CurrentStamina = NewValue;
    
	if (StaminaProgressBar && MaxStamina != 0.f)
	{
		StaminaProgressBar->SetPercent(CurrentStamina / MaxStamina);
	}
}
