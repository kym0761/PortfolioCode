// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby_GM.h"
#include "Lobby_GS.h"
#include "Lobby_PC.h"
#include "Kismet/GameplayStatics.h"
#include "SelectButtonBase.h"
#include "../Instance/TotalLog_GameInstance.h"

void ALobby_GM::StartGame()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobby_PC::StaticClass(), OutActors);
	
	UE_LOG(LogClass, Warning, TEXT("%d"), OutActors.Num());

	for (int i = 0; i != OutActors.Num(); ++i)
	{
		ALobby_PC* PC = Cast<ALobby_PC>(OutActors[i]);
		PC->S2C_SetGIbool(PC->IsDefencePlayer);
	}

	//GetWorld()->ServerTravel(TEXT("CrumblingRuins_Level"));
	GetWorld()->ServerTravel(TEXT("Desert_Level"));
}

void ALobby_GM::BeginPlay()
{
	Super::BeginPlay();

	Setting();
}

void ALobby_GM::Setting()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASelectButtonBase::StaticClass(), Buttons);

	StartCheckButton();
}

void ALobby_GM::StartCheckButton()
{
	GetWorldTimerManager().SetTimer(
		ButtonTimer, this,
		&ALobby_GM::CheckButton,
		1.0f, true, 1.0f
	);
}

void ALobby_GM::CheckButton()
{
	if (Buttons.Num() == 2)
	{
		bool button0 = Cast<ASelectButtonBase>(Buttons[0])->bIsPush;
		bool button1 = Cast<ASelectButtonBase>(Buttons[1])->bIsPush;
		
		if (button0 && button1)
		{
			GetWorldTimerManager().ClearTimer(ButtonTimer);
			StartCountDown();
		}
	}
}

void ALobby_GM::StartCountDown()
{
	GetWorldTimerManager().SetTimer(
		LobbyTimer, this,
		&ALobby_GM::DecreaseTime,
		1.0f, true, 1.0f
	);
}

void ALobby_GM::DecreaseTime()
{
	ALobby_GS* GS = GetGameState<ALobby_GS>();
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