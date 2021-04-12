// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "MonsterAIController.h"
#include "../Instance/TotalLog_GameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MonsterHpWidgetBase.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	Tags.Add(TEXT("Monster"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	Rotation = CreateDefaultSubobject<USceneComponent>(TEXT("Rotation"));
	//HPBar = CreateDefaultSubobject<UMonsterHpWidgetBase>(TEXT("HPBar"));
	HPBar3D = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar3D"));
	HPBar3D->SetWidgetClass(HpWidgetClass);

	Rotation->SetupAttachment(RootComponent);
	HPBar3D->SetupAttachment(Rotation);

	

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	SetSpeed(WalkSpeed);
	//OnRep_HPChanged();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AMonster::ProcessSeenPawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &AMonster::ProcessHeardPawn);
	}
	
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::ProcessSeenPawn(APawn* Pawn)
{
	if (Pawn->ActorHasTag(TEXT("Player")) &&
		CurrentState == EMonsterState::Normal)
	{
		UE_LOG(LogClass, Warning, TEXT("See %s"), *Pawn->GetName());
		SetCurrentState(EMonsterState::Chase);
		
		AMonsterAIController* AIC = GetController<AMonsterAIController>();
		if (AIC)
		{
			AIC->SetPlayer(Pawn);
		}

	}
}

void AMonster::ProcessHeardPawn(APawn* Pawn, const FVector& Location, float Volume)
{
	UE_LOG(LogClass, Warning, TEXT("Heard %s"), *Pawn->GetName());
}

void AMonster::SetCurrentState_Implementation(EMonsterState NewState)
{
	AMonsterAIController* AIC = GetController<AMonsterAIController>();
	if (AIC)
	{
		CurrentState = NewState;
		AIC->SetCurrnetState(NewState);
	}

	S2A_DisableUI(NewState);
}

void AMonster::SetSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonster, CurrentHP);
	DOREPLIFETIME(AMonster, CurrentState);
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f)
		return 0.0f;

	CurrentHP -= DamageAmount;
	OnRep_HPChanged();

	if (CurrentHP <= 0)
	{
		SetCurrentState(EMonsterState::Death);
	}

	return 0.0f;
}

void AMonster::Destroyed()
{
	UTotalLog_GameInstance* GI = Cast< UTotalLog_GameInstance>(GetGameInstance());

	if (IsValid(GI))
	{
		GI->Monster_Record[MonType] += 1;

		UE_LOG(LogClass, Warning, TEXT("%d"), GI->Monster_Record[MonType]);
	}		

	Super::Destroyed();
}

void AMonster::DamageProcess()
{
	if (HasAuthority())
	{
		C2S_DamageProcess();
	}
}


void AMonster::C2S_DamageProcess_Implementation()
{	
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("MonsterAttack"));

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjecTypes;
	ObjecTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));

	TArray<AActor*> IgnoreActors;
	//IgnoreActors.Add(Monster);

	TArray<AActor*>OutActors;

	bool bResult = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		200.0f,
		ObjecTypes,
		AActor::StaticClass(),
		IgnoreActors,
		OutActors

	);

	if (bResult)
	{
		UE_LOG(LogClass, Warning, TEXT("Monster Attack : %s"), OutActors[0]);

		UGameplayStatics::ApplyDamage(OutActors[0],
			10.0f,
			nullptr,
			this,
			nullptr
		);
	}
	
}

void AMonster::OnRep_HPChanged()
{
	if (HPBar3D)
	{
		UMonsterHPWidgetBase* Hp = Cast<UMonsterHPWidgetBase>(HPBar3D->GetUserWidgetObject());

		if (IsValid(Hp))
		{
			float Percent = CurrentHP / MaxHP;
			Hp->HpBarUpdate(Percent);
		}
	}
}


void AMonster::S2A_DisableUI_Implementation(EMonsterState NewState)
{
	if (NewState == EMonsterState::Death)
	{
		HPBar3D->SetVisibility(false);
	}
}