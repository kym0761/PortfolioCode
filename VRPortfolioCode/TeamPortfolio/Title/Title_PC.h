// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Title_PC.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ATitle_PC : public APlayerController
{
	GENERATED_BODY()
	
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UTitle_UserWidgetBase> TitleWidgetClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	class UTitle_UserWidgetBase* TitleWidgetObject;

#pragma endregion

#pragma region Public Function
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartServer();

	UFUNCTION(BlueprintCallable)
	void ConnectServer(FString ServerIP);

#pragma endregion
};
