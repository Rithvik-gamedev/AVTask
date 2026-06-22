// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelVolume/LevelVolumeBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Character/CustomCharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/LevelVolumeBaseWidget.h"
#include "Weapon/WeaponDataAsset.h"


ALevelVolumeBase::ALevelVolumeBase()
{
	bReplicates = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetHiddenInGame(false);
	
	InfoWidget = CreateDefaultSubobject<UWidgetComponent>("InfoWidget");
	InfoWidget->SetupAttachment(RootComponent);
	InfoWidget->SetDrawAtDesiredSize(true);
}

void ALevelVolumeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (InfoWidget)
	{
#if WITH_EDITOR
		
		if (!InfoWidget->GetUserWidgetObject())
		{
			InfoWidget->InitWidget();
		}
#endif
		
		ULevelVolumeBaseWidget* LevelVolumeWidget = Cast<ULevelVolumeBaseWidget>(InfoWidget->GetUserWidgetObject());
		if (LevelVolumeWidget)
		{
			LevelVolumeWidget->UpdateText(InfoWidgetText);
		}
	}
}

void ALevelVolumeBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALevelVolumeBase::OnBoxOverlap);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALevelVolumeBase::OnBoxEndOverlap);
	}
	
	if (InfoWidget)
	{
		ULevelVolumeBaseWidget* LevelVolumeWidget = Cast<ULevelVolumeBaseWidget>(InfoWidget->GetUserWidgetObject());
		if (LevelVolumeWidget)
		{
			LevelVolumeWidget->UpdateText(InfoWidgetText);
		}
	}
}

void ALevelVolumeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ALevelVolumeBase::OnBoxOverlap);
		BoxCollision->OnComponentEndOverlap.RemoveDynamic(this, &ALevelVolumeBase::OnBoxEndOverlap);
	}
}

void ALevelVolumeBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!OtherActor) return;
	
	if (ACustomCharacterBase* CharacterBase = Cast<ACustomCharacterBase>(OtherActor))
	{
		UAbilitySystemComponent* ASC = CharacterBase->GetAbilitySystemComponent();
		if (!ASC) return;
		
		// Only Grant Abilities to Player Character
		FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("Player.Character");
		if (AbilityToApply && !CharacterBase->HasAbilityClass(AbilityToApply) && ASC->HasMatchingGameplayTag(PlayerTag))
		{
			FGameplayAbilitySpec NewSpec(AbilityToApply, 1, INDEX_NONE, CharacterBase);
			FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(NewSpec);
			CharacterBase->AddToGrantedAbilities(AbilitySpecHandle);
		}
		
		// Effects Can be applied to Characters and Enemies such as healing, damage etc
		if (EffectToApply && !ActiveVolumeEffects.Contains(OtherActor))
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectToApply, 1, EffectContext);
			if (EffectSpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveEffectSpecHandle = ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), ASC);
				ActiveVolumeEffects.Add(OtherActor, ActiveEffectSpecHandle);
			}
		}
		
		// Equip Weapon to Characters only
		if (WeaponToEquip && ASC->HasMatchingGameplayTag(PlayerTag))
		{
			CharacterBase->EquipWeapon(WeaponToEquip);
		}
		
	}
}

void ALevelVolumeBase::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;
	if (!OtherActor) return;
	
	// Remove Effects on End Overlap
	if (ACustomCharacterBase* CharacterBase = Cast<ACustomCharacterBase>(OtherActor))
	{
		UAbilitySystemComponent* ASC = CharacterBase->GetAbilitySystemComponent();
		if (!ASC) return;
		
		if (FActiveGameplayEffectHandle* EffectHandlePtr = ActiveVolumeEffects.Find(OtherActor))
		{
			ASC->RemoveActiveGameplayEffect(*EffectHandlePtr);
			ActiveVolumeEffects.Remove(OtherActor);
		}
	}
}
