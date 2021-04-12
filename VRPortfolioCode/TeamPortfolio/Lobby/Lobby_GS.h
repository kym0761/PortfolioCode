// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Lobby_GS.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ALobby_GS : public AGameStateBase
{
	GENERATED_BODY()

#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = "OnRep_LeftTime", Category = "Network")
	int LeftTime = 3;
#pragma endregion


#pragma region Public Function
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_LeftTime();

#pragma endregion


};
