// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/GameStatsWidget.h"
#include "Widget/PlayerStatsWidget.h"
#include "Components/ScrollBox.h"

UGameStatsWidget::UGameStatsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mPlayerStatsClass, UUserWidget, "/Game/_Main/Widget/WBP_PlayerStats.WBP_PlayerStats_C");
}

void UGameStatsWidget::Clear()
{
	PlayerStatSB->ClearChildren();
}

void UGameStatsWidget::AddChild(int32 Index, FPlayerStats* PlayerStat)
{
	int32 childCount = PlayerStatSB->GetChildrenCount();
	UPlayerStatsWidget* playerStatWidget = nullptr;
	if (Index >= childCount)
	{
		playerStatWidget = CreateWidget<UPlayerStatsWidget>(this, mPlayerStatsClass);
		playerStatWidget->SetPlayerStat(PlayerStat);
		PlayerStatSB->AddChild(playerStatWidget);
	}
	else
	{
		playerStatWidget = Cast<UPlayerStatsWidget>(PlayerStatSB->GetChildAt(Index));
		playerStatWidget->SetPlayerStat(PlayerStat);
	}
}
