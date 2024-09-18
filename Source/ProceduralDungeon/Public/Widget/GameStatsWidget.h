// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <DungeonStruct.h>
#include "Blueprint/UserWidget.h"
#include "GameStatsWidget.generated.h"

class UScrollBox;

UCLASS(Abstract)
class PROCEDURALDUNGEON_API UGameStatsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UGameStatsWidget(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> PlayerStatSB;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> mPlayerStatsClass;

public:
	void Clear();
	void AddChild(int32 Index,FPlayerStats* PlayerStat);
};
