// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDWidget::UpdateHUD(float HP, float MP, int32 Gold)
{
	HealthBar->SetPercent(HP);
	ManaBar->SetPercent(MP);
	GoldAmountText->SetText(FText::AsNumber(Gold));
}
