// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/PC_MainMenu.h"
#include "Blueprint/UserWidget.h"
//#include "MainMenu/MainMenuWidget.h"

APC_MainMenu::APC_MainMenu()
{
	GetClassAsset(mMainMenuClass, UUserWidget, "/Game/_Main/MainMenu/WBP_MainMenu.WBP_MainMenu_C");
}

void APC_MainMenu::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		if (GetNetMode() == NM_Standalone)
		{
			mMainMenu = CreateWidget<UUserWidget>(this, mMainMenuClass);
			mMainMenu->AddToViewport();
			FInputModeGameAndUI inputmode;
			inputmode.SetWidgetToFocus(mMainMenu->GetCachedWidget());
			inputmode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
			inputmode.SetHideCursorDuringCapture(false);
			SetInputMode(inputmode);
		}
	}
	else
	{
		mMainMenu = CreateWidget<UUserWidget>(this, mMainMenuClass);
		mMainMenu->AddToViewport();
		FInputModeGameAndUI inputmode;
		inputmode.SetWidgetToFocus(mMainMenu->GetCachedWidget());
		inputmode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
		inputmode.SetHideCursorDuringCapture(false);
		SetInputMode(inputmode);

	}
	SetShowMouseCursor(true);
}
