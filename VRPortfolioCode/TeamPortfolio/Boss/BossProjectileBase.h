// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossObject.h"
#include "GameFramework/Actor.h"
#include "BossProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent; 
UCLASS()
class TEAMPORTFOLIO_API ABossProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossProjectileBase();

	//Sphere Collision
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		USphereComponent* Sphere;

	/*Warning. This Actor Had Projectile Movement Component in the Past. But, after the change to Throwing PlayMode, Projectile Component is Deleted. So, Do Not Use This As Having Projectile Movement Component.*/

	//// Projectile movement component. Not Used when holding.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	//	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		USkeletalMeshComponent* Mesh;

	//true Means Activate OK.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, ReplicatedUsing = "OnRep_bActivated", Category = "Info")
		bool bActivated;

	//Info Has What Do you Want to Spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		FBossProjectileInfo ProjectileInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		UNiagaraSystem* TrailNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		UNiagaraComponent* CurrentTrailNiagara;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	
	/*
	ProjectileFunction. Do Implement virtual _Implementation() at Derived Class.
	It Does In Server Only.
	*/
	UFUNCTION(Server, Reliable)
		void ProjectileTask(const FHitResult& Hit);
		virtual void ProjectileTask_Implementation(const FHitResult& Hit);

	//It is Actual Function of Projectile. You Must Override In Blueprint or C++.
	UFUNCTION(BlueprintNativeEvent)
		void StartFunction(const FHitResult& Hit);
		virtual void StartFunction_Implementation(const FHitResult& Hit);

	UFUNCTION()
		void OnRep_bActivated();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
