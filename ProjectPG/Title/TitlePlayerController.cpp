// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "Blueprint/UserWidget.h" //CreateWidget
#include "TitleWidgetBase.h"
#include "kismet/GameplayStatics.h"

void ATitlePlayerController::BeginPlay()
{
	if (TitleWidgetClass)
	{
		TitleWidgetObject = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidgetObject->AddToViewport();
	
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("You Must Add TitleWidgetClass"));
	}
}

void ATitlePlayerController::StartServer()
{
	//SetInputMode(FInputModeGameOnly());
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void ATitlePlayerController::ConnectServer(FString ServerIPAddress)
{
	//SetInputMode(FInputModeGameOnly());
	UGameplayStatics::OpenLevel(GetWorld(),FName(*ServerIPAddress));
}
