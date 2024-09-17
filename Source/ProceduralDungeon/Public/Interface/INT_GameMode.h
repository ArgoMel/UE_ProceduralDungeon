// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "UObject/Interface.h"
#include "INT_GameMode.generated.h"

class ABasePlayer;

UINTERFACE(MinimalAPI)
class UINT_GameMode : public UInterface
{
	GENERATED_BODY()
};

class PROCEDURALDUNGEON_API IINT_GameMode
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetSelectedClass(TSubclassOf<ABasePlayer> SelectedClass,APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetPlayerStats(TArray<FPlayerStats>& PlayerStats);
};
