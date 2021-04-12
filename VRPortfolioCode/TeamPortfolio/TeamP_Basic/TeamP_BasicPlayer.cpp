// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamP_BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameframeWork/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.h"
#include "TeamP_BasicPC.h"
#include "BulletDamageType.h"
#include "Components/DecalComponent.h"
#include "SpawnTest.h"
#include "../MainUI/MainUIBase.h"
#include "../Item/Inventory.h"
#include "../Item/MasterItem.h"
#include "Weapon0.h"
#include "../MainUI/WeaponInfoBase.h"
#include "Granade.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
// Sets default values
ATeamP_BasicPlayer::ATeamP_BasicPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	//카메라
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SocketOffset = FVector(0, 40.0f, 88.f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//무기 추가
	//Weapon1 = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon1"));
	//Weapon1->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	NormalSpringArmPosition = SpringArm->GetRelativeLocation();
	CrouchedSpringArmPosition = NormalSpringArmPosition + FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2);

	Tags.Add(TEXT("Player"));


}

// Called when the game starts or when spawned
void ATeamP_BasicPlayer::BeginPlay()
{

	Super::BeginPlay();


	//CurrentWeaponBullet.Init(4,0)

	if (HasAuthority()) {

		for (int i = 0; i < WeaponClasses.Num(); i++) {

			AWeapon0* WeaponBulletCount = Cast<AWeapon0>(WeaponClasses[i].GetDefaultObject());

			//UE_LOG(LogClass, Warning, TEXT("BeginPlay [%d]"),i);

			//UE_LOG(LogClass, Warning, TEXT("valid? : %d"), IsValid(WeaponBulletCount));

			if (WeaponBulletCount)			//총알갯수 받아옴
			{
				CurrentWeaponBullet.Add(WeaponBulletCount->CurrentBullet);
				RemainedWeaponBullet.Add(WeaponBulletCount->RemainedBullet);
				UE_LOG(LogClass, Warning, TEXT("CurrentWeaponBullet[%d] = %d,RemainedWeaponBullet[%d] = %d"), i, CurrentWeaponBullet[i], i, RemainedWeaponBullet[i]);

			}
		}

	}

	WeaponChange(0);
}

// Called every frame
void ATeamP_BasicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ATeamP_BasicPC* PC = GetController<ATeamP_BasicPC>();
	//if (IsValid(PC))
	//{
	//	PC->GetMainUI()->UpdateHpBar(CurrentHP / MaxHP);
	//}
}

// 플레이어 입력
void ATeamP_BasicPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATeamP_BasicPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATeamP_BasicPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATeamP_BasicPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATeamP_BasicPlayer::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATeamP_BasicPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ATeamP_BasicPlayer::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ATeamP_BasicPlayer::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ATeamP_BasicPlayer::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ATeamP_BasicPlayer::StartCrouch);

	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &ATeamP_BasicPlayer::StartIronsight);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &ATeamP_BasicPlayer::StopIronsight);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ATeamP_BasicPlayer::Reload);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATeamP_BasicPlayer::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATeamP_BasicPlayer::StopFire);
	//??

	//무기변경입력, 인벤토리 추가해서 인벤토리에서 무기바꿀시 삭제 or 변경

	PlayerInputComponent->BindAction<FBindActionParamDelegate>(TEXT("Weaponchange1"), IE_Pressed, this, &ATeamP_BasicPlayer::WeaponChangeTrigger,0);
	PlayerInputComponent->BindAction<FBindActionParamDelegate>(TEXT("Weaponchange2"), IE_Pressed, this, &ATeamP_BasicPlayer::WeaponChangeTrigger,1);
	PlayerInputComponent->BindAction<FBindActionParamDelegate>(TEXT("Weaponchange3"), IE_Pressed, this, &ATeamP_BasicPlayer::WeaponChangeTrigger,2);
	PlayerInputComponent->BindAction<FBindActionParamDelegate>(TEXT("Weaponchange4"), IE_Pressed, this, &ATeamP_BasicPlayer::WeaponChangeTrigger, 3);

	PlayerInputComponent->BindAction<FBindActionParamDelegate>(TEXT("ChangeGranade"), IE_Pressed, this, &ATeamP_BasicPlayer::ChangeGranade, 4);

	PlayerInputComponent->BindAction(TEXT("Heal"), IE_Pressed, this, &ATeamP_BasicPlayer::Heal);
	PlayerInputComponent->BindAction(TEXT("Root"), IE_Pressed, this, &ATeamP_BasicPlayer::Root);

}

