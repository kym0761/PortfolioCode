// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterLobby_GM.h"
#include "../MainUI/UI_PC.h"
#include "../Item/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "../MainUI/UI_PC.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MasterLobby_GS.h"

void AMasterLobby_GM::StartGame()
{
	//GetWorld()->ServerTravel(TEXT("StageTestLevel"));
	GetWorld()->ServerTravel(TEXT("Snow_Level"));
}

void AMasterLobby_GM::BeginPlay()
{
	Super::BeginPlay();
}

void AMasterLobby_GM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUI_PC* PC = Cast<AUI_PC>(NewPlayer);

	if (PC)
		PC->AddReadyWidget();
}

void AMasterLobby_GM::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);
	AUI_PC* PC = Cast<AUI_PC>(C);

	if(PC)
		PC->AddReadyWidget();
}

void AMasterLobby_GM::Setting()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUI_PC::StaticClass(), Buttons);

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.ExecutionFunction = TEXT("StartCheck");
	Info.UUID = 123;
	Info.Linkage = 0;

	UE_LOG(LogClass, Warning, TEXT("Call Delay"));
	UKismetSystemLibrary::Delay(GetWorld(), 5.f, Info);
}

void AMasterLobby_GM::StartCheck()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUI_PC::StaticClass(), OutActors);

	for (int Index = 0; Index != OutActors.Num(); ++Index)
	{
		PC_Array.Add(Cast<AUI_PC>(OutActors[Index]));
	}

	GetWorldTimerManager().SetTimer(
		ButtonTimer, this,
		&AMasterLobby_GM::Check,
		1.0f, true, 1.0f
	);
}

void AMasterLobby_GM::Check()
{
	if (PC_Array.Num() == 2)
	{
		UE_LOG(LogClass, Warning, TEXT("%d, %d"), PC_Array[0]->IsReady, PC_Array[1]->IsReady);
		if (PC_Array[0]->IsReady && PC_Array[1]->IsReady)
		{
			GetWorldTimerManager().ClearTimer(ButtonTimer);
			StartCountDown();
		}
	}
	else if (PC_Array.Num() == 1)
	{
		UE_LOG(LogClass, Warning, TEXT("%d"), PC_Array[0]->IsReady);
		if (PC_Array[0]->IsReady)
		{
			GetWorldTimerManager().ClearTimer(ButtonTimer);
			StartCountDown();
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("AMasterLobby_GM : PC_Array = 0"));
	}
}

void AMasterLobby_GM::StartCountDown()
{
	GetWorldTimerManager().SetTimer(
		LobbyTimer, this,
		&AMasterLobby_GM::DecreaseTime,
		1.0f, true, 1.0f
	);
}

void AMasterLobby_GM::DecreaseTime()
{
	AMasterLobby_GS* GS = GetGameState<AMasterLobby_GS>();
	if (GS)
	{
		GS->LeftTime--;
		GS->OnRep_LeftTime();

		if (GS->LeftTime <= 0)
		{
			GetWorldTimerManager().ClearTimer(LobbyTimer);
			StartGame();
		}
	}
}