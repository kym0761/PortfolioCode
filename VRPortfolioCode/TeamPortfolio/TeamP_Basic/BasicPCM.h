// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPCM.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ABasicPCM : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ABasicPCM();

	virtual void UpdateCamera(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV")
		float NormalFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV")
		float IronsightFOV = 60.0f;
	
	void Recoil();

};
