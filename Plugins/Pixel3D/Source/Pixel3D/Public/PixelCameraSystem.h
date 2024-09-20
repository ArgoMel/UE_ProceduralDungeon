// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PixelCameraSystem.generated.h"

class UCameraComponent;

UCLASS()
class PIXEL3D_API APixelCameraSystem : public AActor
{
	GENERATED_BODY()	
public:	
	APixelCameraSystem();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	TArray<UMaterialInstanceDynamic*> mMaterialArray;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USceneComponent> mPixelCameraTransform;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCameraComponent> mPixelCamera;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<USceneComponent> mUpscaledDisplay;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UCameraComponent> mViewCamera;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> mQuadCanvas;

public:
	/// <summary>
	/// Resize the canvas to the provided aspect and height.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "UpscaledCanvas")
	void ResizeCanvas(float Aspect, float CanvasHeight);

	/// <summary>
	/// Sets the canvas render texture.
	/// </summary>
	void SetCanvasRenderTexture(UTexture* RenderTexture);
};
