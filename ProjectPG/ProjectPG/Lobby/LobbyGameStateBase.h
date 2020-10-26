// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API ALobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly,ReplicatedUsing = "OnRep_ConnectCount", Category = "Network")
		int32 ConnectCount = 0;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_ConnectCount();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_LeftTime", Category = "Network")
		int32 LeftTime = 60;

	UFUNCTION()
		void OnRep_LeftTime();
};
