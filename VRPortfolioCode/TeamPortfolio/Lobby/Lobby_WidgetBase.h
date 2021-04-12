// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lobby_WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ULobby_WidgetBase : public UUserWidget
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* TimeLeftMessage;


#pragma endregion

#pragma region Public Member
public:
	virtual void NativeConstruct() override;
	void SetMessage(int LeftTime, bool isLobby = true);

#pragma endregion
};
