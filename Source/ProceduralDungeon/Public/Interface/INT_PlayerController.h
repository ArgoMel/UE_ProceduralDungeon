// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INT_PlayerController.generated.h"

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
};
