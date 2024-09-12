// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/MainMenuWidget.h"
#include "MainMenu/PC_MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

constexpr int32 MinPlayerNum = 2;
constexpr int32 MaxPlayerNum = 4;

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	NewGameBtn->OnClicked.AddDynamic(this, &ThisClass::OnNewGameBtnClicked);
	MultiplayerBtn->OnClicked.AddDynamic(this, &ThisClass::OnMultiplayerBtnClicked);
	QuitGameBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitGameBtnClicked);

	CreateGameBtn->OnClicked.AddDynamic(this, &ThisClass::OnCreateGameBtnClicked);
	FindGameBtn->OnClicked.AddDynamic(this, &ThisClass::OnFindGameBtnClicked);
	MPBackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);

	GameNameETB->OnTextChanged.AddDynamic(this, &ThisClass::OnGameNameETBTextChanged);
	DecreasePlayerBtn->OnClicked.AddDynamic(this, &ThisClass::OnDecreasePlayerBtnClicked);
	IncreasePlayerBtn->OnClicked.AddDynamic(this, &ThisClass::OnIncreasePlayerBtnClicked);
	StartGameBtn->OnClicked.AddDynamic(this, &ThisClass::OnStartGameBtnClicked);
	HGBackBtn->OnClicked.AddDynamic(this, &ThisClass::OnMultiplayerBtnClicked);

	RefreshBtn->OnClicked.AddDynamic(this, &ThisClass::RefreshGame);
	BGBackBtn->OnClicked.AddDynamic(this, &ThisClass::OnMultiplayerBtnClicked);
}

void UMainMenuWidget::OnNewGameBtnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MAP_FIRSTLEVEL);
}

void UMainMenuWidget::OnMultiplayerBtnClicked()
{
	MainMenuWS->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::OnQuitGameBtnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),GetOwningPlayer(),EQuitPreference::Quit,false);
}

void UMainMenuWidget::OnCreateGameBtnClicked()
{
	MainMenuWS->SetActiveWidgetIndex(2);
}

void UMainMenuWidget::OnFindGameBtnClicked()
{
	MainMenuWS->SetActiveWidgetIndex(3);
	RefreshGame();
}

void UMainMenuWidget::OnBackBtnClicked()
{
	MainMenuWS->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnGameNameETBTextChanged(const FText& Text)
{
	StartGameBtn->SetIsEnabled(!Text.IsEmpty());
}

void UMainMenuWidget::OnDecreasePlayerBtnClicked()
{
	int curPlayerNum = FCString::Atof(*PlayerNumText->GetText().ToString());
	curPlayerNum = FMath::Clamp(curPlayerNum-1, MinPlayerNum,MaxPlayerNum);
	PlayerNumText->SetText(FText::AsNumber(curPlayerNum));
}

void UMainMenuWidget::OnIncreasePlayerBtnClicked()
{
	int curPlayerNum = FCString::Atof(*PlayerNumText->GetText().ToString());
	curPlayerNum = FMath::Clamp(curPlayerNum + 1, MinPlayerNum, MaxPlayerNum);
	PlayerNumText->SetText(FText::AsNumber(curPlayerNum));
}

void UMainMenuWidget::OnStartGameBtnClicked()
{
	APC_MainMenu* pc=Cast<APC_MainMenu>(GetOwningPlayer());
	VALIDCHECK(pc);
	int curPlayerNum = FCString::Atof(*PlayerNumText->GetText().ToString());
	pc->HostGame(GameNameETB->GetText().ToString(), curPlayerNum);
}

void UMainMenuWidget::RefreshGame_Implementation()
{
}
