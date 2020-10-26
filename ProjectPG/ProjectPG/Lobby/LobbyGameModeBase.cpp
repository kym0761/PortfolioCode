// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"
#include "LobbyGameStateBase.h"
void ALobbyGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CountConnectCount();

}

void ALobbyGameModeBase::Logout(AController * Exiting)
{
	CountConnectCount();

	Super::Logout(Exiting); // Do Last
}

void ALobbyGameModeBase::CountConnectCount()
{
	ALobbyGameStateBase* gameState = GetGameState<ALobbyGameStateBase>();
	if (gameState)
	{
		gameState->ConnectCount = 0;
		for (auto iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
		{
			gameState->ConnectCount++;
			gameState->OnRep_ConnectCount(); // host call only
		}
	}
}

void ALobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(LobbyTimer, this,&ALobbyGameModeBase::DecreaseTime, 1.0f, true, 1.0f);
}

void ALobbyGameModeBase::DecreaseTime()
{
	ALobbyGameStateBase* gs = GetGameState<ALobbyGameStateBase>();

	if (gs)
	{
		gs->LeftTime--;
		gs->OnRep_LeftTime();
	
		if (gs->LeftTime <= 0)
		{
			//start game
			StartGame();
		}
	}
}

void ALobbyGameModeBase::StartGame()
{
	//서버의 맵을 이동
	GetWorld()->ServerTravel(TEXT("Battle"));
}
