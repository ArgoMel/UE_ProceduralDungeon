// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INT_MainMenu.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UINT_MainMenu : public UInterface
{
	GENERATED_BODY()
};

class PROCEDURALDUNGEON_API IINT_MainMenu
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HostGame(const FString& GameName, int32 Players);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JoinGame();
};
