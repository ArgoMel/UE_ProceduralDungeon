// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator/DungeonGenerator.h"
#include "MapGenerator/StartingRoom.h"

ADungeonGenerator::ADungeonGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	mSeed = 0;
	mRoomAmount = 20;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);
}

void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	//mSeed = FMath::RandRange(0, 1000);
	mRandomStream = FRandomStream(mSeed);
	SpawnStartRoom();
}

void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADungeonGenerator::SpawnStartRoom_Implementation()
{
	mAllRooms.Empty();
	mExitPointList.Empty();

	int32 randIndex = mRandomStream.RandRange(0, mStartRoomClasses.Num() - 1);
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStartingRoom* room=GetWorld()->SpawnActor<AStartingRoom>(mStartRoomClasses[randIndex], RootComponent->GetComponentTransform(), spawnParam);
	TArray<USceneComponent*> exits;
	room->GetExitArrows(exits);
	mExitPointList.Append(exits);
	mAllRooms.Add(room);
	SpawnNextRoom();
}

void ADungeonGenerator::SpawnNextRoom_Implementation()
{
	if(mExitPointList.IsEmpty())
	{
		return;
	}
	int32 randIndex=mRandomStream.RandRange(0, mExitPointList.Num()-1);
	USceneComponent* selectedExitPoint= mExitPointList[randIndex];
	mExitPointList.Remove(selectedExitPoint);

	randIndex = mRandomStream.RandRange(0, mRoomList.Num() - 1);
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStartingRoom* latestRoom = GetWorld()->SpawnActor<AStartingRoom>(mRoomList[randIndex], selectedExitPoint->GetComponentTransform(), spawnParam);
	TArray<USceneComponent*> exits;
	latestRoom->GetExitArrows(exits);
	mExitPointList.Append(exits);

	--mRoomAmount;
	if(mRoomAmount<0)
	{
		return;
	}
	FTimerHandle tempTimer;
	GetWorld()->GetTimerManager().SetTimer(tempTimer,this,&ThisClass::SpawnNextRoom,0.2f);
}

