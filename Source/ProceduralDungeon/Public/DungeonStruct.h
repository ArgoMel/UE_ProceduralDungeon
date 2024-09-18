
#pragma once
#include "CoreMinimal.h"
#include "DungeonStruct.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 KillCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Gold;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HealthPCT;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ManaPCT;

	FPlayerStats()
	{
		KillCount = 0;
		Gold = 0;
		HealthPCT = 0.f;
		ManaPCT = 0.f;
	}
};

USTRUCT(BlueprintType)
struct FAbilityUpgrade
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ActionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ActionDesc;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Action;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ActionSub;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 GoldCost;

	FAbilityUpgrade()
	{
		Action = 0;
		ActionSub = 0;
		GoldCost = 0.f;
	}
};

class PROCEDURALDUNGEON_API DungeonStruct
{
public:
	DungeonStruct();
	~DungeonStruct();
};
