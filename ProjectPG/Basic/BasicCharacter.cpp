// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BulletDamageType.h"
#include "Components/DecalComponent.h"
#include "MyCameraShake.h"
#include "Net/UnrealNetwork.h"

#include "../Battle/BattleWidgetBase.h"
#include "../Battle/BattlePlayerController.h"
#include "../Battle/BattleGameModeBase.h"

#include "../Item/MasterItem.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SocketOffset = FVector(0.0f, 40.0f, 88.0f);
	//bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 88.0f;

	NormalSpringArmPosition = SpringArm->GetRelativeLocation();
	CrouchSpringArmPosition = NormalSpringArmPosition + FVector(0.0f,0.0f,-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()/2);

	CurrentHP = 100.0f;
	MaxHP = 100.0f;

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
//	OnRep_CurrentHP();
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward",this, &ABasicCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ABasicCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABasicCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasicCharacter::LookUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasicCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABasicCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABasicCharacter::Unsprint);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasicCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABasicCharacter::EndFire);
	PlayerInputComponent->BindAction("Ironsight", IE_Pressed, this, &ABasicCharacter::StartIronsight);
	PlayerInputComponent->BindAction("Ironsight", IE_Released, this, &ABasicCharacter::EndIronsight);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABasicCharacter::StartCrouch);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasicCharacter::Reload);
	
	PlayerInputComponent->BindAction("LeftLean", IE_Pressed, this, &ABasicCharacter::StartLeftLean);
	PlayerInputComponent->BindAction("LeftLean", IE_Released, this, &ABasicCharacter::EndLeftLean);

	PlayerInputComponent->BindAction("RightLean", IE_Pressed, this, &ABasicCharacter::StartRightLean);
	PlayerInputComponent->BindAction("RightLean", IE_Released, this, &ABasicCharacter::EndRightLean);

	PlayerInputComponent->BindAction("Pickup", IE_Released, this, &ABasicCharacter::Pickup);
}

void ABasicCharacter::MoveForward(float Value)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);


	//FRotator controlRot = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);

	FVector temp
		= UKismetMathLibrary::GetForwardVector(controlRot);

	AddMovementInput(temp, Value);
}

void ABasicCharacter::MoveRight(float Value)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);

	//FRotator controlRot = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);

	FVector temp
		= UKismetMathLibrary::GetRightVector(controlRot);

	AddMovementInput(temp, Value);
}

void ABasicCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABasicCharacter::LookUp(float Value)
{
	AddControllerPitchInput(-Value);
}

void ABasicCharacter::Sprint()
{
	bIsSprint = true;
	C2S_SetSprint(true);
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasicCharacter::Unsprint()
{
	bIsSprint =false;
	C2S_SetSprint(false);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasicCharacter::C2S_SetFire_Implementation(bool State)
{
	bIsFire = State;
}

void ABasicCharacter::C2S_SetSprint_Implementation(bool State)
{
	bIsSprint = State;
	GetCharacterMovement()->MaxWalkSpeed = State ? SprintSpeed : WalkSpeed;
}

void ABasicCharacter::C2S_SetIronsight_Implementation(bool State)
{
	bIsIronsight = State;
}

void ABasicCharacter::StartFire()
{
	bIsFire = true;
	C2S_SetFire(true);
	GetWorldTimerManager().SetTimer(FireTimer, this, &ABasicCharacter::OnFire, 0.12f, true, 0.0f);


	//OnFire();
}

void ABasicCharacter::EndFire()
{
	bIsFire = false;
	C2S_SetFire(false);
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ABasicCharacter::OnFire()
{
	if (!bIsFire)
	{
		return;
	}


	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		int32 sizeX, sizeY;
		int32 randx = FMath::RandRange(-10, 10);
		int32 randy = FMath::RandRange(3, 15);
		FVector crosshairLocation, crosshairDirection; // this is world location & direction.
		playerController->GetViewportSize(sizeX, sizeY);
		playerController->DeprojectScreenPositionToWorld(
			sizeX / 2 + randx, sizeY / 2 + randy, crosshairLocation, crosshairDirection);

		FVector cameraLocation;
		FRotator cameraRotation;
		playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

		////recoil
		//FRotator playerRot = GetControlRotation();
		//playerRot.Pitch += FMath::FRandRange(0.1f,0.25f);
		//GetController()->SetControlRotation(playerRot);


		////shaking camera
		//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UMyCameraShake::StaticClass());

		//world camera shaking
		if (WorldCameraShake)
		{
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), WorldCameraShake, FVector(0.0f, 0.0f, 0.0f), 3000.0f, 9000.0f, 1.0f, false);
		}

		FVector traceStart = cameraLocation;
		FVector traceEnd = traceStart + crosshairDirection * 100000.0f;

		C2S_ProcessFire(traceStart, traceEnd);
	}
}

void ABasicCharacter::StartIronsight()
{
	bIsIronsight = true;
	C2S_SetIronsight(true);
}

