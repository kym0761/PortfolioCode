// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameModeBase.h"
#include "BattleGameStateBase.h"
#include "BattlePlayerController.h"
#include "../Basic/BasicCharacter.h"
#include "BattlePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../Item/ItemPoint.h"
#include "../Item/MasterItem.h"

void ABattleGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CountAlivePlayer();

	ABattleGameStateBase* gameState = GetGameState<ABattleGameStateBase>();
	if (gameState)
	{
		//GetNumPlayers() in gamemode basic Function.
		gameState->TotalPerson = GetNumPlayers();
	}

}

void ABattleGameModeBase::Logout(AController * Exiting)
{

	CountAlivePlayer();

	Super::Logout(Exiting); // Do Last
}

void ABattleGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> itemPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemPoint::StaticClass(), itemPoints);

	for (int i = 0; i < itemPoints.Num(); ++i)
	{
		GetWorld()->SpawnActor<AMasterItem>(WantToSpawn,itemPoints[i]->GetActorTransform());

	}

}

void ABattleGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	
	UE_LOG(LogClass, Warning, TEXT("PostSeamlessTravel On"));

}

void ABattleGameModeBase::C2S_UpdateAlivePerson_Implementation()
{
	ABattleGameStateBase* gameState = GetGameState<ABattleGameStateBase>();
	if (gameState)
	{
		int32 temp = 0;
		for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
		{
			ABasicCharacter* playerCharacter = Cast<ABasicCharacter>(iter->Get()->GetPawn());
			if(playerCharacter)
			{
				if (!(playerCharacter->CurrentHP <= 0))
				{
					temp++;
				}
			}
		}
		gameState->AlivePerson = temp;
		//gameState->TotalPerson = temp;
		gameState->OnRep_AlivePerson();
	}
}

void ABattleGameModeBase::EndBattleGame()
{
	FTimerHandle timer;

	FTimerDelegate timerCallback;
	timerCallback.BindLambda([&]
		{
			GetWorld()->ServerTravel(TEXT("Lobby"));
		});

	GetWorldTimerManager().SetTimer(timer, timerCallback, 3.0f, false);


}

void ABattleGameModeBase::CountAlivePlayer()
{


	ABattleGameStateBase* gameState = GetGameState<ABattleGameStateBase>();
	if (gameState)
	{
		int32 temp = 0;
		for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
		{
			ABasicCharacter* playerCharacter = Cast<ABasicCharacter>(iter->Get()->GetPawn());
			if (playerCharacter)
			{
				if (playerCharacter->CurrentHP > 0)
				{
					temp++;
				}
			}
		}

		gameState->AlivePerson = temp;
		//gameState->TotalPerson = temp;
		gameState->OnRep_AlivePerson(); // server Rep Call

		for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
		{
			ABasicCharacter* playerCharacter = Cast<ABasicCharacter>(iter->Get()->GetPawn());
			if (playerCharacter)
			{
				if (playerCharacter->CurrentHP <= 0)
				{
					//rank
					ABattlePlayerState* ps = playerCharacter->GetPlayerState<ABattlePlayerState>();
					if (ps)
					{
						if (ps->PlayerRank == -1)
						{
							ps->PlayerRank = gameState->AlivePerson + 1;
						}
					}
				}
			}
		}

		if (gameState->AlivePerson == 1 && GetNumPlayers() > 1 )
		{
			////gameover;
			//UE_LOG(LogClass, Warning, TEXT("Gameover State maybe?"));

			for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
			{
				ABasicCharacter* playerCharacter = Cast<ABasicCharacter>(iter->Get()->GetPawn());
				if (playerCharacter)
				{
					if (playerCharacter->CurrentHP > 0)
					{
						playerCharacter->GetPlayerState<ABattlePlayerState>()->PlayerRank = 1;
					}
				}
			}

			EndBattleGame();

		}


	}


}
