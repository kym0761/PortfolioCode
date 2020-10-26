// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API UBasicAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		float Direction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		uint64 bIsFire : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		uint64 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		uint64 bIsIronsight : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		uint64 bIsCrouched: 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		uint64 bIsReloaded : 1;

	UFUNCTION()
	void AnimNotify_ReloadEnd(UAnimNotify* AnimNotify);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float CurrentLeanAngle = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float AimPitch;

};