void ABasicCharacter::EndIronsight()
{
	bIsIronsight = false;
	C2S_SetIronsight(false);
}

void ABasicCharacter::StartCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ABasicCharacter::OnRep_CurrentHP()
{
	ABattlePlayerController* pc = Cast<ABattlePlayerController>(GetController());
	if (pc&&pc->IsLocalPlayerController()&&pc->BattleWidgetObject)
	{
		pc->BattleWidgetObject->UpdateHPBar(CurrentHP / MaxHP);
	}

}

float ABasicCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f) // 이미 죽었으면 처리 안함.
	{
		return 0.0f;
	}

	//UE_LOG(LogClass, Warning, TEXT("Damage : %f"), DamageAmount);

	FVector impulseDirection(0.0f, 0.0f, 0.0f);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* pointEvent = (FPointDamageEvent*)(&DamageEvent);

		//UE_LOG(LogClass, Warning, TEXT("Point Damage -----> Bone Name : %s"), *pointEvent->HitInfo.BoneName.ToString());

		if (pointEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0.0f;
		}
		else
		{
			CurrentHP = FMath::Clamp<float>(CurrentHP - DamageAmount, 0.0f, MaxHP);
		}
		impulseDirection = pointEvent->ShotDirection;
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* radialEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Radial Damage"));
	}
	else
	{
		CurrentHP = FMath::Clamp<float>(CurrentHP - DamageAmount, 0.0f, MaxHP);
	}

	OnRep_CurrentHP();// host only

	if (CurrentHP <= 0.0f)
	{
		//Destroy();

		////physics
		//GetMesh()->SetSimulatePhysics(true);
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->AddImpulse(impulseDirection* 30000.0f, TEXT("head"));
		//SetLifeSpan(5.0f);

		//animation ->애니메이션 몽타주 사용.
		S2A_PlayDeathMontage(FMath::RandRange(1, 3));

		//count alive player
		// this will call in server only because takedamage() always Only Execute in server
		ABattleGameModeBase* gm = Cast<ABattleGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gm)
		{
			gm->CountAlivePlayer();
		}



	}
	else
	{
		S2A_PlayHitMontage(FMath::RandRange(1, 4));
	}

	return 0.0f;
}

void ABasicCharacter::Reload()
{
	bIsReloaded = true;
	C2S_SetReload(true);
}

void ABasicCharacter::StartLeftLean()
{
	bLeftLean = true;
	C2S_SetLeftLean(true);
}

void ABasicCharacter::EndLeftLean()
{
	bLeftLean = false;
	C2S_SetLeftLean(false);
}

void ABasicCharacter::C2S_SetLeftLean_Implementation(bool State)
{
	bLeftLean = State;
}

void ABasicCharacter::StartRightLean()
{
	bRightLean = true;
	C2S_SetRightLean(true);
}

void ABasicCharacter::EndRightLean()
{
	bRightLean = false;
	C2S_SetRightLean(false);
}

void ABasicCharacter::C2S_SetRightLean_Implementation(bool State)
{
	bRightLean = State;
}

FRotator ABasicCharacter::GetAimOffset() const
{

	//world rotation -> local rotation
	//world space rotation -> world direction vector -> local space direction vector -> local space rotation

	FRotator rot = GetBaseAimRotation();

	FVector aimDirWS = rot.Vector();
	FVector aimDirLS = ActorToWorld().InverseTransformVectorNoScale(aimDirWS);

	FRotator aimRotLS = aimDirLS.Rotation();

	return aimRotLS;
}

void ABasicCharacter::C2S_ProcessFire_Implementation(FVector TraceStart, FVector TraceEnd)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;

	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	//objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));


	TArray<AActor*> ignores;
	ignores.Add(this);

	FHitResult hit;

	bool result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		objects,
		true,
		ignores,
		EDrawDebugTrace::None, //EDrawDebugTrace::ForDuration,
		hit,
		true,
		FLinearColor::Red,
		FLinearColor::Blue,
		5.0f
	);

	if (result)
	{
		//UE_LOG(LogClass, Warning, TEXT("Hit : %s"), *hit.GetActor()->GetName());

		S2A_SpawnHitEffectAndDecal(hit);

		

		////
		//UGameplayStatics::ApplyDamage(
		//	hit.GetActor(),
		//	30.0f,
		//	GetController(),
		//	this,
		//	UBulletDamageType::StaticClass()
		//);

		////
		//UGameplayStatics::ApplyRadialDamage(
		//	GetWorld(),
		//	25.0f, 
		//	hit.ImpactPoint, //위치
		//	500.0f, //범위
		//	UBulletDamageType::StaticClass(),
		//	ignores,
		//	this,
		//	GetController(),
		//	true, // 풀데미지 여부
		//	ECC_Visibility // 막힘 처리
		//);

		UGameplayStatics::ApplyPointDamage(
			hit.GetActor(),
			10.0f,
			-hit.ImpactNormal, //방향
			hit,
			GetController(),
			this,
			UBulletDamageType::StaticClass()
		);

		MakeNoise(1.0f, this, hit.ImpactPoint);

	}
	//UE_LOG(LogClass, Warning, TEXT("Fire"));
	S2A_SpawnMuzzleAndFlash();
}

