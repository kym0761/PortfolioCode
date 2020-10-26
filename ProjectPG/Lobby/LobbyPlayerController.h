// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ULobbyWidgetBase;

/**
 * 
 */
UCLASS()
class PROJECTPG_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<ULobbyWidgetBase> LobbyWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		ULobbyWidgetBase* LobbyWidgetObject;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Message);
	bool C2S_SendMessage_Validate(const FText& Message);
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client, Reliable)
		void S2C_SendMessage(const FText& Message);
	void S2C_SendMessage_Implementation(const FText& Message);
};
