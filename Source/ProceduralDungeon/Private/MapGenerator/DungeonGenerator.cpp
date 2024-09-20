// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator/DungeonGenerator.h"
#include "MapGenerator/StartingRoom.h"
#include "ProceduralDungeon.h"
#include "Components/ShapeComponent.h"

ADungeonGenerator::ADungeonGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	mSeed = 0;
	mRoomAmount = 20;
	mMaxRooms = 0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);
}

void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	if(mRoomAmount<4)
	{
		mRoomAmount = 4;
	}
	mMaxRooms = mRoomAmount;
	//mSeed = FMath::RandRange(0, 1000);
	mRandomStream = FRandomStream(mSeed);
	SpawnStartRoom();
}

void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADungeonGenerator::Overlaps()
{
	mOverlappingRoomList.Empty();
	VALIDCHECK_RETURNVALUE(mLatestRoom,true);
	TArray<UShapeComponent*> colliders;
	mLatestRoom->GetColliders(colliders);
	for(auto& collider: colliders)
	{
		TArray<UPrimitiveComponent*> overlappingRoomList;
		collider->GetOverlappingComponents(overlappingRoomList);
		mOverlappingRoomList.Append(overlappingRoomList);
	}
	return !mOverlappingRoomList.IsEmpty();
}

void ADungeonGenerator::SpawnStartRoom_Implementation()
{
	--mRoomAmount;
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
	TSubclassOf<AStartingRoom> roomToSpawn = mRoomList[randIndex];
	if(mRoomAmount<mMaxRooms/2)
	{
		for(auto& requiredRoom:mRequiredRooms)
		{
			if(requiredRoom.Value>0)
			{
				roomToSpawn = requiredRoom.Key;
				--requiredRoom.Value;
				break;
			}
		}
	}

	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	mLatestRoom = GetWorld()->SpawnActor<AStartingRoom>(roomToSpawn, selectedExitPoint->GetComponentTransform(), spawnParam);
	CheckForOverlappingRoom();
}

void ADungeonGenerator::CheckForOverlappingRoom_Implementation()
{
	if(Overlaps())
	{
		mLatestRoom->Destroy();
	}
	else
	{
		--mRoomAmount;
	}
	TArray<USceneComponent*> exits;
	mLatestRoom->GetExitArrows(exits);
	mExitPointList.Append(exits);
	mAllRooms.Add(mLatestRoom);

	if (mRoomAmount < 0)
	{
		return;
	}
	FTimerHandle tempTimer;
	GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::SpawnNextRoom, 0.2f);
}

