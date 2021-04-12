// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Monster.h"

#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "State")
	EMonsterState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	float Speed;

	UFUNCTION()
	void AnimNotify_CheckAttack2(UAnimNotify* Notify);

};
