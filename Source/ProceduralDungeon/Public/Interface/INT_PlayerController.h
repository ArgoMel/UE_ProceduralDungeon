// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "UObject/Interface.h"
#include "INT_PlayerController.generated.h"

class APC_DungeonGame;
class UDungeonGameIAs;

UINTERFACE(MinimalAPI)
class UINT_PlayerController : public UInterface
{
	GENERATED_BODY()
};

class PROCEDURALDUNGEON_API IINT_PlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPlayerCanMove(bool CanMove);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePlayerHUD(float HP,float MP);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayerRespawn(FVector PlayerSpawnLoc,TSubclassOf<ABasePlayer> PlayerClass);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayerSelectScreenChoice(TSubclassOf<ABasePlayer> SelectedClass);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayerFirstSpawn(TSubclassOf<ABasePlayer> PlayerClass,FVector SpawnLoc);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddGold(int32 Amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddKill();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	APC_DungeonGame* GetDungeonPCRef();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FPlayerStats GetPlayerStats();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UDungeonGameIAs* GetDungeonGameIAs() const;
};
