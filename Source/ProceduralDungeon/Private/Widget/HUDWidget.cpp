// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/HUDWidget.h"
#include "Components/ProgressBar.h"

void UHUDWidget::UpdateHUD(float HP, float MP)
{
	HealthBar->SetPercent(HP);
	ManaBar->SetPercent(MP);
}
