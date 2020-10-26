// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAnimInstance.h"
#include "BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ABasicCharacter* character = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (character && character->IsValidLowLevel())
	{
		Direction = CalculateDirection(character->GetCharacterMovement()->Velocity, character->GetActorRotation());

		Speed = character->GetCharacterMovement()->Velocity.Size();

		bIsFire = character->bIsFire;
		bIsSprint = character->bIsSprint;
		bIsIronsight = character->bIsIronsight;
		bIsCrouched = character->bIsCrouched;

		bool bLeftLean = character->bLeftLean;
		bool bRightLean = character->bRightLean;

		float targetLeanAngle;
		if (bLeftLean && bRightLean)
		{
			targetLeanAngle = 0.0f;
		}
		else if (bLeftLean)
		{
			targetLeanAngle = -30.0f;
		}
		else if (bRightLean)
		{
			targetLeanAngle = 30.0f;
		}
		else
		{
			targetLeanAngle = 0.0f;
		}
		CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, targetLeanAngle, DeltaSeconds, 15.0f);

		FRotator aimRotation = character->GetAimOffset();

		AimYaw = aimRotation.Yaw;
		AimPitch = aimRotation.Pitch;

		bIsReloaded = character->bIsReloaded;

		if (character->ReloadMontage && character->bIsReloaded)
		{
			if (!Montage_IsPlaying(character->ReloadMontage))
			{
				character->PlayAnimMontage(character->ReloadMontage);
			}
		}

	}
}

void UBasicAnimInstance::AnimNotify_ReloadEnd(UAnimNotify * AnimNotify)
{
	ABasicCharacter* character = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (character && character->IsValidLowLevel())
	{
		bIsReloaded = false;
		character->bIsReloaded = false;
		character->C2S_SetReload(false);
	}
}
