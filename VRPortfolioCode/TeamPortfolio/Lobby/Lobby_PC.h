// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Lobby_PC.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ALobby_PC : public APlayerController
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class ULobby_WidgetBase> LobbyWidgetClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	class ULobby_WidgetBase* LobbyWidgetObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	bool IsDefencePlayer = false;

#pragma endregion

#pragma region Public Function
public:
	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void S2C_SetGIbool(bool bValue); 
	void S2C_SetGIbool_Implementation(bool bValue);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;

#pragma endregion
};
