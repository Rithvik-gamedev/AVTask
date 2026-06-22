// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController.h"

#include "Character/CustomAttributeSet.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UWidgetController::BindCallbacks()
{
	const UCustomAttributeSet* CustomAttributeSet = CastChecked<UCustomAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CustomAttributeSet->GetHealthAttribute()).AddUObject(this, &UWidgetController::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CustomAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UWidgetController::OnMaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CustomAttributeSet->GetStaminaAttribute()).AddUObject(this, &UWidgetController::OnStaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CustomAttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &UWidgetController::OnMaxStaminaChanged);
}

void UWidgetController::BroadcastInitialValues()
{
	const UCustomAttributeSet* CustomAttributeSet = CastChecked<UCustomAttributeSet>(AttributeSet);

	OnHealthChange.Broadcast(CustomAttributeSet->GetHealth());
	OnMaxHealthChange.Broadcast(CustomAttributeSet->GetMaxHealth());
	OnStaminaChange.Broadcast(CustomAttributeSet->GetStamina());
	OnMaxStaminaChange.Broadcast(CustomAttributeSet->GetMaxStamina());
}

void UWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChange.Broadcast(Data.NewValue);
}

void UWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChange.Broadcast(Data.NewValue);
}

void UWidgetController::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	OnStaminaChange.Broadcast(Data.NewValue);
}

void UWidgetController::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	OnMaxStaminaChange.Broadcast(Data.NewValue);
}
