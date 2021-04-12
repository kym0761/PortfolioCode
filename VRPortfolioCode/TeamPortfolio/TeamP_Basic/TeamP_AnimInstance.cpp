// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamP_AnimInstance.h"
#include "TeamP_BasicPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"



void UTeamP_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	ATeamP_BasicPlayer* Pawn = Cast< ATeamP_BasicPlayer>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel()) {
		bIsCrouched = Pawn->bIsCrouched;
	
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();

		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, Pawn->GetActorRotation());
		
		bIsIronsight = Pawn->bIsIronsight;

		bIsReload = Pawn->bIsReload;

		bIsShotgun = Pawn->bIsShotgun;

		bIsWeaponChange = Pawn->bIsWeaponChange;

		bIsGranade = Pawn->bIsGranade;

		//bFireShotgun = Pawn->bFireShotgun;
		if (!bIsShotgun) {
			bIsFireAnim = Pawn->bIsFireAnim;
		}
		else { //샷건쏨?
				bIsFireAnim = Pawn->bFireShotgun;//쐇으면 애니메이션 동작
		}

		if (bIsGranade) {//수류탄 들었는가?

			if (bIsFireAnim) {// 공격눌럿는가?
				if (Pawn->ThrowGranadeMontage)
				{
					UE_LOG(LogClass, Warning, TEXT("ThrowStart_Montage"))

						FString SectionName = FString::Printf(TEXT("Throw_1"));
					Pawn->PlayAnimMontage(Pawn->ThrowGranadeMontage, 1.0f, FName(SectionName));

				}
			}

		}
		

		FRotator AimRotation = Pawn->GetAimOffset();
		AimYaw = AimRotation.Yaw;
		AimPitch = AimRotation.Pitch;


		if (Pawn->bIsWeaponChange && Pawn->ChangeWeaponMontage)
		{
			if (!Montage_IsPlaying(Pawn->ChangeWeaponMontage))
			{
				Pawn->PlayAnimMontage(Pawn->ChangeWeaponMontage);
			}
		}



		if (Pawn->bIsReload && Pawn->ReloadMontage)
		{
			if (!Montage_IsPlaying(Pawn->ReloadMontage))
			{
				Pawn->PlayAnimMontage(Pawn->ReloadMontage);
			}
		}

		if (Pawn->bIsDead && Pawn->DeadMontage)
		{
			if (!Montage_IsPlaying(Pawn->DeadMontage))
			{
				FString SectionName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
				Pawn->PlayAnimMontage(Pawn->DeadMontage, 1.0f, FName(SectionName));
			}
		}

	}
}

void UTeamP_AnimInstance::Animnotify_ReloadEnd(UAnimNotify* Animnotify)
{
	ATeamP_BasicPlayer* Pawn = Cast<ATeamP_BasicPlayer>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel()) 
	{
		bIsReload = false;
		Pawn->bIsReload = false;
	}
}

void UTeamP_AnimInstance::Animnotify_Throw_Ready(UAnimNotify* Animnotify)
{
	if (!bThrowReady) {
		UE_LOG(LogClass, Warning, TEXT("Ready"))
		bThrowReady = true;
	}
}


void UTeamP_AnimInstance::Animnotify_Throw_End(UAnimNotify* Animnotify)
{
	if (bThrowReady)
	{
		ATeamP_BasicPlayer* Pawn = Cast<ATeamP_BasicPlayer>(TryGetPawnOwner());
		if (Pawn) {
			Pawn->ThrowGranade();
			bThrowReady = false;
			UE_LOG(LogClass, Warning, TEXT("Throw"))

		}

	}
}