void ABasicCharacter::S2A_SpawnMuzzleAndFlash_Implementation()
{
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, Weapon->GetSocketTransform(TEXT("Muzzle")));
	}

	if (WeaponSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound, Weapon->GetComponentLocation());
	}
}

void ABasicCharacter::S2A_SpawnHitEffectAndDecal_Implementation(FHitResult OutHit)
{
	if (Cast<ACharacter>(OutHit.GetActor()))
	{

		if (BloodEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, OutHit.ImpactPoint + (OutHit.ImpactNormal*10.0f));
		}
	}
	else
	{
		if (HitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, OutHit.ImpactPoint + (OutHit.ImpactNormal*10.0f));
		}

		//데칼은 스켈레톤 메시에 쓰면 안됨.
		UDecalComponent* NewDecal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			NormalDecal,
			FVector(10.0f, 10.0f, 10.0f),
			OutHit.ImpactPoint,
			OutHit.ImpactNormal.Rotation(),
			20.0f
		);

		NewDecal->SetFadeScreenSize(0.005f);
		//UE_LOG(LogClass, Warning, TEXT("Decal Spawn"));
	}
}

void ABasicCharacter::S2A_PlayDeathMontage_Implementation(int32 AnimNum)
{
	if (DeathMontage)
	{
		FString sectionName = FString::Printf(TEXT("Death_%d"), AnimNum);

		PlayAnimMontage(DeathMontage, 1.0f, FName(sectionName)); // 애님블루프린트의 defaultslot 을 사용한다.

		DisableInput(Cast<APlayerController>(GetController()));
	}
}

void ABasicCharacter::C2S_SetReload_Implementation(bool ReloadState)
{
	//bisReload = true;//?
	bIsReloaded = ReloadState;
	UAnimInstance* animInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;

	if (ReloadMontage && animInstance)
	{
		if (!animInstance->Montage_IsPlaying(ReloadMontage))
		{
			PlayAnimMontage(ReloadMontage);
		}
	}
}

void ABasicCharacter::AddPickItem(AMasterItem * AddItem)
{
	PickItemList.Add(AddItem);
	ABattlePlayerController* pc = Cast<ABattlePlayerController>(GetController());
	if (pc)
	{
		pc->ShowItemToolTip(AddItem->ItemData.ItemName);
	}

}

void ABasicCharacter::RemovePickItem(AMasterItem * RemoveItem)
{
	PickItemList.Remove(RemoveItem);
	ABattlePlayerController* pc = Cast<ABattlePlayerController>(GetController());
	if (pc)
	{
		if (PickItemList.Num() > 0)
		{
			pc->ShowItemToolTip(PickItemList[PickItemList.Num()-1]->ItemData.ItemName);
		}
		else
		{
			pc->HideItemToolTip();
		}

	}
}

void ABasicCharacter::Pickup()
{
	if (PickItemList.Num() > 0)
	{
		//Server Pickup Check
		C2S_CheckPickupItem(PickItemList[PickItemList.Num() - 1]);
	}
}

void ABasicCharacter::C2S_CheckPickupItem_Implementation(AMasterItem * PickupItem)
{
	if (PickupItem && !PickupItem->IsPendingKill())
	{
		S2C_InsertItem(PickupItem);
		PickupItem->Destroy();
	}

}

void ABasicCharacter::S2C_InsertItem_Implementation(AMasterItem * PickupItem)
{
	AddToInventory(PickupItem);
}

void ABasicCharacter::AddToInventory(AMasterItem * Item)
{
	Inventory.Add(Item);
}

void ABasicCharacter::RemoveFromInventory(AMasterItem * Item)
{
	Inventory.Remove(Item);
}

void ABasicCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicCharacter, bIsFire);
	DOREPLIFETIME(ABasicCharacter, bIsSprint);
	DOREPLIFETIME(ABasicCharacter, bIsIronsight);
	DOREPLIFETIME(ABasicCharacter, CurrentHP);
	DOREPLIFETIME(ABasicCharacter, MaxHP);
	DOREPLIFETIME(ABasicCharacter, bLeftLean);
	DOREPLIFETIME(ABasicCharacter, bRightLean);
	DOREPLIFETIME(ABasicCharacter, bIsReloaded);
}


void ABasicCharacter::S2A_PlayHitMontage_Implementation(int AnimNum)
{
	if (HitReactMontage)
	{
		FString sectionName = FString::Printf(TEXT("Hit_React_%d"), AnimNum);

		PlayAnimMontage(HitReactMontage, 1.0f, FName(sectionName));
	}
}

