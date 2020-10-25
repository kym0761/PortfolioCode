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

	GetCharacterMovement()->MaxWalkSpeed = 0.0f; // �̵��ӵ� 0 ���� �ϸ� ĳ���Ͱ� ����Ű�� �������� ����.
	GetCharacterMovement()->bConstrainToPlane = true; // Plane Constrain�� �����ؼ� ���ϴ� �������θ� �����̰� ����.
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 0.0f)); // �����̴� Plane�� ���� (1,0,0)���� �Ͽ� ���� Ȥ�� ���������θ� ������
	GetCharacterMovement()->SetWalkableFloorAngle(30.0f);//���� �� �ִ� ����
	GetCharacterMovement()->GravityScale = 1.5f;//gravity scale ����
	GetCharacterMovement()->MaxAcceleration = 16.0f; // ���� �ٲٴ� �ӵ�?
	GetCharacterMovement()->AirControl = 0.1f; // ���߿��� ���� �ٲ� �ÿ� ���ư��� �ӵ��� �󸶳� ������ ��ĥ ���ΰ�?
	GetCharacterMovement()->Mass = 100.0f;//ĳ���� ����.


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

	//���� �����Ϳ��� ���� �������� �ٲ�ٸ� �����������.
	FuelCharge_default = FuelCharge;

	//CapsuleComp�� ���� ����� �ִ´�.
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AJetFireCharacter::OnComponentHit);

	//���콺 Ŀ���� ���̵��� ��.
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

void AJetFireCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Fuel < 100.0f)//���ᰡ Ǯ�� �ƴ϶�� ���Ḧ ä������.
	{
		if (GetCharacterMovement()->IsFalling() == true) // ���߿� �ִ� ������
		{
			if (FuelCharge > FuelCharge_default) // ������ FuelCharge �ӵ��� �ʱ�ȭ.
			{
				FuelCharge = FuelCharge_default;
			}
		}
		else // ���� �ִ� ������ ���ᰡ ������ ��.
		{
			Fuel = FMath::Clamp<float>(Fuel + (FuelCharge * DeltaSeconds), 0.0f, 100.0f);
			FuelCharge += FuelCharge_default / 5 * DeltaSeconds;// Fuelcharge�� ��¦ ����.
		}
	}

	
	if (IsStunned == true) //���Ͽ� �ɷȴٸ�.
	{
		if (GetCharacterMovement()->IsFalling() == false && StunCount < MaxStunCount)//���� ī��Ʈ
		{
			StunCount += DeltaSeconds;
		}
		else if(StunCount>=MaxStunCount)//���� ī��Ʈ�� �Ϸ�Ǹ� ������ Ǯ���ش�.
		{
			IsStunned = false;
			EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			GetCharacterMovement()->GravityScale = 1.5f;
			StunEffect->DeactivateSystem(); //Deactivate Stun Effect
		}
	}
	
	/* // ������ ������� ����.
if(GetCharacterMovement()->IsFalling() == true) // ���߿��� LerpOffset��ŭ rotation�� (0,1,0)�� ����(�⺻ forward vector)
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

		//x���� �� forwardvector (x �������� �� �����̰�)
		FVector forwardVector = GetActorForwardVector();
		forwardVector.X = 0.0f;

		//x���� �� upvector
		FVector upVector = GetActorUpVector();
		upVector.X = 0.0f;

		//ĳ���͸� upVector forward vector �������� ���� ���缭 �߻���.
		//false�� ���� velocity�� �ش� �������� �������� �ʰ� ���� velocity�� add�Ѵٴ� ����.
		LaunchCharacter(upVector * upPower + forwardVector * forwardPower, false, false);

		//fuel reduction with deltatime
		Fuel -= FuelReduction * FApp::GetDeltaTime();
		Fuel = FMath::Clamp<float>(Fuel, 0.0f, 100.0f);
	} */


	//Mouse Version
	
	FVector mousePos;
	FVector mouseDir;
	//���콺�� ȭ�鿡 �ش�Ǵ� X Y Z ��ǥ�� ���Ѵ�.
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(mousePos, mouseDir);
	//UE_LOG(LogTemp, Warning, TEXT("MouseDirection : %s"), *mouseDir.ToString());
	
	//ĳ���͸� ���콺 ���⿡ ���� ������ �ٲ��ش�.
	AddMovementInput(FVector(0.f, 1.f, 0.f), mouseDir.Y);



	if (Fuel > 0.0f && Value > 0.0f)
	{
		GetCharacterMovement()->GravityScale = 1.0f; //���ƴٴ� �� �߷��� ���ϰ���.

		//X ��ǥ ������ �߻�� ����
		FVector launchVector(0.0f, mousePos.Y - GetActorLocation().Y, mousePos.Z - GetActorLocation().Z);
		launchVector.Normalize(); // ����ȭ

		//�߻�� ���Ϳ� Jet Power�� ���Ѵ�.
		launchVector.Y *= JetPower_Forward;
		launchVector.Z *= JetPower;
		launchVector *= UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) *60.0f;
		//UE_LOG(LogTemp, Warning, TEXT("Launch Vector : %s"), *launchVector.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("deltatime : %f"), FApp::GetDeltaTime());
		//UE_LOG(LogTemp, Warning, TEXT("deltatime x 60 : %f"), FApp::GetDeltaTime()*60.0f);

		//ĳ���͸� upVector forward vector �������� ���� ���缭 �߻���.
		//false�� ���� velocity�� �ش� �������� �������� �ʰ� ���� velocity�� add�Ѵٴ� ����.
		//FPS60 ���� �Ŀ��� ĳ���͸� �߻��Ѵ�.
		LaunchCharacter(launchVector, false, false);

		//GetCharacterMovement()->AddForce(launchVector);
		//GetCapsuleComponent()->AddForce(launchVector);

		//fuel reduction with deltatime
		Fuel = FMath::Clamp<float>(Fuel - (FuelReduction * FApp::GetDeltaTime()), 0.0f, 100.0f);
	}
	else if(Fuel == 0.0f || GetCharacterMovement()->IsFalling() == false) // �⸧�� 0�̰ų� Ȥ�� ���ٴڿ� ������ �߷��� ���ϰ� ��.
	{
		GetCharacterMovement()->GravityScale = 1.5f;
	}
}

void AJetFireCharacter::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("ImpactNormal : %s"), *Hit.ImpactNormal.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("normalImpulse %s"), *NormalImpulse.ToString());
	
	float dotProduct = FVector::DotProduct(Hit.ImpactNormal, GetActorUpVector()); // Inner Product �� -0.? ���� ������ ���� �΋H�ƴٰ� �Ǵ�.

	//UE_LOG(LogTemp, Warning, TEXT(" z velocity while hit : %f"), GetCharacterMovement()->Velocity.Z);
	//UE_LOG(LogTemp, Warning, TEXT("dot product : %f"), dotProduct);

	if (dotProduct < -0.75f && GetCharacterMovement()->IsFalling() == true) //�Ӹ��� ���� �΋H���� ���Ͽ� �ɸ���.
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

