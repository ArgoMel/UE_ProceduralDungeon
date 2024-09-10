// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BaseCharacter.h"
#include "Interface/INT_EnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseEnemy.generated.h"

class UBoxComponent;
class ABasePlayer;

UCLASS()
class PROCEDURALDUNGEON_API ABaseEnemy : public ABaseCharacter
	, public IINT_EnemyCharacter
{
	GENERATED_BODY()
public:
	ABaseEnemy();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	void AddPlayerTarget_Implementation(AActor* PlayerTarget, bool Add);
	ABaseEnemy* GetEnemyRef_Implementation();
	void EnemyMeleeAttack_Implementation();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = "Component")
	TObjectPtr<UBoxComponent> mMeleeBoxCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> mMeleeAttackMontage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> mRangedAttackMontage;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Variable")
	TArray<ABasePlayer*> mPlayerTargets;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	float mAttackDistance;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	float mTimeBetweenAttacks;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	float mMeleeDamage;
	UPROPERTY(BlueprintReadWrite, Replicated,Category = "Variable")
	bool bIsChasing;

protected:
	UFUNCTION()
	void OnMeleeBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	UFUNCTION()
	void CheckChasing();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Chase();
	virtual void Chase_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GoHome();
	virtual void GoHome_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void Server_Death();
	virtual void Server_Death_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Client")
	void Client_Death();
	virtual void Client_Death_Implementation();

	UFUNCTION(BlueprintCallable)
	void Death();
};
