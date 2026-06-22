// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CustomPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Core/CustomPlayerState.h"
#include "UI/PlayerHUD.h"

void ACustomPlayerController::InitializeHUD(UAbilitySystemComponent* ASC, UAttributeSet* AS, ACustomPlayerState* CPS)
{
	if (!IsLocalController()) return;
	
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(this, PlayerHUDClass);
		if (PlayerHUD)
		{
			check(ASC);
			check(AS);
			check(CPS);
			PlayerHUD->InitOverlay(this, CPS, ASC, AS);
		}
	}
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}
