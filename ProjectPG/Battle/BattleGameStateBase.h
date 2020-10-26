// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API ABattleGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:

	UPROPERTY(/*Replicated,*/ VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_AlivePerson",Category = "Battle")
		int32 AlivePerson = 10000;

	UPROPERTY(/*Replicated,*/ VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Battle")
		int32 TotalPerson = -1;


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_AlivePerson();

};
