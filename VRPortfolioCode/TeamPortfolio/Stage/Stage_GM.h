// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/Base_GM.h"
#include "Stage_GM.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API AStage_GM : public ABase_GM
{
	GENERATED_BODY()
#pragma region Public Member
public:

	//Time
	FTimerHandle GameTimer;	

#pragma endregion
	

#pragma region Public Function
public:
	AStage_GM();	

	UFUNCTION(BlueprintCallable)
	void StartCountDown();

	UFUNCTION()
	void DecreaseTime();

	UFUNCTION()
	void Travel_MasterLobby();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	
#pragma endregion
};
