// Fill out your copyright notice in the Description page of Project Settings.


#include "Granade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AGranade::AGranade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

		// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AGranade::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = Grenade_InitialSpeed;
	ProjectileMovement->MaxSpeed = Grenade_MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = Grenade_ProjectileGravityScale;

	// Die after 3 seconds by default
	


}

//void AGranade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	// Only add impulse and destroy projectile if we hit a physics
//	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
//	{
//		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
//
//	}
//}


