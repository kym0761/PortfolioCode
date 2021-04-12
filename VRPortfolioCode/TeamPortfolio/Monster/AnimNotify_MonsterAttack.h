// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MonsterAttack.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API UAnimNotify_MonsterAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
