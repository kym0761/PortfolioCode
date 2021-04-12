// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectButtonBase.generated.h"

UCLASS()
class TEAMPORTFOLIO_API ASelectButtonBase : public AActor
{
	GENERATED_BODY()
#pragma region Public Member
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	bool bIsDefencePlayer = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
	bool bIsPush = false;

#pragma endregion


#pragma region Public Function
public:	
	// Sets default values for this actor's properties
	ASelectButtonBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma endregion
};