//앞뒤좌우이동, 상하좌우카메라
void ATeamP_BasicPlayer::MoveForward(float AxisValue)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);

	FVector CameraForward = UKismetMathLibrary::GetForwardVector(YawBaseRotation);

	AddMovementInput(CameraForward, AxisValue);

}

void ATeamP_BasicPlayer::MoveRight(float AxisValue)
{

	FVector CameraLocation;
	FRotator CameraRotation;

	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);

	FVector CameraRight = UKismetMathLibrary::GetRightVector(YawBaseRotation);

	AddMovementInput(CameraRight, AxisValue);

}

void ATeamP_BasicPlayer::LookUp(float AxisValue)
{

	AddControllerPitchInput(AxisValue);

}

void ATeamP_BasicPlayer::Turn(float AxisValue)
{

	AddControllerYawInput(AxisValue);

}

void ATeamP_BasicPlayer::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ATeamP_BasicPlayer::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}


//앉기
void ATeamP_BasicPlayer::StartCrouch()
{
	if (CanCrouch())
	{
		Crouch();
		UE_LOG(LogClass, Warning, TEXT("coruch"));
	}
	else
	{
		UnCrouch();
		UE_LOG(LogClass, Warning, TEXT("uncoruch"));
	}
}

void ATeamP_BasicPlayer::StartIronsight()
{
	bIsIronsight = true;
}

void ATeamP_BasicPlayer::StopIronsight()
{
	bIsIronsight = false;
}

FRotator ATeamP_BasicPlayer::GetAimOffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}


//데미지 처리

void ATeamP_BasicPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamP_BasicPlayer,CurrentHP);
	DOREPLIFETIME(ATeamP_BasicPlayer, CurrentWeapon);
	DOREPLIFETIME(ATeamP_BasicPlayer, CurrentWeaponBullet);
	DOREPLIFETIME(ATeamP_BasicPlayer, RemainedWeaponBullet);
	DOREPLIFETIME(ATeamP_BasicPlayer, bIsReload);
	DOREPLIFETIME(ATeamP_BasicPlayer, bIsWeaponChange); 
	DOREPLIFETIME(ATeamP_BasicPlayer, bIsGranade); 
	DOREPLIFETIME(ATeamP_BasicPlayer, bIsDead);
	DOREPLIFETIME(ATeamP_BasicPlayer, bFireShotgun);
	DOREPLIFETIME(ATeamP_BasicPlayer, bIsShotgun);
	DOREPLIFETIME(ATeamP_BasicPlayer, bIsFireAnim);
	
}

float ATeamP_BasicPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0) //본 이름 head에 맞으면 사망 
		{
			CurrentHP = 0;
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("Damage %f, HP : %f"), DamageAmount, CurrentHP);

			CurrentHP -= DamageAmount;
		}

		if (HitMontage)
		{
			FString SectionName = FString::Printf(TEXT("Hit%d"), FMath::RandRange(1, 4));
			PlayAnimMontage(HitMontage, 1.0f, FName(SectionName));
		}




	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //RadialDamage 처리
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);

		CurrentHP -= DamageAmount;

		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f HP : %f"), DamageAmount, CurrentHP);

	}
	else //모든 데미지 처리
	{
		UE_LOG(LogClass, Warning, TEXT("Damage %f, HP : %f"), DamageAmount, CurrentHP);

		CurrentHP -= DamageAmount;


	}

	if (CurrentHP <= 0)
	{
		//사망애니메이션
		bIsDead = true;

		DisableInput(Cast<APlayerController>(GetController()));
	}


	UpdateHpUI();	

	return 0.0f;
}


