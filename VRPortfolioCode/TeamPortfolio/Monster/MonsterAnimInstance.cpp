// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (Monster && Monster->IsValidLowLevel())
	{
		CurrentState = Monster->CurrentState;
		Speed = Monster->GetCharacterMovement()->Velocity.Size();

	}
	
}

void UMonsterAnimInstance::AnimNotify_CheckAttack2(UAnimNotify* Notify)
{
	UE_LOG(LogClass, Warning, TEXT("Attack2"));
}
