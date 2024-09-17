// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerStatsWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UPlayerStatsWidget::SetPlayerStat(FPlayerStats* PlayerStat)
{
	PlayerNameText->SetText(FText::FromString(PlayerStat->PlayerName));
	GoldAmountText->SetText(FText::AsNumber(PlayerStat->Gold));
	KillAmountText->SetText(FText::AsNumber(PlayerStat->KillCount));
	HealthPB->SetPercent(PlayerStat->HealthPCT);
	ManaPB->SetPercent(PlayerStat->ManaPCT);
}
