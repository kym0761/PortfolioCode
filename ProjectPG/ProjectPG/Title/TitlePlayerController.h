// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

class UTitleWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECTPG_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UTitleWidgetBase> TitleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UTitleWidgetBase* TitleWidgetObject;


	UFUNCTION(BlueprintCallable)
		void StartServer();
	UFUNCTION(BlueprintCallable)
		void ConnectServer(FString ServerIPAddress);


};