void ATeamP_BasicPlayer::OnSpawnFire()
{
}

void ATeamP_BasicPlayer::StartFire() //발사키 입력시 
{

	CheckCanFire();

	if (!bCanFire)
	{
		return;
	}

	if (bIsGranade) {
		if (HasAuthority()) {
			ThrowGranade_Start();//<<<
			return;
		}
		else {
			ThrowGranade_Start_Server();//<<<
			return;
		}
	}

	bIsFire = true;

	if (bIsShotgun) {

		CurrentWeapon->OnFireShotgun();
	}
	else {
		CurrentWeapon->OnFire();
	}

} 

void ATeamP_BasicPlayer::StopFire()
{
	if (bIsGranade)
	{
		if (HasAuthority()) {
			ThrowGranade_End();
			return;
		}
		else {
			ThrowGranade_End_Server();
			return;
		}

	}
	bIsFire = false;
	if (!bIsShotgun) {
		bIsFireAnim = false;
	}
}


void ATeamP_BasicPlayer::Reload_Implementation()
{
	if ( (CurrentWeapon->RemainedBullet == 0) ||CurrentWeapon->CurrentBullet == CurrentWeapon->MaxBullet || bIsWeaponChange|| bFireShotgun || bIsGranade) {

		UE_LOG(LogClass, Warning, TEXT("bIsWeaponChange = %d,bFireShotgun = %d, bIsGranade = %d"), bIsWeaponChange, bFireShotgun, bIsGranade);
		UE_LOG(LogClass, Warning, TEXT("CurrentWeapon->RemainedBullet == 0 : %d"), (CurrentWeapon->RemainedBullet == 0));
		UE_LOG(LogClass, Warning, TEXT("CurrentWeapon->CurrentBullet == CurrentWeapon->MaxBullet : %d"), (CurrentWeapon->CurrentBullet == CurrentWeapon->MaxBullet) );
		return;
	}

	bIsReload = true;

	if(!CurrentWeapon->RemainedBullet == 0){

		if ((CurrentWeapon->RemainedBullet + CurrentWeapon->CurrentBullet) >= CurrentWeapon->MaxBullet) {//남은총알+현재총알이 탄창보다 많을경우 

			CurrentWeapon->RemainedBullet -= (CurrentWeapon->MaxBullet - CurrentWeapon->CurrentBullet);// 남은총알 = 남은총알-(최대총알-현재총알(충전된총알))

			CurrentWeapon->CurrentBullet = CurrentWeapon->MaxBullet;//현재총알 = 최대총알

		}else{																					//남은총알이 30개 이하일  경우
		
			CurrentWeapon->CurrentBullet += CurrentWeapon->RemainedBullet;

			CurrentWeapon->RemainedBullet = 0;
			
		}
	}

	UpdateReloadUI();

	UE_LOG(LogClass, Warning, TEXT("CurrentBullet = %d,MaxBullet = %d, RemainedBullet = %d"), CurrentWeapon->CurrentBullet, CurrentWeapon->MaxBullet, CurrentWeapon->RemainedBullet);
}

void ATeamP_BasicPlayer::UpdateHpUI_Implementation()
{
	ATeamP_BasicPC* PC = GetController<ATeamP_BasicPC>();
	if (IsValid(PC))
	{
		PC->GetMainUI()->UpdateHpBar(CurrentHP / MaxHP);
	}
}

