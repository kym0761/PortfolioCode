// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Lobby_GM.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ALobby_GM : public AGameModeBase
{
	GENERATED_BODY()
#pragma region Public Member
public:
	TArray<AActor*> Buttons;

#pragma endregion

#pragma region Public Function
public:
	virtual void StartGame();

	virtual void BeginPlay() override;

	virtual void Setting();

	//CheckButton
	FTimerHandle ButtonTimer;

	virtual void StartCheckButton();
	virtual void CheckButton();


	//Time
	FTimerHandle LobbyTimer;

	virtual void StartCountDown();
	virtual void DecreaseTime();

#pragma endregion
	
};
