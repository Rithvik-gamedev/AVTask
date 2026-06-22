// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CustomAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AController* KilledBy);

UCLASS()
class AVTASK_API UCustomAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UCustomAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	FOnDeathSignature OnDeath;

	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Stamina);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_MaxStamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxStamina);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_StaminaRegenRate")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, StaminaRegenRate);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_StaminaRegenDelay")
	FGameplayAttributeData StaminaRegenDelay;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, StaminaRegenDelay);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_AttackPower")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, AttackPower);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_Defence")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Defence);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = "OnRep_MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MoveSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, IncomingDamage);
	
protected:
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UFUNCTION()
	virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);

	UFUNCTION()
	virtual void OnRep_StaminaRegenDelay(const FGameplayAttributeData& OldStaminaRegenDelay);
	
	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);
	
	UFUNCTION()
	virtual void OnRep_Defence(const FGameplayAttributeData& OldDefence);
	
	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
};
