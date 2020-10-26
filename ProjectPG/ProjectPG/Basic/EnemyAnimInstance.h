// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyCharacter.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPG_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		EZombieState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float Speed = 300.0f;

	UFUNCTION()
	void AnimNotify_Attack2(UAnimNotify* Nofity);
};
