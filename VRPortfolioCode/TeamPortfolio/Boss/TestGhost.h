// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossCharacter.h"
#include "TestGhost.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ATestGhost : public ABossCharacter
{
	GENERATED_BODY()
public:

	virtual void CreateUI() override;
};
