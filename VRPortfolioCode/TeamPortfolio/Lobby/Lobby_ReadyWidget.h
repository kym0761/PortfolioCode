// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lobby_ReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ULobby_ReadyWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Button")
	class UButton* UnReadyButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Button")
	class UButton* ReadyButton;

#pragma endregion

#pragma region Public Member
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Ready();

	UFUNCTION()
	void UnReady();

#pragma endregion

};
