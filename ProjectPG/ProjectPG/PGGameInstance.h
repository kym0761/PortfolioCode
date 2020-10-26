// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API UPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	FString GetUserID();

	void SetUserID(FString NewUserID);

private:

	UPROPERTY(VisibleAnywhere, Category = "User")
	FString UserID;

};
