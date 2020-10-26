// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

class UEditableTextBox;
class UScrollBox;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTPG_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UEditableTextBox* ChatInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UScrollBox* ChatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextBlock* ConnectCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextBlock* PublicMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UButton* StartGameButton;

	UFUNCTION()
	void PressStartGameButton();

	UFUNCTION()
	void ProcessTextCommit(const FText& Text, ETextCommit::Type CommitMethod);

	void AddMessage(FText Message);

	//virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

	void SetConnectCount(int32 Count);

	void SetPublicMessage(int32 LeftTime);

	void HideStartGameButton();

};
