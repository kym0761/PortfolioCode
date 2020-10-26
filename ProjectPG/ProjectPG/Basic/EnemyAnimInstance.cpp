// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (enemy && enemy ->IsValidLowLevel())
	{
		CurrentState = enemy->CurrentState;
		Speed = enemy->GetCharacterMovement()->Velocity.Size();
	}


}

void UEnemyAnimInstance::AnimNotify_Attack2(UAnimNotify * Nofity)
{
	UE_LOG(LogClass, Warning, TEXT("ATTACKED!"));
}
