// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/AbilityWidget.h"
#include "Interface/INT_PlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UAbilityWidget::UAbilityWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mAction = 0;
	mActionSub = 0;
}

void UAbilityWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AbilityBtn->OnClicked.AddDynamic(this,&ThisClass::OnAbilityBtnClicked);
}

void UAbilityWidget::OnAbilityBtnClicked()
{
	if (GetOwningPlayer()->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_UpdateAbility(GetOwningPlayer(), mAction,mActionSub, FCString::Atoi(*GoldCostText->GetText().ToString()) );
	}
}

void UAbilityWidget::SetAbility(FAbilityUpgrade* AbilityUpgrade)
{
	mAction=AbilityUpgrade->Action;
	mActionSub=AbilityUpgrade->ActionSub;
	AbilityNameText->SetText(AbilityUpgrade->ActionName);
	GoldCostText->SetText(FText::AsNumber(AbilityUpgrade->GoldCost));
	AbilityUpgradeDescText->SetText(AbilityUpgrade->ActionDesc);
}
