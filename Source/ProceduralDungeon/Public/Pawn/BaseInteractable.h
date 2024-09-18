// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "GameFramework/Actor.h"
#include "BaseInteractable.generated.h"

class USphereComponent;
class UCapsuleComponent;

UCLASS()
class PROCEDURALDUNGEON_API ABaseInteractable : public AActor
{
	GENERATED_BODY()
public:
	ABaseInteractable();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCapsuleComponent> mCapsule;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USphereComponent> mSphere;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> mSkeletalMesh;

protected:
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
