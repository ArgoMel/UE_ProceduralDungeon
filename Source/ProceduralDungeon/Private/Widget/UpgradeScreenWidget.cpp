// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/UpgradeScreenWidget.h"
#include "Widget/AbilityWidget.h"
#include "ProceduralDungeon.h"
#include "Components/ScrollBox.h"

UUpgradeScreenWidget::UUpgradeScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mAbilityClass, UUserWidget, "/Game/_Main/Widget/WBP_Ability.WBP_Ability_C");
}

void UUpgradeScreenWidget::Clear()
{
	AbilityToUpgradeSB->ClearChildren();
}

void UUpgradeScreenWidget::AddChild(int32 Index, FAbilityUpgrade* AbilityUpgrade, int32 PlayerGold)
{
	int32 childCount = AbilityToUpgradeSB->GetChildrenCount();
	UAbilityWidget* abilityWidget = nullptr;
	if (Index >= childCount)
	{
		abilityWidget = CreateWidget<UAbilityWidget>(GetOwningPlayer(), mAbilityClass);
		abilityWidget->SetAbility(AbilityUpgrade);
		AbilityToUpgradeSB->AddChild(abilityWidget);
	}
	else
	{
		abilityWidget = Cast<UAbilityWidget>(AbilityToUpgradeSB->GetChildAt(Index));
		abilityWidget->SetAbility(AbilityUpgrade);
	}
	abilityWidget->SetIsEnabled(AbilityUpgrade->GoldCost<=PlayerGold);
}
