// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/PlayerController.h"
#include "MainMenu/INT_MainMenu.h"
#include "PC_MainMenu.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API APC_MainMenu : public APlayerController
	,public IINT_MainMenu
{
	GENERATED_BODY()
public:
	APC_MainMenu();
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> mMainMenuClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UUserWidget> mMainMenu;
};
