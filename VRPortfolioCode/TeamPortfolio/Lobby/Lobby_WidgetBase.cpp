// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby_WidgetBase.h"
#include "Components/TextBlock.h"

void ULobby_WidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	TimeLeftMessage = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimeLeftMessage")));
	TimeLeftMessage->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobby_WidgetBase::SetMessage(int LeftTime, bool isLobby)
{
	FString Message;

	if (LeftTime <= 0)
	{
		TimeLeftMessage->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (isLobby == true)
	{
		Message = FString::Printf(TEXT("%d초 후에 게임이 시작됩니다."), LeftTime);
		if (TimeLeftMessage)
		{
			TimeLeftMessage->SetText(FText::FromString(Message));
		}
	}
	else
	{
		int minute = LeftTime / 60;
		int second = LeftTime % 60;

		if (minute == 0)
		{
			Message = FString::Printf(TEXT("%d"), LeftTime);
		}
		else
		{
			Message = FString::Printf(TEXT("%d : %d"), minute, second);
		}		
	}

	if (TimeLeftMessage)
	{
		TimeLeftMessage->SetText(FText::FromString(Message));
		TimeLeftMessage->SetVisibility(ESlateVisibility::Visible);
	}	
}
