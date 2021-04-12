// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatHoverBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UStatHoverBase : public UUserWidget
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTextBlock* TextBox;

#pragma endregion 

#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	void SettingText(FString NewText);

#pragma endregion 

};
