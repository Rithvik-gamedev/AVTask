// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

struct FOnAttributeChangeData;
class UCustomAttributeSet;
class UAbilitySystemComponent;
class UProgressBar;
/**
 * 
 */
UCLASS()
class AVTASK_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable)
	void InitializeWithASC(UAbilitySystemComponent* ASC, const UCustomAttributeSet* AS);
	
protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;
	
	float CurrentHealth = 0.f;
	float MaxHealth = 0.f;
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	
	void UpdateHealthBar();

};
