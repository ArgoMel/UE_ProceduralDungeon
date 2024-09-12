// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_MainMenu.generated.h"

class UMainMenuWidget;

UCLASS()
class PROCEDURALDUNGEON_API APC_MainMenu : public APlayerController
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
	TObjectPtr<UMainMenuWidget> mMainMenu;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HostGame(const FString& GameName,int32 Players);
	void HostGame_Implementation(const FString& GameName, int32 Players);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JoinGame();
	void JoinGame_Implementation();
};
