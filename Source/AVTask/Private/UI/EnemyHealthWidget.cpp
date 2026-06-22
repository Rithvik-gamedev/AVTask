// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHealthWidget.h"

#include "Character/CustomAttributeSet.h"
#include "Components/ProgressBar.h"
#include "UI/WidgetController.h"

void UEnemyHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UEnemyHealthWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEnemyHealthWidget::InitializeWithASC(UAbilitySystemComponent* ASC, const UCustomAttributeSet* AS)
{
	bool bFoundHealth = false;
	CurrentHealth = ASC->GetGameplayAttributeValue(AS->GetHealthAttribute(), bFoundHealth);
    
	bool bFoundMaxHealth = false;
	MaxHealth = ASC->GetGameplayAttributeValue(AS->GetMaxHealthAttribute(), bFoundMaxHealth);

	UpdateHealthBar();
	
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UEnemyHealthWidget::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &UEnemyHealthWidget::OnMaxHealthChanged);
}

void UEnemyHealthWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealth = Data.NewValue;
	UE_LOG(LogTemp, Warning, TEXT("Health Changed : %f"), CurrentHealth);
	UpdateHealthBar();
}

void UEnemyHealthWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	MaxHealth = Data.NewValue;
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed : %f"), MaxHealth);
	UpdateHealthBar();
}

void UEnemyHealthWidget::UpdateHealthBar()
{
	if (HealthProgressBar && MaxHealth > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Bar Updated: %f"), HealthProgressBar->GetPercent());
		HealthProgressBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
