// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTipBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API UItemToolTipBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UTextBlock* ItemName;

	UFUNCTION(BlueprintCallable)
	void SetItemName(FString NewName);

};
