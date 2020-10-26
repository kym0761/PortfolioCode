// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "TitlePlayerController.h"
#include "../PGGameInstance.h"
#include "kismet/GameplayStatics.h"

void UTitleWidgetBase::NativeConstruct()
{
	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("TextBox_ID")));
	UserPassword = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("TextBox_Password")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("TextBox_ServerIP")));
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));

	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnConnectionButton);
	}
	else 
	{
		UE_LOG(LogClass, Warning, TEXT("No Connection"));
	}
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnStartButton);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("No Start"));
	}

}

void UTitleWidgetBase::OnConnectionButton()
{
	ATitlePlayerController* playerController = GetOwningPlayer<ATitlePlayerController>();
	if (playerController)
	{
		if (ServerIP)
		{
			SaveUserID();

			FString serverIPAddress = ServerIP->GetText().ToString();

			playerController->ConnectServer(serverIPAddress);
		}


	}

}

void UTitleWidgetBase::OnStartButton()
{
	ATitlePlayerController* playerController = GetOwningPlayer<ATitlePlayerController>();
	if (playerController)
	{
		SaveUserID();
	
		playerController->StartServer();
	}

}

void UTitleWidgetBase::SaveUserID()
{
	UPGGameInstance* gameInstance = Cast<UPGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (gameInstance)
	{
		if (UserID)
		{
			gameInstance->SetUserID(UserID->GetText().ToString());
		}
	}
}