void ATeamP_BasicPlayer::UpdateReloadUI_Implementation()
{
	//ATeamP_BasicPC* PC = GetController<ATeamP_BasicPC>();

	//if (IsValid(PC) && IsValid(PC->GetMainUI()) && IsValid(PC->GetMainUI()->WeaponInfo))
	//{
	//	PC->GetMainUI()->WeaponInfo->SetIBulletNum(CurrentWeapon->CurrentBullet);
	//	PC->GetMainUI()->WeaponInfo->SetIBulletMaxNum(CurrentWeapon->RemainedBullet);

	//	FString newText = FString::Printf(TEXT("SpawnWeapon %d"), WeaponNumber);
	//	PC->GetMainUI()->WeaponInfo->SetItemName(newText);
	//}

	if (HasAuthority()) {
		UE_LOG(LogClass, Warning, TEXT("Server"));

	}
	else {
		UE_LOG(LogClass, Warning, TEXT("Client"));
	}

	ATeamP_BasicPC* PC = GetController<ATeamP_BasicPC>();
	if (IsValid(PC))
	{
		UE_LOG(LogClass, Warning, TEXT("UI.....CurrentBullet = %d,MaxBullet = %d, RemainedBullet = %d"), CurrentWeapon->CurrentBullet, CurrentWeapon->MaxBullet, CurrentWeapon->RemainedBullet);
		
		if (PC->MainWidgetObject)
		{
			if (PC->GetMainUI()->WeaponInfo)
			{
				PC->GetMainUI()->WeaponInfo->SetItemName(CurrentWeapon->WeaponName);
				PC->GetMainUI()->WeaponInfo->SetIBulletNum(CurrentWeapon->CurrentBullet);
				PC->GetMainUI()->WeaponInfo->SetIBulletMaxNum(CurrentWeapon->RemainedBullet);
			}
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("PC is Null"));

	}
}

//발사가능체크.
void ATeamP_BasicPlayer::CheckCanFire()
{
	// 리로드중인것도 체크해줘야댐
	
	if (CurrentWeapon->GetWorldTimerManager().IsTimerActive(CurrentWeapon->FireTimerHandle) || bIsReload || CurrentWeapon->CurrentBullet <= 0||bFireShotgun||bIsWeaponChange) {
		
		bCanFire = false;
		
	}
	else {
		bCanFire = true;
	}
}

void ATeamP_BasicPlayer::SpawnWeapon_Implementation(int WeaponNumber)
{

	
	FActorSpawnParameters params;
	params.Owner = this;

	AWeapon0* spawnWeapon = GetWorld()->SpawnActor<AWeapon0>(WeaponClasses[WeaponNumber], params);

	if (spawnWeapon)
	{
		spawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));//소켓은 스켈레탈메시에 있음.따라서 parent는 GetMesh()해준것.
		if (CurrentWeapon)
		{

			CurrentWeaponBullet[UsingWeaponNumber] = CurrentWeapon->CurrentBullet;

			RemainedWeaponBullet[UsingWeaponNumber] = CurrentWeapon->RemainedBullet;

			UsingWeaponNumber = WeaponNumber;

			CurrentWeapon->Destroy();
		}
		CurrentWeapon = spawnWeapon;

		CurrentWeapon->CurrentBullet = CurrentWeaponBullet[UsingWeaponNumber];

		CurrentWeapon->RemainedBullet = RemainedWeaponBullet[UsingWeaponNumber];

	}

	WeaponAttackSpeed = CurrentWeapon->WeaponAttackSpeed;
	WeaponDamageC = CurrentWeapon->WeaponDamage;
	////총알 바꾸기					//이제 사용할 무기의 총알 = 캐릭터가 저장해 놓은 총알
	//UE_LOG(LogClass, Warning, TEXT("WeaponNumber : %d  MaxBullet : %d  Bullet : %d / %d"), WeaponNumber, CurrentWeapon->MaxBullet, CurrentWeapon->CurrentBullet, CurrentWeapon->RemainedBullet);

}

void ATeamP_BasicPlayer::CheckCanAnimation()
{												//애니메이션 사용가능한가?
	if (bIsReload || bFireShotgun||bIsReload) {

		bCanAnimation = false;

	}
	else {
		bCanAnimation = true;
	}
}




//선택한 무기 표시



void ATeamP_BasicPlayer::WeaponChangeTrigger(int WeaponNumber)
{

	WeaponChange(WeaponNumber);
}

