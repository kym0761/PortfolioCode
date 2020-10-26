// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "LobbyPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "../PGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGameStateBase.h"
#include "Components/Button.h"
#include "LobbyGameModeBase.h"

void ULobbyWidgetBase::NativeConstruct()
{

	Super::NativeConstruct();

	ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("MessageInputBox")));

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::ProcessTextCommit);
	}

	ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatBox")));

	ConnectCount= Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_ConnectCount")));
	PublicMessage = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_PublicMessage")));
	StartGameButton= Cast<UButton>(GetWidgetFromName(TEXT("GameStartButton")));

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::PressStartGameButton);
	}
	
}

void ULobbyWidgetBase::PressStartGameButton()
{
	ALobbyGameModeBase* gm = Cast<ALobbyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gm) // server only
	{
		gm->StartGame();
	}

}

void ULobbyWidgetBase::ProcessTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	switch(CommitMethod)
	{
	case ETextCommit::OnEnter:
	{//local -> host
		ALobbyPlayerController* pc = GetOwningPlayer<ALobbyPlayerController>();
		if (pc)
		{

			UPGGameInstance* gameInst = Cast
				<UPGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (gameInst)
			{
				FString temp = FString::Printf(TEXT("%s : %s"), *gameInst->GetUserID(), *Text.ToString());
				pc->C2S_SendMessage(FText::FromString(temp));
				ChatInput->SetText(FText::FromString(TEXT("")));
			}


		}
	}
		break;
	case ETextCommit::OnCleared:
	{
		ChatInput->SetUserFocus(GetOwningPlayer());
	}
		break;
	default:

		break;
	}
}

void ULobbyWidgetBase::AddMessage(FText Message)
{
	if (ChatBox)
	{
		UTextBlock* newTextBlock = NewObject<UTextBlock>(ChatBox);
		if (newTextBlock)
		{
			newTextBlock->SetText(Message);
			newTextBlock->Font.Size = 18;
			ChatBox->AddChild(newTextBlock);
			ChatBox->ScrollToEnd();
		}
	}
}

//void ULobbyWidgetBase::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//	ALobbyGameStateBase* gameState = Cast<ALobbyGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
//	
//	if (gameState)
//	{
//		FString temp = FString::Printf(TEXT("% Connected"), gameState->ConnectCount);
//		if (ConnectCount)
//		{
//			ConnectCount->SetText(FText::FromString(temp));
//		}
//	}
//}

void ULobbyWidgetBase::SetConnectCount(int32 Count)
{

		FString temp = FString::Printf(TEXT("%d Connected"), Count);
		if (ConnectCount)
		{
			ConnectCount->SetText(FText::FromString(temp));
		}
}

void ULobbyWidgetBase::SetPublicMessage(int32 LeftTime)
{
	FString temp = FString::Printf(TEXT("%d seconds Left"), LeftTime);
	if (ConnectCount)
	{
		PublicMessage->SetText(FText::FromString(temp));
	}

}

void ULobbyWidgetBase::HideStartGameButton()
{
	if (StartGameButton)
	{
		StartGameButton->SetVisibility(ESlateVisibility::Hidden);
	}
}
