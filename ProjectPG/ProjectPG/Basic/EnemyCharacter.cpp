// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIController.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));


	CurrentHP = 100.0f;
	MaxHP = 100.0f;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetSpeed();
	SetCurrentState(EZombieState::Normal);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::ProcessSeenPawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &AEnemyCharacter::ProcessHeardPawn);
	}

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f) // 이미 죽었으면 처리 안함.
	{
		return 0.0f;
	}

	//UE_LOG(LogClass, Warning, TEXT("Damage : %f"), DamageAmount);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* pointEvent = (FPointDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Point Damage -----> Bone Name : %s"), *pointEvent->HitInfo.BoneName.ToString());

		if (pointEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0 || pointEvent->HitInfo.BoneName.Compare(TEXT("Head")) == 0)
		{
			CurrentHP = 0.0f;
		}
		else
		{
			CurrentHP = FMath::Clamp<float>(CurrentHP - DamageAmount, 0.0f, MaxHP);
		}

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



	if (CurrentHP <= 0.0f)
	{
		//Destroy();

		////physics
		//GetMesh()->SetSimulatePhysics(true);
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetMesh()->AddImpulse(impulseDirection* 30000.0f, TEXT("head"));
		//SetLifeSpan(5.0f);

		SetCurrentState(EZombieState::Dead);

	}
	else
	{

	}

	return 0.0f;
}

void AEnemyCharacter::ProcessSeenPawn(APawn * Pawn)
{
	if (CurrentState != EZombieState::Dead)
	{
		UE_LOG(LogClass, Warning, TEXT("SEE %s"), *Pawn->GetName());

		SetCurrentState(EZombieState::Chase);

		AEnemyAIController* aiController = GetController<AEnemyAIController>();

		if (aiController)
		{
			aiController->SetPlayer(Pawn);
		}

	}
}

void AEnemyCharacter::ProcessHeardPawn(APawn* Pawn, const FVector & Location, float Volume)
{
	if (CurrentState != EZombieState::Dead)
	{
		UE_LOG(LogClass, Warning, TEXT("Heard %s"), *Pawn->GetName());
	}
}

void AEnemyCharacter::SetCurrentState(EZombieState NewState)
{
	CurrentState = NewState;

	//update Blackboard.
	AEnemyAIController* aiController = GetController<AEnemyAIController>();
	if (aiController)
	{
		aiController->SetCurrentState(NewState);
	}


}

void AEnemyCharacter::SetSpeed()
{

	switch (CurrentState)
	{
	case EZombieState::Normal:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;

	case EZombieState::Battle:
	case EZombieState::Dead:
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		break;

	case EZombieState::Chase:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	}
}

void AEnemyCharacter::SetSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}


