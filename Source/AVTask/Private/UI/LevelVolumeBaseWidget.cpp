// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LevelVolumeBaseWidget.h"

#include "Components/TextBlock.h"

void ULevelVolumeBaseWidget::UpdateText(const FText& Text)
{
	if (VolumeTextBlock)
	{
		VolumeTextBlock->SetText(Text);
	}
}
