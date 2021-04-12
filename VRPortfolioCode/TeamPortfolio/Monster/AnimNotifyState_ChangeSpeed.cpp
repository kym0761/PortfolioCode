// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ChangeSpeed.h"
#include "Monster.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ChangeSpeed::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->GetCharacterMovement()->MaxWalkSpeed = 0.1f;
	}
}

void UAnimNotifyState_ChangeSpeed::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UAnimNotifyState_ChangeSpeed::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AMonster* Monster = Cast<AMonster>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->WalkSpeed;
	}
}
