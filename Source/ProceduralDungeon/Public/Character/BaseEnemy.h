// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Character/BaseCharacter.h"
#include "Interface/INT_EnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseEnemy.generated.h"

class UBoxComponent;
class ABasePlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDies, ABaseEnemy*, DeadEnemy);

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
public:
	virtual void HandleDeath_Implementation();
	virtual void Server_Death_Implementation(AActor* Player = nullptr) override;
	virtual void Death() override;

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

public:
	UPROPERTY(BlueprintAssignable,BlueprintCallable, Category = "Event")
	FEnemyDies OnEnemyDies;

protected:
	UFUNCTION()
	void OnMeleeBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Chase();
	virtual void Chase_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GoHome();
	virtual void GoHome_Implementation();
};
