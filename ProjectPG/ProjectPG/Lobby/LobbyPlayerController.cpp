// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "LobbyWidgetBase.h"
void ALobbyPlayerController::BeginPlay()
{

	Super::BeginPlay();

	//Off When It is Not Local Player Controller.
	if (IsLocalPlayerController())
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());

		if (LobbyWidgetClass)
		{
			LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
			if (LobbyWidgetObject)
			{
				LobbyWidgetObject->AddToViewport();

				if (!HasAuthority())
				{
					LobbyWidgetObject->HideStartGameButton();
				}
			}
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("You Must Set LobbyWidgetClass in the Blueprint"));
		}

		/*

		//블루프린트 경로 _C 필요함.
		FSoftClassPath widgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/Lobby/LobbyWidget.LobbyWidget'_C"));

		UClass* temp =widgetClass.TryLoadClass<ULobbyWidgetBase>();

		if (temp )
		{
			LobbyWidgetObject = CreateWidget<ULobbyWidgetBase>(this, temp);
			if (LobbyWidgetObject)
			{
				LobbyWidgetObject->AddToViewport();
			}
		}

		*/


	}
}

//서버에서 실행됨.
void ALobbyPlayerController::C2S_SendMessage_Implementation(const FText& Message)
{
	auto iter = GetWorld()->GetPlayerControllerIterator();

	for (; iter; ++iter)
	{
		ALobbyPlayerController* pc = Cast<ALobbyPlayerController>(*iter);
		if (pc)
		{
			//server to client
			pc->S2C_SendMessage(Message);
		}
	}

}

//서버에서 실행됨.
bool ALobbyPlayerController::C2S_SendMessage_Validate(const FText & Message)
{
	return true;
}

//call server , execute in client
void ALobbyPlayerController::S2C_SendMessage_Implementation(const FText & Message)
{
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->AddMessage(Message);
	}
}