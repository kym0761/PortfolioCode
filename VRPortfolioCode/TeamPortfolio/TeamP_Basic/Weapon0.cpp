// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon0.h"
#include "TeamP_BasicPlayer.h"
#include "TeamP_BasicPC.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "BulletDamageType.h"
#include "Math/UnrealMathUtility.h"
#include "BasicPCM.h"
#include "GameFramework/Character.h"
#include "../MainUI/MainUIBase.h"
#include "../MainUI/WeaponInfoBase.h"
#include "Net/UnrealNetwork.h"

void AWeapon0::OnFire()
{
	
	ATeamP_BasicPlayer* Player = Cast<ATeamP_BasicPlayer>(GetOwner());//문제잇어?

	UE_LOG(LogClass, Warning, TEXT("OnFire"))

	ABasicPCM* PCM = Cast<ABasicPCM>(Player->GetController());

	//ABasicPCM* PCM =  Cast<ABasicPCM>(Player->Camera->GetPlayerCameraManager());
		
	if (Player) {



		if (!(Player->bIsFire)|| Player->bIsReload || Player->bIsWeaponChange)
		{
			UE_LOG(LogClass, Warning, TEXT("return onfire"))
			Player->bIsFire = false;
			Player->bIsFireAnim = false;
			return;
		}

	

		if (Player->CurrentWeapon->CurrentBullet <= 0)
		{
			Player->bIsFireAnim = false;
			return;
		}

		if (Player->bCanFire) //bCanFire로 변경
		{
			Player->bIsFireAnim = false;

			//UE_LOG(LogClass, Warning, TEXT("Bullet = %d / %d"), Player->CurrentWeapon->CurrentBullet, Player->CurrentWeapon->MaxBullet);

			StartRecoil();

			ATeamP_BasicPC* PC = Cast<ATeamP_BasicPC>(Player->GetController());

			if (PC)
			{
				//Effect1();
				int32 ScreenSizeX;
				int32 ScreenSizeY;

				FVector CrosshairWorldPosition;
				FVector CrosshairWorldDirection;

				FVector CameraLocation;
				FRotator CameraRotation;

				FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));

				PC->GetViewportSize(ScreenSizeX, ScreenSizeY);



				int RandX = FMath::RandRange(1, 2);
				int RandY = FMath::RandRange(1, 2);


				PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2 + RandX, ScreenSizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);
				PC->GetPlayerViewPoint(CameraLocation, CameraRotation);



				//FRotator PlayerRotation = Player->GetControlRotation();
				//PlayerRotation.Pitch += FMath::FRandRange(0.2f, 0.5f);
				//Player->GetController()->SetControlRotation(PlayerRotation);

				FVector TraceStart = CameraLocation;
				//FVector TraceStart = CameraLocation;
				FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 99999.f);

				//UE_LOG(LogClass, Warning, TEXT("Trace Start : %s, Trace End %s"), *TraceStart.ToString(), *TraceEnd.ToString());

				//C2S_ProcessFire(TraceStart, TraceEnd);

				/*------*/

				CalculateFire(TraceStart, TraceEnd);
				DecreaseBullet();
				/*-----*/

				//All Client Spawn Muzzleflash and Sound
				//S2A_SpawnMuzzleFlashAndSound();

				Effect2();


			}


			//총발사속도 제어용 타이머
			GetWorldTimerManager().SetTimer(FireTimerHandle,
				this,
				&AWeapon0::OnFire,
				WeaponAttackSpeed,
				false);


			//반동 애니메이션
			Player->bIsFireAnim = true;
		}
		else {//총발사이후 쿨타임동안 onfire다시 호출하는 타이머, 연사안되는 총을 onfire 연속호출(클릭연타)로 연사되도록하는거 방지함


			GetWorldTimerManager().SetTimer(FireTimerHandle2,
				this,
				&AWeapon0::OnFire,
				WeaponAttackSpeed / 4,
				false);
		}
	}
}

