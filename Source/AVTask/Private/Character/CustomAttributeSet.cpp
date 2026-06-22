// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UCustomAttributeSet::UCustomAttributeSet()
{
}

void UCustomAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCustomAttributeSet, StaminaRegenDelay, COND_None, REPNOTIFY_Always);
}

void UCustomAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UCustomAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		
		const bool bWasAlive = GetHealth() > 0.0f;
		if (bWasAlive && GetHealth() == 0.0f)
		{
			AController* InstigatorController = Data.EffectSpec.GetEffectContext().GetInstigator()->GetInstigatorController();
			OnDeath.Broadcast(InstigatorController);
		}
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalDamageDone = GetIncomingDamage();
		
		SetIncomingDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			const bool bWasAlive = GetHealth() > 0.0f;
			const float NewHealth = FMath::Clamp(GetHealth() - LocalDamageDone, 0.0f, GetMaxHealth());
			SetHealth(NewHealth);

			if (bWasAlive && NewHealth == 0.0f)
			{
				AController* InstigatorController = Data.EffectSpec.GetEffectContext().GetInstigator()->GetInstigatorController();
				OnDeath.Broadcast(InstigatorController);
			}
		}
	}
}

void UCustomAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UCustomAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, Health, OldHealth);
}

void UCustomAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, MaxHealth, OldMaxHealth);
}

void UCustomAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, Stamina, OldStamina);
}

void UCustomAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, MaxStamina, OldMaxStamina);
}

void UCustomAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, StaminaRegenRate, OldStaminaRegenRate);
}

void UCustomAttributeSet::OnRep_StaminaRegenDelay(const FGameplayAttributeData& OldStaminaRegenDelay)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, StaminaRegenDelay, OldStaminaRegenDelay);
}

void UCustomAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, AttackPower, OldAttackPower);
}

void UCustomAttributeSet::OnRep_Defence(const FGameplayAttributeData& OldDefence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, Defence, OldDefence);
}

void UCustomAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCustomAttributeSet, MoveSpeed, OldMoveSpeed);
}
