// Fill out your copyright notice in the Description page of Project Settings.


#include "Title_PC.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Title_UserWidgetBase.h"

#include "HeadMountedDisplayFunctionLibrary.h"

void ATitle_PC::BeginPlay()
{
	Super::BeginPlay();

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected())
	{
		//If VR is ON, Do Nothing... maybe?
		SetInputMode(FInputModeGameAndUI());
	}
	else if (TitleWidgetClass && IsLocalController())
	{
		TitleWidgetObject = CreateWidget<UTitle_UserWidgetBase>(this, TitleWidgetClass);
		TitleWidgetObject->AddToViewport();

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ATitle_PC::StartServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void ATitle_PC::ConnectServer(FString ServerIP)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIP));
}
