// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Core/CustomPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponDataAsset.h"


ACustomCharacterBase::ACustomCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ACustomCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACustomCharacterBase::InitAbilityActorInfo()
{
	// Use Derived Class virtual function to set ASC and AS. Leaving this empty  
}

void ACustomCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(GetAbilitySystemComponent())
	check(GameplayEffectClass)
	
	if (GameplayEffectClass == nullptr) return;
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACustomCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultAttributes, 1.f);
}

void ACustomCharacterBase::OnRep_CurrentWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandGrip_R"));
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

void ACustomCharacterBase::EquipWeapon(UWeaponDataAsset* WeaponDataAsset)
{
	if (!WeaponDataAsset || !HasAuthority()) return;
	if (!AbilitySystemComponent) return;
	
	UnequipWeapon();
	
	CurrentWeaponData = WeaponDataAsset;
	
	if (WeaponDataAsset->WeaponClass)
	{
		FActorSpawnParameters SpawnInfo;
		
		if (ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>())
		{
			SpawnInfo.Owner = PS;
		}
		
		SpawnInfo.Instigator = this;
		
		// Attach to mesh
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponDataAsset->WeaponClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandGrip_R"));
			
			// Grant Abilities
			for (auto AbilityClass : WeaponDataAsset->GrantedAbilities)
			{
				if (AbilityClass)
				{
					FGameplayAbilitySpec NewSpec(AbilityClass, 1, INDEX_NONE, CurrentWeapon);
					
					FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(NewSpec);
					WeaponGrantedAbilities.Add(SpecHandle);
				}
			}
			
			// Apply Weapon Gameplay Effect 
			if (WeaponDataAsset->EquipEffect)
			{
				FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
				ContextHandle.AddSourceObject(this);
				const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(WeaponDataAsset->EquipEffect, 1.f, ContextHandle);
				EquipEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
			}
		}
	}
}

void ACustomCharacterBase::UnequipWeapon()
{
	if (!AbilitySystemComponent || !HasAuthority()) return;

	if (EquipEffectHandle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(EquipEffectHandle);
		EquipEffectHandle.Invalidate();
	}
	
	if (WeaponGrantedAbilities.Num() > 0)
	{
		for (const FGameplayAbilitySpecHandle& Handle : WeaponGrantedAbilities)
		{
			AbilitySystemComponent->ClearAbility(Handle);
		}
		
		WeaponGrantedAbilities.Empty();
	}
	
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = nullptr;
	}
    
	CurrentWeaponData = nullptr;
}

bool ACustomCharacterBase::HasAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!AbilitySystemComponent || !AbilityClass) return false;
	
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass);
	return Spec != nullptr;
}

void ACustomCharacterBase::AddToGrantedAbilities(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	if (!AbilitySpecHandle.IsValid()) return;
	
	if (!GrantedAbilities.Contains(AbilitySpecHandle))
	{
		GrantedAbilities.AddUnique(AbilitySpecHandle);
	}
}

void ACustomCharacterBase::RemoveFromGrantedAbilities(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	if (!AbilitySpecHandle.IsValid()) return;
	
	if (GrantedAbilities.Contains(AbilitySpecHandle))
	{
		GrantedAbilities.Remove(AbilitySpecHandle);
	}
}

void ACustomCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomCharacterBase, CurrentWeapon);
}