void AWeapon0::CalculateFire_Implementation(FVector TraceStart, FVector TraceEnd)
{

	ATeamP_BasicPlayer* Player = Cast<ATeamP_BasicPlayer>(GetOwner());
	if (Player)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

		//Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		//Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(GetOwner());
		ActorToIgnore.Add(this);

		FHitResult OutHit;

		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			Objects,
			true,
			ActorToIgnore,
			//EDrawDebugTrace::ForDuration,
			EDrawDebugTrace::None,
			OutHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);
		//UE_LOG(LogClass, Warning, TEXT("Result : %d"), Result);
		if (Result)
		{

			//effect
			Effect1(OutHit);	

			//all client spawn Hiteffect and Decal
			FString HitActor = OutHit.GetActor()->GetName();

			//UE_LOG(LogClass, Warning, TEXT("Result : %s"), *HitActor);


			//Point Damage
			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), //맞은놈
				WeaponDamage,	//데미지
				-OutHit.ImpactNormal,	//데미지 방향
				OutHit,	//데미지 충돌 정보
				Player->GetController(),	//때린 플레이어
				this,	//때린놈
				UBulletDamageType::StaticClass() //데미지 타입
			);

			MakeNoise(1.0f, Player, OutHit.ImpactPoint); //AI가 센서로 받을 수 있는 Noise를 생성한다.
		}
	}
}

void AWeapon0::DecreaseBullet_Implementation()
{
	CurrentBullet -= 1;
	OnRep_CurrentBullet();
}

void AWeapon0::OnCurrentBulletCheck_Implementation()
{
	ATeamP_BasicPlayer* Player = Cast<ATeamP_BasicPlayer>(GetOwner());
	ATeamP_BasicPC* PC = Cast<ATeamP_BasicPC>(Player->GetController());

	if (IsValid(PC))
	{
		//UE_LOG(LogClass, Warning, TEXT("OnCurrentBulletCheck .... Current Bullet : %d, RemainedBullet :%d"), CurrentBullet, RemainedBullet);

		if (IsValid(PC->MainWidgetObject))
		{
			PC->GetMainUI()->WeaponInfo->SetItemName(WeaponName);
			PC->GetMainUI()->WeaponInfo->SetIBulletNum(CurrentBullet);
			PC->GetMainUI()->WeaponInfo->SetIBulletMaxNum(RemainedBullet);
		}
		
	}
}

void AWeapon0::OnRep_CurrentBullet()
{
	OnCurrentBulletCheck();
}

void AWeapon0::Effect1_Implementation(FHitResult Hit)
{
	if(HasAuthority()){
		UE_LOG(LogClass, Warning, TEXT("Effect1.Server"))

	}
	else {
		UE_LOG(LogClass, Warning, TEXT("Effect1.Client"))
	}
	//S2A_SpawnHitEffectAndDecal(OutHit);
	if (Cast<ACharacter>(Hit.GetActor()))
	{

		//캐릭터
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			BloodHitEffect,
			Hit.ImpactPoint + (Hit.ImpactNormal * 10)
		);
	}
	else
	{
		//지형
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitEffect,
			Hit.ImpactPoint + (Hit.ImpactNormal * 10)
		);

		UDecalComponent* NewDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			NormalDecal,
			FVector(5, 5, 5),
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			10.0f
		);

		NewDecal->SetFadeScreenSize(0.005f);
	}

}

void AWeapon0::Effect2_Implementation()
{
	if (WeaponSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
			WeaponSound,
			WeaponMesh->GetComponentLocation()
		);
	}

	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			MuzzleFlash,
			WeaponMesh->GetSocketTransform(TEXT("Muzzle"))
		);
	}
}

