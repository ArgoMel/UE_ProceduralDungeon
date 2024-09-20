// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator/StartingRoom.h"
#include "Components/ArrowComponent.h"
#include "Components/ShapeComponent.h"

AStartingRoom::AStartingRoom()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	mRoomExitList = CreateDefaultSubobject<USceneComponent>(TEXT("RoomExitList"));
	mRoomExitList->SetupAttachment(RootComponent);

	mOverlapBoxList = CreateDefaultSubobject<USceneComponent>(TEXT("OverlapBoxList"));
	mOverlapBoxList->SetupAttachment(RootComponent);

	mGeometry = CreateDefaultSubobject<USceneComponent>(TEXT("Geometry"));
	mGeometry->SetupAttachment(RootComponent);

	mArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	mArrow->SetupAttachment(RootComponent);
	mArrow->SetArrowSize(5.f);
}

void AStartingRoom::BeginPlay()
{
	Super::BeginPlay();
}

void AStartingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStartingRoom::GetExitArrows(TArray<USceneComponent*>& ExitList)
{
	mRoomExitList->GetChildrenComponents(false, ExitList);
}

void AStartingRoom::GetColliders(TArray<UShapeComponent*>& ColliderList)
{
	TArray<USceneComponent*> components;
	mOverlapBoxList->GetChildrenComponents(false, components);
	for(auto& component: components)
	{
		UShapeComponent* shapeComp = Cast<UShapeComponent>(component);
		if(shapeComp)
		{
			ColliderList.Add(shapeComp);
		}
	}
}

