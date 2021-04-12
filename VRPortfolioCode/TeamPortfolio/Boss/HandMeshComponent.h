// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "BossObject.h"
#include "Components/SkeletalMeshComponent.h"
#include "HandMeshComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class TEAMPORTFOLIO_API UHandMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:

	UHandMeshComponent();

	//State For Hand Pose
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "State")
		EHandState HandState;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
