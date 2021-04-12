// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:


	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

	// 0.0f == Open , 0.5f == CanGrab, 1.0f == Grab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float Grab = 0.0f;


};
