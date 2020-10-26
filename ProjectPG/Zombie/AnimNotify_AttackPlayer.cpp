// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackPlayer.h"

void UAnimNotify_AttackPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogClass, Warning, TEXT("Custom C++ Notify"));
}