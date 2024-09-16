// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"

class USphereComponent;
class ABaseEnemy;

UCLASS()
class PROCEDURALDUNGEON_API AAISpawner : public AActor
{
	GENERATED_BODY()
public:	
	AAISpawner();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> mStaticMesh;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USphereComponent> mStartSpawnSphere;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FTimerHandle mSpawnEnemyTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	int32 mSpawnedEnemies;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	int32 mCurrentSpawned;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bIsSpawning;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MustSet")
	TArray<TSubclassOf<ABaseEnemy>> mSpawnableEnemies;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MustSet")
	int32 mTotalEnemiesToSpawn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MustSet")
	int32 mSpawnAtTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MustSet")
	float mSpawnTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MustSet")
	float mSpawnRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MustSet")
	bool bDestroyAfterFinalSpawn;

protected:
	UFUNCTION()
	void OnStartSpawnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Timer")
	void SpawnEnemyTimer();
	virtual void SpawnEnemyTimer_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Timer")
	void StopSpawning();
	virtual void StopSpawning_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Delegate")
	void HandleSpawnedDeath(ABaseEnemy* DeadEnemy);
	virtual void HandleSpawnedDeath_Implementation(ABaseEnemy* DeadEnemy);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void SpawnEnemies();
	virtual void SpawnEnemies_Implementation();
};
