// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContactWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UContactWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* ContactText;

#pragma endregion

#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetText(FString NewText);
#pragma endregion
};
