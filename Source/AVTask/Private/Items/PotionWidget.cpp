// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PotionWidget.h"

#include "Components/TextBlock.h"

void UPotionWidget::UpdateText(const FText& Text)
{
	if (PotionTextBlock)
	{
		PotionTextBlock->SetText(Text);
	}
}