void ATeamP_BasicPlayer::WeaponChange_Implementation(int WeaponNumber)
{
	if (bIsReload || bIsWeaponChange || bFireShotgun||bIsFire) {
		return;
	}
	//현재 UsingWeaponNumber 은 기존에 사용하고있던 UsingWeaponNumber
	// 캐릭터가 저장한 총알 = 사용하고 있던 무기의 총알




	bIsGranade = false;

	UE_LOG(LogClass, Warning, TEXT("%d"), bIsGranade)

	

	bIsFire = false;
	bIsFireAnim = false;

	if (ChangeWeaponMontage) //문제있는 부분.
	{
		//FString SectionName = FString::Printf(TEXT("Hit%d"), FMath::RandRange(1, 4));
		UE_LOG(LogClass, Warning, TEXT("bIsWeaponChange"))
		bIsWeaponChange = true;
	}

	if (WeaponNumber == 3) {
		bIsShotgun = true;
		UE_LOG(LogClass, Warning, TEXT("WeaponNumber %d"), WeaponNumber);
	}
	else {
		bIsShotgun = false; 
	}
	
	SpawnWeapon(WeaponNumber);
	

	ATeamP_BasicPC* PC = GetController<ATeamP_BasicPC>();
	if (IsValid(PC))
	{

		UE_LOG(LogClass, Warning, TEXT("UI.....CurrentBullet = %d,MaxBullet = %d, RemainedBullet = %d"), CurrentWeapon->CurrentBullet, CurrentWeapon->MaxBullet, CurrentWeapon->RemainedBullet);
		if (PC->MainWidgetObject)
		{
			if (PC->GetMainUI()->WeaponInfo)
			{
				PC->GetMainUI()->WeaponInfo->SetItemName(CurrentWeapon->WeaponName);
				PC->GetMainUI()->WeaponInfo->SetIBulletNum(CurrentWeapon->CurrentBullet);
				PC->GetMainUI()->WeaponInfo->SetIBulletMaxNum(CurrentWeapon->RemainedBullet);
			}
		}
		
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("PC is Null"));

	}

	//GetWorld()->GetTimeSeconds();

	////header
	//TArray<float> Cooldown;

	//Cooldown->Add(0.0f);

	//tick()
	//	for ()
	//		Cooldown[i] += deltasecond;

	/*switch (WeaponNumber)
	{
	case 1: WeaponNumber = 1;
		Weapon1->SetHiddenInGame(false);
		Weapon2->SetHiddenInGame(true);
		Weapon3->SetHiddenInGame(true);
		Weapon4->SetHiddenInGame(true);
		break;
	case 2: WeaponNumber = 2;
		Weapon1->SetHiddenInGame(true);
		Weapon2->SetHiddenInGame(false);
		Weapon3->SetHiddenInGame(true);
		Weapon4->SetHiddenInGame(true);
		break;
	case 3: WeaponNumber = 3;
		Weapon1->SetHiddenInGame(true);
		Weapon2->SetHiddenInGame(true);
		Weapon3->SetHiddenInGame(false);
		Weapon4->SetHiddenInGame(true);
		break;
	case 4: WeaponNumber = 4;
		Weapon1->SetHiddenInGame(true);
		Weapon2->SetHiddenInGame(true);
		Weapon3->SetHiddenInGame(true);
		Weapon4->SetHiddenInGame(false);
		break;
	}*/
}


//무기 옵션들 




void ATeamP_BasicPlayer::LoadWeapon(int Index)
{
	//ATeamP_BasicPC* PC = GetController<ATeamP_BasicPC>();
	//if (IsValid(PC))
	//{
	//	switch (Index)
	//	{
	//	case 1:

	//		break;
	//	case 2:

	//		break;
	//	case 3:

	//		break;
	//	case 4:

	//		break;
	//	default:
	//		break;
	//	}	
	//	
	//	//PC->Inventory->Equipment[Index]->ItemData.;
	//	
	//	//PC->Inventory->Equipment[Index]->StaticMesh;//StaticMesh
	//}
}

