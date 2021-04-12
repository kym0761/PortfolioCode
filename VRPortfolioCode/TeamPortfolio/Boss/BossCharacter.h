// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStaticsTypes.h"/*FPredictProjectilePathParams & FPredictProjectilePathResult*/
#include "BossCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class ABossProjectileBase;
class USkeletalMeshComponent;
class UHandMeshComponent;
class UBossWidgetBase;
class UPhysicsHandleComponent;
class UMotionControllerComponent;
class USplineComponent;
class USplineMeshComponent;
class UNiagaraSystem;
class UWidgetComponent;
class ULobby_ReadyWidget;

UCLASS()
class TEAMPORTFOLIO_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

	//Camera Boon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* SpringArm;

	//Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* VR_Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMotionControllerComponent* VR_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMotionControllerComponent* VR_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UHandMeshComponent* VR_LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UHandMeshComponent* VR_RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<USplineMeshComponent*> SplineMeshArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* WidgetRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWidgetComponent* Widget_3D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWidgetComponent* Ready_3D;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	//Draw Trajectory in Tick. Deprecated.
	FPredictProjectilePathParams TrajectoryParams;
	FPredictProjectilePathResult Trajectoryresult;
	void DrawProjectileTrajectory();
	*/

	/*Movement Functions*/
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	////Fire & Spawn Monsters
	//void FireToSpawn();

	//List that You Can.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Monster")
		TArray<TSubclassOf<ABossProjectileBase>> ProjectileClasses;

	//Current Cooldown.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "Monster")
		TArray<float> SpawnCooldown;

	//MaxCoolDown.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "Monster")
		TArray<float> MaxSpawnCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UMG")
		TSubclassOf<UBossWidgetBase> BossWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UMG")
		TSubclassOf<ULobby_ReadyWidget> LobbyWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Grab")
		bool bIsGrabbed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Grab")
		bool bCanSeeTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		UNiagaraSystem* PositionNiagara;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Hold Projectile Actor. Call From "SearchHold"
	UFUNCTION(Server, Reliable)
		void HoldSpawnProjectile(ABossProjectileBase* ProjectileObject);
		void HoldSpawnProjectile_Implementation(ABossProjectileBase* ProjectileObject);

	//Release when it is holding something, if not hold anything, find actor to hold.
	void HandAction();

	void LeftHandAction();
	void RightHandPress();
	void RightHandRelease();

	virtual void CreateUI();

	//Search that Want to Hold, And Hold that.
	UFUNCTION(Server, Reliable)
		void SearchHold(FVector TraceStart, FVector TraceEnd);
	void SearchHold_Implementation(FVector TraceStart, FVector TraceEnd);

	//Release Will do at Server Only.
	UFUNCTION(Server, Reliable)
		void ReleaseHold();
	void ReleaseHold_Implementation();

	//UI Using.
	UFUNCTION(Server, Reliable)
		void SpawnProjectile(int32 Index);
	void SpawnProjectile_Implementation(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
		UStaticMesh* MeshForSpline;

	void DrawTrajectoryLine();

	UFUNCTION(Server, Reliable)
	void TrajectoryLineTeleport();
	void TrajectoryLineTeleport_Implementation();

	void SetWidget3DVisibility(bool Value);

	void SetReady3DVisibility(bool Value);

	UFUNCTION(Server, UnReliable)
		void SyncHandLocRot(FVector L_Loc, FRotator L_Rot, FVector R_Loc, FRotator R_Rot);
	void SyncHandLocRot_Implementation(FVector L_Loc, FRotator L_Rot, FVector R_Loc, FRotator R_Rot);

	UFUNCTION(Server, UnReliable)
		void SyncMeshRot(FRotator MeshRot);
	void SyncMeshRot_Implementation(FRotator MeshRot);

	UFUNCTION(Server, UnReliable)
		void SyncUMGRot(FRotator Rot);
	void SyncUMGRot_Implementation(FRotator Rot);
};
