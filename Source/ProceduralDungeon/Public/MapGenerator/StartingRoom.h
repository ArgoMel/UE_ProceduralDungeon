// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartingRoom.generated.h"

class UArrowComponent;

UCLASS()
class PROCEDURALDUNGEON_API AStartingRoom : public AActor
{
	GENERATED_BODY()	
public:	
	AStartingRoom();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USceneComponent> mRoomExitList;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USceneComponent> mOverlapBoxList;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USceneComponent> mGeometry;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UArrowComponent> mArrow;

public:
	void GetExitArrows(TArray<USceneComponent*>& ExitList);
	void GetColliders(TArray<UShapeComponent*>& ColliderList);
};
