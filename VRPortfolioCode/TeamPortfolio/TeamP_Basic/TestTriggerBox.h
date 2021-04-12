// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TestTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPORTFOLIO_API ATestTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATestTriggerBox();

    UFUNCTION()
        void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);
	
};
