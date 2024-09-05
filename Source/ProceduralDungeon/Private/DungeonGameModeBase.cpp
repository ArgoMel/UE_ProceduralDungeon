// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGameModeBase.h"
#include "PC_DungeonGame.h"
#include "Character/BaseCharacter.h"

ADungeonGameModeBase::ADungeonGameModeBase()
{
	PlayerControllerClass = APC_DungeonGame::StaticClass();
	DefaultPawnClass = ABaseCharacter::StaticClass();
}
