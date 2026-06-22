// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "CustomCharacterBase.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class AWeaponBase;
class UWeaponDataAsset;
class UGameplayAbility;
class UCustomAttributeSet;
class UGameplayEffect;

UCLASS()
class AVTASK_API ACustomCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCustomAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	void EquipWeapon(UWeaponDataAsset* WeaponDataAsset);
	void UnequipWeapon();
	
	bool HasAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	void AddToGrantedAbilities(const FGameplayAbilitySpecHandle& AbilitySpecHandle);
	void RemoveFromGrantedAbilities(const FGameplayAbilitySpecHandle& AbilitySpecHandle);

protected:
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;
	
	UFUNCTION()
	void OnRep_CurrentWeapon();
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CurrentWeapon, Category = "Combat")
	TObjectPtr<AWeaponBase> CurrentWeapon;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UWeaponDataAsset> CurrentWeaponData;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> WeaponGrantedAbilities;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;
	
	UPROPERTY()
	FActiveGameplayEffectHandle EquipEffectHandle;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCustomAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Abilities")
	TSubclassOf<UGameplayAbility> JumpAbility;
	
	public:
	
	FORCEINLINE AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon;}
};