void AWeapon0::OnFireShotgun()												//샷건
{
	ATeamP_BasicPlayer* Player = Cast<ATeamP_BasicPlayer>(GetOwner());

	UE_LOG(LogClass, Warning, TEXT("OnFireShotgun"))

		if (Player) {
			
				if (!(Player->bIsFire))
				{
					return;
				}


				if (Player->CurrentWeapon->CurrentBullet <= 0)
				{
					Player->bIsFireAnim = false;
					return;
				}

			if (Player->bIsShotgun) {
				Player->bFireShotgun = true;
			}

			if (Player->bCanFire) //bCanFire로 변경
			{

				Player->bIsFireAnim = false;


				UE_LOG(LogClass, Warning, TEXT("Bullet = %d / %d"), Player->CurrentWeapon->CurrentBullet, Player->CurrentWeapon->MaxBullet);

				StartRecoil();

				ATeamP_BasicPC* PC = Cast<ATeamP_BasicPC>(Player->GetController());

				if (PC)
				{

					int32 ScreenSizeX;
					int32 ScreenSizeY;

					FVector CrosshairWorldPosition;
					FVector CrosshairWorldDirection;

					FVector CameraLocation;
					FRotator CameraRotation;


					FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));

					FHitResult OutHit;

					for (int i = 0; i <= ShotgunCount;i++) {

						int RandX = FMath::RandRange(1, 30);
						int RandY = FMath::RandRange(1, 30);

						PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
						PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2 + RandX, ScreenSizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);

						PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

						//FRotator PlayerRotation = Player->GetControlRotation();
						//PlayerRotation.Pitch += FMath::FRandRange(0.2f, 0.5f);
						//Player->GetController()->SetControlRotation(PlayerRotation);

						FVector TraceStart = CameraLocation;
						FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 99999.f);

						//C2S_ProcessFire(TraceStart, TraceEnd);

						CalculateFire(TraceStart, TraceEnd);

						
					}

					DecreaseBullet();

					MakeNoise(1.0f, Player, OutHit.ImpactPoint); //AI가 센서로 받을 수 있는 Noise를 생성한다.

					Effect2();
					//All Client Spawn Muzzleflash and Sound
					//S2A_SpawnMuzzleFlashAndSound();
					//MakeNoise(1.0f, Player, OutHit.ImpactPoint); //AI가 센서로 받을 수 있는 Noise를 생성한다.

					

				}


				//총발사속도 제어용 타이머
				GetWorldTimerManager().SetTimer(FireTimerHandle,
					this,
					&AWeapon0::OnFireShotgun,
					WeaponAttackSpeed,
					false);


				//반동 애니메이션
				Player->bIsFireAnim = true;
				Player->bFireShotgun = true;
			}
			else {//총발사이후 쿨타임동안 onfire다시 호출하는 타이머, 연사안되는 총을 onfire 연속호출(클릭연타)로 연사되도록하는거 방지함


				GetWorldTimerManager().SetTimer(FireTimerHandle2,
					this,
					&AWeapon0::OnFireShotgun,
					WeaponAttackSpeed / 4,
					false);
			}
		}

}

void AWeapon0::OnFireGranade()
{
	ATeamP_BasicPlayer* Player = Cast<ATeamP_BasicPlayer>(GetOwner());//문제잇어?

	UE_LOG(LogClass, Warning, TEXT("OnFire"))
	
	if (Player->CurrentWeapon->CurrentBullet <= 0)
		{
			Player->bIsFireAnim = false;
			return;
		}

}

void AWeapon0::StartRecoil()
{

	InterpPitch = -WeaponRecoil;

	//FMath::FRandRange(WeaponRecoil-0.1f, WeaponRecoil+0.1f);
	
}


// Sets default values
AWeapon0::AWeapon0()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	SetReplicates(true);

	WeaponName = "DefaultName";
}


// Called when the game starts or when spawned
void AWeapon0::BeginPlay()
{
	Super::BeginPlay();
	
	OnCurrentBulletCheck();
}

// Called every frame
void AWeapon0::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* player = Cast<ACharacter>(GetOwner());
	if (player)
	{
		APlayerController* pc= player->GetController<APlayerController>();
		if (pc)
		{
			InterpPitch = FMath::FInterpTo(InterpPitch, 0.0f, DeltaTime, 20.0f);
			pc->AddPitchInput(InterpPitch);
		}
	}
}

void AWeapon0::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon0, CurrentBullet);
	DOREPLIFETIME(AWeapon0, RemainedBullet);
	DOREPLIFETIME(AWeapon0, InterpPitch);
}

void AWeapon0::OnRep_Owner()
{
	Super::OnRep_Owner();
}

