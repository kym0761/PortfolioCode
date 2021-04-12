// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby_ReadyWidget.h"
#include "Components/Button.h"
#include "../MainUI/UI_PC.h"

void ULobby_ReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UnReadyButton = Cast<UButton>(GetWidgetFromName(TEXT("UnReady")));
	ReadyButton = Cast<UButton>(GetWidgetFromName(TEXT("Ready")));

	if(UnReadyButton)
		UnReadyButton->OnClicked.AddDynamic(this, &ULobby_ReadyWidget::Ready);

	if(ReadyButton)
		ReadyButton->OnClicked.AddDynamic(this, &ULobby_ReadyWidget::UnReady);
}


void ULobby_ReadyWidget::Ready()
{
	UnReadyButton->SetVisibility(ESlateVisibility::Collapsed);
	ReadyButton->SetVisibility(ESlateVisibility::Visible);

	AUI_PC* PC = Cast<AUI_PC>(GetOwningPlayer());
	PC->C2S_SetIsReady(true);
}

void ULobby_ReadyWidget::UnReady()
{
	UnReadyButton->SetVisibility(ESlateVisibility::Visible);
	ReadyButton->SetVisibility(ESlateVisibility::Collapsed);

	AUI_PC* PC = Cast<AUI_PC>(GetOwningPlayer());
	PC->C2S_SetIsReady(false);
}