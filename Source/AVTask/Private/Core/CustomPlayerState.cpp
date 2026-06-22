// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CustomPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Character/CustomAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACustomPlayerState::ACustomPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>("AttributeSet");
}

void ACustomPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACustomPlayerState, Level);
}

UAbilitySystemComponent* ACustomPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACustomPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
