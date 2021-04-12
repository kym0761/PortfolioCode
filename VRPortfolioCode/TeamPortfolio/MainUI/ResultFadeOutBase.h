// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultFadeOutBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UResultFadeOutBase : public UUserWidget
{
	GENERATED_BODY()

#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UTextBlock*> TextBoxArray;

	int TextBoxArray_Max = 5;

#pragma endregion 

	
#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	void SettingText();

#pragma endregion 

};