void ATeamP_BasicPlayer::ChangeGranade_Implementation(int WeaponNumber)
{
	
	bIsGranade = true;


	UE_LOG(LogClass, Warning, TEXT("%d"), bIsGranade)

	FActorSpawnParameters params;
	params.Owner = this;
	//transform.SetLocation(GetActorLocation());

	AWeapon0* spawnWeapon = GetWorld()->SpawnActor<AWeapon0>(WeaponClasses[WeaponNumber], params);

	if (spawnWeapon)
	{
		spawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));//소켓은 스켈레탈메시에 있음.따라서 parent는 GetMesh()해준것.
		if (CurrentWeapon)
		{

			CurrentWeaponBullet[UsingWeaponNumber] = CurrentWeapon->CurrentBullet;

			RemainedWeaponBullet[UsingWeaponNumber] = CurrentWeapon->RemainedBullet;

			UsingWeaponNumber = WeaponNumber;

			CurrentWeapon->Destroy();
		}
		CurrentWeapon = spawnWeapon;

		CurrentWeapon->CurrentBullet = CurrentWeaponBullet[UsingWeaponNumber];

		CurrentWeapon->RemainedBullet = RemainedWeaponBullet[UsingWeaponNumber];
	}




	WeaponAttackSpeed = CurrentWeapon->WeaponAttackSpeed;
	WeaponDamageC = CurrentWeapon->WeaponDamage;
	
}

void ATeamP_BasicPlayer::ThrowGranade_Start_Server_Implementation()
{
	ThrowGranade_Start();
}

void ATeamP_BasicPlayer::ThrowGranade_End_Server_Implementation()
{
	ThrowGranade_End();
}

void ATeamP_BasicPlayer::ThrowGranade_Start_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("ThrowStart"))
	if (ThrowGranadeMontage)
	{
		UE_LOG(LogClass, Warning, TEXT("ThrowStart_Montage"))

		FString SectionName = FString::Printf(TEXT("Throw_1"));
		PlayAnimMontage(ThrowGranadeMontage, 1.0f, FName(SectionName));
	}
}

void ATeamP_BasicPlayer::ThrowGranade_End_Implementation()
{
	UE_LOG(LogClass,Warning,TEXT("ThrowEnd"))
	if (ThrowGranadeMontage)
	{

		FString SectionName = FString::Printf(TEXT("Throw_2"));
		PlayAnimMontage(ThrowGranadeMontage, 1.0f, FName(SectionName));
	}
}

void ATeamP_BasicPlayer::ThrowGranade_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("ThrowGranade_ThrowGranade"))
	// try and fire a projectile
	if (GranadeClass != NULL)
	{
		UE_LOG(LogClass, Warning, TEXT("ThrowGranade_ProjectileClass"))

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
				UE_LOG(LogClass,Warning,TEXT("ThrowGranade_World"))

				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("WeaponSocket"));

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// spawn the projectile at the muzzle
				World->SpawnActor<AGranade>(GranadeClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}

}

void ATeamP_BasicPlayer::Destroyed()
{
	Super::Destroyed();

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Destroy();
	}
}

void ATeamP_BasicPlayer::Root()
{
	AUI_PC* PC = Cast<AUI_PC>(GetController());

	if (PC)
	{
		PC->Pickup();
	}
}

void ATeamP_BasicPlayer::Heal()
{
	AUI_PC* PC = Cast<AUI_PC>(GetController());

	if (PC)
	{
		if(PC->HealItemUse() == true)

		C2S_Heal();
	}
}

void ATeamP_BasicPlayer::C2S_Heal_Implementation()
{
	UGameplayStatics::ApplyDamage(this, -20.0f, nullptr, nullptr,nullptr);
	HealEffect();
}

void ATeamP_BasicPlayer::HealEffect_Implementation()
{
	if (HealNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(HealNiagara, RootComponent, FName("None"), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), EAttachLocation::KeepRelativeOffset, true);
	}
}


