// Fill out your copyright notice in the Description page of Project Settings.


#include "BossProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshcomponent.h"
#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ABossProjectileBase::ABossProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(30.0f);
	Sphere->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	Sphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	Sphere->CanCharacterStepUpOn = ECB_No;

	SetRootComponent(Sphere);

	Sphere->SetSimulatePhysics(true);
	Sphere->SetEnableGravity(true);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetNotifyRigidBodyCollision(true);


	//MeshSetting
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -Sphere->GetScaledSphereRadius()));

	//Replication
	SetReplicates(true);
	SetReplicateMovement(true);
	MinNetUpdateFrequency = 30.0f;
	/* Deprecated.
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = Sphere;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
	*/

	bActivated = false;

	
	Sphere->SetAngularDamping(0.5f);

}

// Called when the game starts or when spawned
void ABossProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	// set up a notification for when this component hits something blocking.
	Sphere->OnComponentHit.AddDynamic(this, &ABossProjectileBase::OnHit);


}

// Called every frame
void ABossProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTrailNiagara)
	{
		CurrentTrailNiagara->SetNiagaraVariableVec3(FString("ProjectileVelocity"), GetVelocity().GetSafeNormal(0.0001f));
	}

}

void ABossProjectileBase::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{

	if (!bActivated)
	{
		return;
	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//Don't Impulse to Projectile.
		if (OtherActor->IsA(ABossProjectileBase::StaticClass()))
		{
			return;
		}

		if (HasAuthority())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}
		//Destroy();
	}
	else if (OtherActor != NULL && OtherActor != this && OtherComp != NULL) //Spawn Monster when Hit Ground -> Not SimulatingPhysics.
	{

		//Check Hit Location is Navigatable.
		UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		bool bOnNav = false;
		if (navSys)
		{
			FNavLocation temp;

			bOnNav = navSys->ProjectPointToNavigation(Hit.Location, temp);
			if (!bOnNav)
			{
				UE_LOG(LogClass, Warning, TEXT("Can't Spawn at the Wrong Location."));
				return;
			}
		}

		//Do This Only Server. It Has Own Function.
		ProjectileTask(Hit);

		Destroy();
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("other actor is null? otheractor is this? othercompo NULL?"));
		//De-Activate. wrong Position.
		bActivated = false;
	}


}

void ABossProjectileBase::ProjectileTask_Implementation(const FHitResult & Hit)
{
	StartFunction(Hit);
}

void ABossProjectileBase::StartFunction_Implementation(const FHitResult & Hit)
{
	UE_LOG(LogClass, Warning, TEXT("ABossProjectileBase::StartFunction. It is a Base Function. You Must Override it."));
}

void ABossProjectileBase::OnRep_bActivated()
{

	if (TrailNiagara && bActivated)
	{
		CurrentTrailNiagara =  UNiagaraFunctionLibrary::SpawnSystemAttached(TrailNiagara, RootComponent,FName("None"),FVector(0.0f,0.0f,0.0f),FRotator(0.0f,0.0f,0.0f),EAttachLocation::KeepRelativeOffset,true);
		if (CurrentTrailNiagara)
		{
			UE_LOG(LogClass, Warning, TEXT("Niagara Success"));
		}

	}

}

void ABossProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABossProjectileBase, bActivated);

}

