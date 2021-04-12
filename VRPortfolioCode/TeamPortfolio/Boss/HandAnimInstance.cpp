// Fill out your copyright notice in the Description page of Project Settings.


#include "HandAnimInstance.h"
#include "HandMeshComponent.h"
void UHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	UHandMeshComponent* hand = Cast<UHandMeshComponent>(GetSkelMeshComponent());
	if (hand)
	{
		Grab = (int32)hand->HandState / 2.0f;
	}

}

void UHandAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

}
