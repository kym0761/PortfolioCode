// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void CountConnectCount();

	virtual void BeginPlay() override;

	FTimerHandle LobbyTimer;

	UFUNCTION()
	void DecreaseTime();

	void StartGame();

};
