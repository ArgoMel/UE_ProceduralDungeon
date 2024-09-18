// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DungeonStruct.h"
#include "Blueprint/UserWidget.h"
#include "AbilityWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS(Abstract)
class PROCEDURALDUNGEON_API UAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UAbilityWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AbilityBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityNameText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldCostText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityUpgradeDescText;

	UPROPERTY(BlueprintReadWrite, Category = "Variable", meta = (ExposeOnSpawn = true))
	int32 mAction;
	UPROPERTY(BlueprintReadWrite, Category = "Variable", meta = (ExposeOnSpawn = true))
	int32 mActionSub;

protected:
	UFUNCTION()
	void OnAbilityBtnClicked();

public:
	void SetAbility(FAbilityUpgrade* AbilityUpgrade);
};
