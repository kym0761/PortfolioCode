// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BattlePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API ABattlePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated , EditAnyWhere, BlueprintReadWrite, Category = "Rank")
	int32 PlayerRank = -1;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;


};
