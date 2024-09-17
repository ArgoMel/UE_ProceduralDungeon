// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROCEDURALDUNGEON_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ABaseCharacter();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AutoCollision")
	bool bUseAutoCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mCurHealth;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category = "MustSet")
	float mMaxHealth;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	FVector mInitialSpawnLoc;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = "OnRep_IsDead", Category = "Variable")
	bool bIsDead;

private:
	void UpdateCollisionCapsuleToFitMesh();
	void UpdateRelativeLocationToFitCapsule() const;

protected:
	UFUNCTION()
	virtual void OnRep_IsDead();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleDeath();
	virtual void HandleDeath_Implementation() {};

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable,Category = "Multicast")
	void Multi_PlayMontage(UAnimMontage* MontageToPlay);
	virtual void Multi_PlayMontage_Implementation(UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Multicast")
	void Multi_PlaySFX(USoundBase* Sound,FVector Loc);
	virtual void Multi_PlaySFX_Implementation(USoundBase* Sound, FVector Loc);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Multicast")
	void Multi_SpawnParticle(UParticleSystem* EmitterTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f));
	virtual void Multi_SpawnParticle_Implementation(UParticleSystem* EmitterTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f));

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Death(AActor* Player=nullptr);
	virtual void Server_Death_Implementation(AActor* Player = nullptr);
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Client")
	void Client_Death();
	virtual void Client_Death_Implementation();
	UFUNCTION(BlueprintCallable)
	virtual void Death();

	float GetCurHP() const
	{
		return mCurHealth;
	}
	float GetMaxHP() const
	{
		return mMaxHealth;
	}
	bool GetIsDead() const
	{
		return bIsDead;
	}
};
