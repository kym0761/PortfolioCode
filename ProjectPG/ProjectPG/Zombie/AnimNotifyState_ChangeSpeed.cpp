// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ChangeSpeed.h"
#include "../Basic/EnemyCharacter.h"
void UAnimNotifyState_ChangeSpeed::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AEnemyCharacter* zombie =
		Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (zombie)
	{
		zombie->SetSpeed(3.0f);
	}
}
void UAnimNotifyState_ChangeSpeed::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{

}
void UAnimNotifyState_ChangeSpeed::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AEnemyCharacter* zombie =
		Cast<AEnemyCharacter>(MeshComp->GetOwner());

	if (zombie)
	{
		zombie->SetSpeed(150.0f);
	}
}