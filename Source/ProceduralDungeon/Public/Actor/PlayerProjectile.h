// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/Actor.h"
#include "PlayerProjectile.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class PROCEDURALDUNGEON_API APlayerProjectile : public AActor
{
	GENERATED_BODY()
public:	
	APlayerProjectile();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UBoxComponent> mBox;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UParticleSystemComponent> mParticleSystem;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UProjectileMovementComponent> mProjectileMovement;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Replicated, Category = "Variable", meta = (ExposeOnSpawn = true))
	float mSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Variable", meta = (ExposeOnSpawn = true))
	float mDamage;

protected:
	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Multi")
	void Multi_SpawnParticle(UParticleSystem* EmitterTemplate, FVector Location);
	virtual void Multi_SpawnParticle_Implementation(UParticleSystem* EmitterTemplate, FVector Location);
};
