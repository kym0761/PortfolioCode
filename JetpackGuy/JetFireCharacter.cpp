// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JetFireCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h" // to use MathLibrary
#include "Kismet/GameplayStatics.h"//get player controller
#include "Classes/Particles/ParticleSystemComponent.h" // particle System Component
#include "Public/UObject/ConstructorHelpers.h"//construction helper
AJetFireCharacter::AJetFireCharacter()
{


	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.f, 96.0f);
	
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 750.f;
	CameraBoom->SocketOffset = FVector(0.0f,0.0f,0.0f);
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
	CameraBoom->ProbeSize = 1.0f;


	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	//GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	StunEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StunEffect")); // ParticleSystemComponent for Stun
	StunEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); // Use Relative Location

	StunEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // default Location of Stun Particle System
	static ConstructorHelpers::FObjectFinder<UParticleSystem> particleEffectObj(TEXT("/Game/Effect/StunEffect.StunEffect") );	//ParticleSystem'/Game/StunEffect/StunEffect.StunEffect'
	if (particleEffectObj.Succeeded())//get Particle system Template and apply it to Particle System Component
	{
		StunEffect->Template = particleEffectObj.Object;
	}
	StunEffect->bAutoActivate = false; // the default is UnActivate.


	Fuel = 100.0f;
	JetPower = 27.0f;
	JetPower_Forward = 12.5f;
	FuelReduction = 35.0f;
	FuelCharge = 30.0f;
	FuelCharge_default = FuelCharge;
	LerpOffset = 0.01f;

	StunCount = 0.0f;
	IsStunned = false;

	GetCharacterMovement()->MaxWalkSpeed = 0.0f; // 이동속도 0 으로 하면 캐릭터가 방향키로 움직이지 않음.
	GetCharacterMovement()->bConstrainToPlane = true; // Plane Constrain을 설정해서 원하는 방향으로만 움직이게 고정.
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 0.0f)); // 움직이는 Plane의 축을 (1,0,0)으로 하여 왼쪽 혹은 오른쪽으로만 움직임
	GetCharacterMovement()->SetWalkableFloorAngle(30.0f);//걸을 수 있는 기울기
	GetCharacterMovement()->GravityScale = 1.5f;//gravity scale 조절
	GetCharacterMovement()->MaxAcceleration = 16.0f; // 방향 바꾸는 속도?
	GetCharacterMovement()->AirControl = 0.1f; // 공중에서 방향 바꿀 시에 날아가는 속도에 얼마나 영향을 끼칠 것인가?
	GetCharacterMovement()->Mass = 100.0f;//캐릭터 질량.


}

//////////////////////////////////////////////////////////////////////////
// Input

void AJetFireCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("Jet", this, &AJetFireCharacter::FireJet);
}

void AJetFireCharacter::BeginPlay() 
{
	Super::BeginPlay();

	//만약 에디터에서 연료 충전량을 바꿨다면 조절해줘야함.
	FuelCharge_default = FuelCharge;

	//CapsuleComp에 감지 기능을 넣는다.
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AJetFireCharacter::OnComponentHit);

	//마우스 커서가 보이도록 함.
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

void AJetFireCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Fuel < 100.0f)//연료가 풀이 아니라면 연료를 채워야함.
	{
		if (GetCharacterMovement()->IsFalling() == true) // 공중에 있는 동안은
		{
			if (FuelCharge > FuelCharge_default) // 빨라진 FuelCharge 속도를 초기화.
			{
				FuelCharge = FuelCharge_default;
			}
		}
		else // 땅에 있는 동안은 연료가 빠르게 참.
		{
			Fuel = FMath::Clamp<float>(Fuel + (FuelCharge * DeltaSeconds), 0.0f, 100.0f);
			FuelCharge += FuelCharge_default / 5 * DeltaSeconds;// Fuelcharge를 살짝 가속.
		}
	}

	
	if (IsStunned == true) //스턴에 걸렸다면.
	{
		if (GetCharacterMovement()->IsFalling() == false && StunCount < MaxStunCount)//스턴 카운트
		{
			StunCount += DeltaSeconds;
		}
		else if(StunCount>=MaxStunCount)//스턴 카운트가 완료되면 스턴을 풀어준다.
		{
			IsStunned = false;
			EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			GetCharacterMovement()->GravityScale = 1.5f;
			StunEffect->DeactivateSystem(); //Deactivate Stun Effect
		}
	}
	
	/* // 지금은 사용하지 않음.
if(GetCharacterMovement()->IsFalling() == true) // 공중에서 LerpOffset만큼 rotation을 (0,1,0)로 조정(기본 forward vector)
{
	FRotator temp = UKismetMathLibrary::VLerp(GetActorRotation().Vector(),
		FVector(0.0f, GetActorForwardVector().Y, 0.0f), LerpOffset).Rotation();
	SetActorRotation(temp);
}*/

	//UE_LOG(LogTemp, Warning, TEXT(" z velocity : %f"), GetCharacterMovement()->Velocity.Z);
}
void AJetFireCharacter::FireJet_Implementation(float Value)
{

	/*//keyboard version

	if (Fuel > 0.0f && Value > 0.0f)
	{
		float upPower = JetPower;
		float forwardPower = JetPower_Forward;

		//x값을 뺀 forwardvector (x 방향으로 안 움직이게)
		FVector forwardVector = GetActorForwardVector();
		forwardVector.X = 0.0f;

		//x값을 뺀 upvector
		FVector upVector = GetActorUpVector();
		upVector.X = 0.0f;

		//캐릭터를 upVector forward vector 방향으로 힘에 맞춰서 발사함.
		//false의 뜻은 velocity를 해당 방향으로 변경하지 않고 기존 velocity에 add한다는 개념.
		LaunchCharacter(upVector * upPower + forwardVector * forwardPower, false, false);

		//fuel reduction with deltatime
		Fuel -= FuelReduction * FApp::GetDeltaTime();
		Fuel = FMath::Clamp<float>(Fuel, 0.0f, 100.0f);
	} */


	//Mouse Version
	
	FVector mousePos;
	FVector mouseDir;
	//마우스의 화면에 해당되는 X Y Z 좌표를 구한다.
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(mousePos, mouseDir);
	//UE_LOG(LogTemp, Warning, TEXT("MouseDirection : %s"), *mouseDir.ToString());
	
	//캐릭터를 마우스 방향에 맞춰 방향을 바꿔준다.
	AddMovementInput(FVector(0.f, 1.f, 0.f), mouseDir.Y);



	if (Fuel > 0.0f && Value > 0.0f)
	{
		GetCharacterMovement()->GravityScale = 1.0f; //날아다닐 땐 중력을 약하게함.

		//X 좌표 제거한 발사용 벡터
		FVector launchVector(0.0f, mousePos.Y - GetActorLocation().Y, mousePos.Z - GetActorLocation().Z);
		launchVector.Normalize(); // 정규화

		//발사용 벡터에 Jet Power를 곱한다.
		launchVector.Y *= JetPower_Forward;
		launchVector.Z *= JetPower;
		launchVector *= UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) *60.0f;
		//UE_LOG(LogTemp, Warning, TEXT("Launch Vector : %s"), *launchVector.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("deltatime : %f"), FApp::GetDeltaTime());
		//UE_LOG(LogTemp, Warning, TEXT("deltatime x 60 : %f"), FApp::GetDeltaTime()*60.0f);

		//캐릭터를 upVector forward vector 방향으로 힘에 맞춰서 발사함.
		//false의 뜻은 velocity를 해당 방향으로 변경하지 않고 기존 velocity에 add한다는 개념.
		//FPS60 기준 파워로 캐릭터를 발사한다.
		LaunchCharacter(launchVector, false, false);

		//GetCharacterMovement()->AddForce(launchVector);
		//GetCapsuleComponent()->AddForce(launchVector);

		//fuel reduction with deltatime
		Fuel = FMath::Clamp<float>(Fuel - (FuelReduction * FApp::GetDeltaTime()), 0.0f, 100.0f);
	}
	else if(Fuel == 0.0f || GetCharacterMovement()->IsFalling() == false) // 기름이 0이거나 혹은 땅바닥에 있을땐 중력을 강하게 함.
	{
		GetCharacterMovement()->GravityScale = 1.5f;
	}
}

void AJetFireCharacter::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("ImpactNormal : %s"), *Hit.ImpactNormal.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("normalImpulse %s"), *NormalImpulse.ToString());
	
	float dotProduct = FVector::DotProduct(Hit.ImpactNormal, GetActorUpVector()); // Inner Product 로 -0.? 값이 나오면 벽에 부딫쳤다고 판단.

	//UE_LOG(LogTemp, Warning, TEXT(" z velocity while hit : %f"), GetCharacterMovement()->Velocity.Z);
	//UE_LOG(LogTemp, Warning, TEXT("dot product : %f"), dotProduct);

	if (dotProduct < -0.75f && GetCharacterMovement()->IsFalling() == true) //머리가 벽에 부딫히면 스턴에 걸린다.
	{
		//UE_LOG(LogTemp, Warning, TEXT("Velocity : %f"), GetCharacterMovement()->Velocity.Size());
		IsStunned = true;
		StunCount = 0.0f;
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		//UE_LOG(LogTemp, Warning, TEXT("lost Control at OnCompHit"));
		GetCharacterMovement()->GravityScale = 5.0f;
		StunEffect->ActivateSystem(); // stun Effect

		StunSoundDelegate.Broadcast();
	}
	
}

float AJetFireCharacter::GetCurrentFuel() const
{
	return Fuel;
}
float AJetFireCharacter::GetMaxFuel() const
{
	return MaxFuel;
}
bool AJetFireCharacter::GetIsStunned() const
{
	return IsStunned;
}

