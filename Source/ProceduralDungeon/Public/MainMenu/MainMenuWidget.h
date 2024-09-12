// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralDungeon.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UEditableTextBox;
class UTextBlock;

UCLASS(Abstract)
class PROCEDURALDUNGEON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MainMenuWS;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NewGameBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> MultiplayerBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> QuitGameBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CreateGameBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FindGameBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> MPBackBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> GameNameETB;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DecreasePlayerBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNumText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> IncreasePlayerBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> StartGameBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HGBackBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> RefreshBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BGBackBtn;

protected:
	UFUNCTION()
	void OnNewGameBtnClicked();
	UFUNCTION()
	void OnMultiplayerBtnClicked();
	UFUNCTION()
	void OnQuitGameBtnClicked();

	UFUNCTION()
	void OnCreateGameBtnClicked();
	UFUNCTION()
	void OnFindGameBtnClicked();
	UFUNCTION()
	void OnBackBtnClicked();

	UFUNCTION()
	void OnGameNameETBTextChanged(const FText& Text);
	UFUNCTION()
	void OnDecreasePlayerBtnClicked();
	UFUNCTION()
	void OnIncreasePlayerBtnClicked();
	UFUNCTION()
	void OnStartGameBtnClicked();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshGame();
	void RefreshGame_Implementation();
};
