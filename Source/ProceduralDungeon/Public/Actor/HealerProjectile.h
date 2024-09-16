// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Actor/PlayerProjectile.h"
#include "HealerProjectile.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API AHealerProjectile : public APlayerProjectile
{
	GENERATED_BODY()
public:
	AHealerProjectile();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> mHitParticle;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Variable", meta = (ExposeOnSpawn = true))
	TArray<AActor*> mHitEnemies;
	/* ¼³Á¤ÇÑ ¼ö +1¸¸Å­ Æ¨±è*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Variable", meta = (ExposeOnSpawn = true))
	int32 mEnemiesToHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Variable", meta = (ExposeOnSpawn = true))
	float mHealPerHit;
};
