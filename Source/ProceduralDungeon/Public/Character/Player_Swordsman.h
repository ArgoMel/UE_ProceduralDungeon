// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BasePlayer.h"
#include <Components/TimelineComponent.h>
#include "Player_Swordsman.generated.h"

class UBoxComponent;

UCLASS()
class PROCEDURALDUNGEON_API APlayer_Swordsman : public ABasePlayer
{
	GENERATED_BODY()
public:
	APlayer_Swordsman();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void GenerateAbilityUpgradeList() override;
public:
	void UseAction1_Implementation();
	void UseAction4_Implementation();
	APlayer_Swordsman* GetPlayerSwordsmanRef_Implementation();
	void UpgradeAbility_Implementation(int32 Action, int32 ActionSub);

private:
	FTimeline mBlinkTL;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UArrowComponent> mAction4ParticleLoc;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UBoxComponent> mBlinkBox;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TArray<UAnimMontage*> mAction1Montages;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> mAction4Montage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> mAction4Particle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Timeline")
	TObjectPtr<UCurveFloat> mLerpCurve;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	FVector mStartBlinkLoc;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	FVector mEndBlinkLoc;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bIsBlinking;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	bool bIsBlocking;

protected:
	UFUNCTION()
	void OnBlinkBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "Timeline")
	void UpdateBlinkLerpCurve(float Value);
	UFUNCTION(Category = "Timeline")
	void CompleteBlink();

	UFUNCTION(BlueprintPure)
	bool PlayerBlockCheck(AActor* DamageCauser) const;

public:
	virtual void Server_Action1_Implementation() override;
	virtual void Server_Action2_Implementation() override;
	virtual void Server_Action3_Implementation() override;
	virtual void Server_Action3End_Implementation() override;
	virtual void Server_Action4_Implementation() override;

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Client")
	void Client_Blink();
	virtual void Client_Blink_Implementation();

	UFUNCTION(BlueprintCallable)
	void Blink();
};
