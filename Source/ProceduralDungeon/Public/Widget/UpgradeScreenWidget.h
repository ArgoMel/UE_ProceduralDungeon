// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DungeonStruct.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeScreenWidget.generated.h"

class UScrollBox;

UCLASS(Abstract)
class PROCEDURALDUNGEON_API UUpgradeScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UUpgradeScreenWidget(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> AbilityToUpgradeSB;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> mAbilityClass;

public:
	void Clear();
	void AddChild(int32 Index, FAbilityUpgrade* AbilityUpgrade,int32 PlayerGold);
};
