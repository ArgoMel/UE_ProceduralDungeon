// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

class AStartingRoom;

UCLASS()
class PROCEDURALDUNGEON_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
public:	
	ADungeonGenerator();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Variable")
	TArray<TSubclassOf<AStartingRoom>> mStartRoomClasses;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Variable")
	TArray<TSubclassOf<AStartingRoom>> mRoomList;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<AStartingRoom*> mAllRooms;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<USceneComponent*> mExitPointList;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FRandomStream mRandomStream;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Variable")
	int32 mSeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
	int32 mRoomAmount;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,CallInEditor)
	void SpawnStartRoom();
	void SpawnStartRoom_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnNextRoom();
	void SpawnNextRoom_Implementation();
};
