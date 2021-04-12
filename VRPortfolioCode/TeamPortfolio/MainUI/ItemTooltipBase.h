// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltipBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UItemTooltipBase : public UUserWidget
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* ItemName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* ItemBodyText;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UImage* ItemThumnail;
#pragma endregion

#pragma region Public Function
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetItemName(FString NewName);

	UFUNCTION(BlueprintCallable)
	void SetItemBodyText(FString NewBody);

	UFUNCTION(BlueprintCallable)
	void SetImage(TAssetPtr<class UTexture2D> Thumnail);

	UFUNCTION(BlueprintCallable)
	void SetTooltipsInfo(FString NewName, FString NewBody, TAssetPtr<class UTexture2D> Thumnail);
#pragma endregion
};
