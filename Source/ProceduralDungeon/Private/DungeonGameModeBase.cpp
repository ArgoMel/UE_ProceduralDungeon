// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGameModeBase.h"
#include "PC_DungeonGame.h"
#include "Interface/INT_PlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include <Kismet/GameplayStatics.h>

ADungeonGameModeBase::ADungeonGameModeBase()
{
	PlayerControllerClass = APC_DungeonGame::StaticClass();
	DefaultPawnClass = nullptr;
}

void ADungeonGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	APlayerController* pc = Cast<APlayerController>(NewPlayer);
	if(pc)
	{
		mConnectedPlayers.AddUnique(pc);
	}
}

void ADungeonGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	APlayerController* pc = Cast<APlayerController>(Exiting);
	if (pc)
	{
		mConnectedPlayers.Remove(pc);
	}
}

void ADungeonGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(), mPlayerStartLocs);
}

void ADungeonGameModeBase::GetSelectedClass_Implementation(TSubclassOf<ABasePlayer> SelectedClass, APlayerController* PlayerController)
{
	FVector randLoc = mPlayerStartLocs[FMath::RandRange(0, mPlayerStartLocs.Num()-1)]->GetActorLocation();
	if (PlayerController->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
	{
		IINT_PlayerController::Execute_PlayerFirstSpawn(PlayerController, SelectedClass, randLoc);
	}
}

void ADungeonGameModeBase::GetPlayerStats_Implementation(TArray<FPlayerStats>& PlayerStats)
{
	mPlayerStats.Empty();
	for (auto& player : mConnectedPlayers)
	{
		if (!player->GetClass()->ImplementsInterface(UINT_PlayerController::StaticClass()))
		{
			continue;
		}
		FPlayerStats playerStats = IINT_PlayerController::Execute_GetPlayerStats(player);
		mPlayerStats.Add(playerStats);
	}
	PlayerStats= mPlayerStats;
}
