
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PixelCamera.generated.h"

class UCameraComponent;

UCLASS()
class PROCEDURALDUNGEON_API APixelCamera : public AActor
{
	GENERATED_BODY()	
public:	
	APixelCamera();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
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

};
