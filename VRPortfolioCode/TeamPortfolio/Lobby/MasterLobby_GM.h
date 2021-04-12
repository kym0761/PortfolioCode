// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lobby_GM.h"
#include "MasterLobby_GM.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API AMasterLobby_GM : public ALobby_GM
{
	GENERATED_BODY()

#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class AUI_PC*> PC_Array;

#pragma endregion
	
#pragma region Public Function
public:
	virtual void StartGame() override;

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void GenericPlayerInitialization(AController* C) override;

	virtual void Setting() override;

	UFUNCTION()
	void StartCheck();
	void Check();

	virtual void StartCountDown() override;
	virtual void DecreaseTime() override;

#pragma endregion 

};
