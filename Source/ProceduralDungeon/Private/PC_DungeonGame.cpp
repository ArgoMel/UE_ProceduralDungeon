// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_DungeonGame.h"
#include "Input/DungeonGameIAs.h"
#include "EnhancedInputSubsystems.h"

APC_DungeonGame::APC_DungeonGame()
{
	GetObjectAsset(mDungeonGameIAs, UDungeonGameIAs,"/Game/_Main/Inputs/DA_DungoenGameIAs.DA_DungoenGameIAs");
}

void APC_DungeonGame::BeginPlay()
{
	Super::BeginPlay();
}

void APC_DungeonGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	inputSubSystem->ClearAllMappings();
	SetInput(true);
	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(input))
	{
		return;
	}
}

void APC_DungeonGame::AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority) const
{
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	ensure(InputMapping);
	if (!inputSubSystem->HasMappingContext(InputMapping))
	{
		inputSubSystem->AddMappingContext(InputMapping, MappingPriority);
	}
}

void APC_DungeonGame::RemoveInputMapping(const UInputMappingContext* InputMapping) const
{
	UEnhancedInputLocalPlayerSubsystem* inputSubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(inputSubSystem))
	{
		return;
	}
	ensure(InputMapping);
	inputSubSystem->RemoveMappingContext(InputMapping);
}

void APC_DungeonGame::SetInput(const bool Enabled) const
{
	ensureMsgf(mDungeonGameIAs, TEXT("mDungeonGameIAs is NULL!"));
	ensure(mDungeonGameIAs->mDefaultMappingContext);
	if (Enabled)
	{
		AddInputMapping(mDungeonGameIAs->mDefaultMappingContext, mDungeonGameIAs->mDefaultMapPriority);
	}
	else
	{
		RemoveInputMapping(mDungeonGameIAs->mDefaultMappingContext);
	}
}
