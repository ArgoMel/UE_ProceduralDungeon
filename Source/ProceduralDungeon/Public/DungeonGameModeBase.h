// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "Interface/INT_GameMode.h"
#include "DungeonGameModeBase.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API ADungeonGameModeBase : public AGameModeBase
	, public IINT_GameMode
{
	GENERATED_BODY()
public:
	ADungeonGameModeBase();
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
public:
	void GetSelectedClass_Implementation(TSubclassOf<ABasePlayer> SelectedClass, APlayerController* PlayerController);
	void GetPlayerStats_Implementation(TArray<FPlayerStats>& PlayerStats);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<AActor*> mPlayerStartLocs;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<APlayerController*> mConnectedPlayers;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FPlayerStats> mPlayerStats;
};
