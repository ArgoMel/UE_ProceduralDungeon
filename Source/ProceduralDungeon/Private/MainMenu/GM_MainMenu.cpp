// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/GM_MainMenu.h"
#include "MainMenu/MainMenuCharacter.h"
#include "MainMenu/PC_MainMenu.h"

AGM_MainMenu::AGM_MainMenu()
{
	PlayerControllerClass = APC_MainMenu::StaticClass();
	DefaultPawnClass = AMainMenuCharacter::StaticClass();
}
