// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

class UEditableTextBox;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECTPG_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	//UMG와 Control 변수 연결.
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
		UEditableTextBox* UserID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
		UEditableTextBox* UserPassword;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
		UEditableTextBox* ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
		UButton* ConnectButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Control")
		UButton* StartButton;


	UFUNCTION()
		void OnConnectionButton();

	UFUNCTION()
		void OnStartButton();

	void SaveUserID();
};
