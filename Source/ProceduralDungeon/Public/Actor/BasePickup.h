// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;

UCLASS()
class PROCEDURALDUNGEON_API ABasePickup : public AActor
{
	GENERATED_BODY()
public:	
	ABasePickup();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> mStaticMesh;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USphereComponent> mPickupSphere;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "MustSet")
	EPickupType mPickupType;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "MustSet")
	int32 mPickupAmount;

protected:
	UFUNCTION()
	virtual void OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void AdjustLoc();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	void GoldPickup(AActor* Player);
	virtual void GoldPickup_Implementation(AActor* Player);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	void HealthPickup(AActor* Player);
	virtual void HealthPickup_Implementation(AActor* Player);
};
