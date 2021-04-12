// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpawnSlotBase.generated.h"

class UBorder;
class UProgressBar;
class UButton;
/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API USpawnSlotBase : public UUserWidget
{
	GENERATED_BODY()
public:
	
	//Just Slot Image.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		UBorder* ImageBorder;

	//Spawn Cooldown Showing Widget.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		UProgressBar* CooldownBar;
	
	//Button to Select Spawn Class.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		UButton* SelectButton;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnButtonClicked();

	void UpdateCooldown();

	int32 GetSlotNumber() const;
};
