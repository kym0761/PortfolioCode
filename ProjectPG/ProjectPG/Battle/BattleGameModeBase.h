
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameModeBase.generated.h"

class AMasterItem;
/**
 * 
 */
UCLASS()
class PROJECTPG_API ABattleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	//only Seamless Travel
	virtual void PostSeamlessTravel() override;

	UFUNCTION(Server, Reliable)
	void C2S_UpdateAlivePerson();
	void C2S_UpdateAlivePerson_Implementation();

	//UFUNCTION(Client , Reliable)
	//	void C2S_UpdateAlivePerson();
	//void C2S_UpdateAlivePerson_Implementation();

	void EndBattleGame();

	void CountAlivePlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMasterItem> WantToSpawn;

};
