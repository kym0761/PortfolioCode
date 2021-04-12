// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Title_UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UTitle_UserWidgetBase : public UUserWidget
{
	GENERATED_BODY()


#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UEditableTextBox* ServerIP;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* ConnectButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* StartButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* SinglePlayButton;

	//Multi
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* MultiPlayButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* MultiReturnButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UCanvasPanel* MultiWindow;

	//Option
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* OptionButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* OptionReturnButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UCanvasPanel* OptionWindow;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* ExitButton;
	
#pragma endregion

#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConnectButton();

	UFUNCTION()
	void OnStartButton();

	UFUNCTION()
	void OnSinglePlayButton();

	UFUNCTION()
	void OnMultiPlayButton();

	UFUNCTION()
	void OnOptionButton();

	UFUNCTION()
	void OnReturnButton();

	UFUNCTION()
	void OnExitButton();

	void SaveUserID();
#pragma endregion


};
