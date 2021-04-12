// Fill out your copyright notice in the Description page of Project Settings.


#include "HandMeshComponent.h"
#include "Net/UnrealNetwork.h"

UHandMeshComponent::UHandMeshComponent()
{
	HandState = EHandState::Open;
}

void UHandMeshComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHandMeshComponent, HandState);
}
