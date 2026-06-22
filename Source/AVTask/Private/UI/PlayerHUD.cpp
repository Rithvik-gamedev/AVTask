// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "UI/PlayerStatsOverlay.h"
#include "UI/WidgetController.h"

UWidgetController* UPlayerHUD::GetWidgetController(const FWidgetControllerParams& Params)
{
	checkf(WidgetControllerClass, TEXT("WidgetControllerClass is not set in PlayerHUD"));
	
	if (WidgetController == nullptr)
	{
		WidgetController = NewObject<UWidgetController>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(Params);
		WidgetController->BindCallbacks();
	}
	return WidgetController;
}

void UPlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(PlayerOverlayClass, TEXT("PlayerOverlayClass is not set in PlayerHUD"));
	checkf(WidgetControllerClass, TEXT("WidgetControllerClass is not set in PlayerHUD"));
	
	PlayerOverlay = CreateWidget<UPlayerStatsOverlay>(GetWorld(), PlayerOverlayClass);
	
	UWidgetController* PlayerWidgetController = GetWidgetController(FWidgetControllerParams(PC, PS, ASC, AS));
	
	PlayerOverlay->SetWidgetController(PlayerWidgetController);
	PlayerWidgetController->BroadcastInitialValues();
	PlayerOverlay->AddToViewport();
}
