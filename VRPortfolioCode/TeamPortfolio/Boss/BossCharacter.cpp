// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BossProjectileBase.h"
#include "BossWidgetBase.h" //See Beginplay
#include "PhysicsEngine/PhysicsHandleComponent.h" // Physics Handle.
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/EngineTypes.h" //EObjectTypeQuery
#include "MotionControllerComponent.h" // Motion Controller.
#include "Net/UnrealNetwork.h"
#include "HandMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "../Lobby/Lobby_ReadyWidget.h"
#include "../MainUI/UI_PC.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Setting Appearance*/
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 88.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GetMesh()->bOwnerNoSee = true;
	//PhysicsHandle Component.
	PhysicsHandle  = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsHandle->SetIsReplicated(true);

	//Disable Use Controller Rotation on Character. Character Rotation will Followed by Character Movement.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	/* Deprecated. 
	//Trajectory Param's Default Parameter. Not Needed when Throwing PlayMode
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TrajectoryParams.ObjectTypes = objectTypes;
	TrajectoryParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	TrajectoryParams.SimFrequency = 15.0f;
	TrajectoryParams.MaxSimTime = 2.0f;
	TrajectoryParams.ProjectileRadius = 10.0f;
	*/



	VR_Root = CreateDefaultSubobject<USceneComponent>(TEXT("VR_Root"));
	VR_Root->SetupAttachment(RootComponent);


	VR_Left = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("VR_Left"));
	VR_Left->SetupAttachment(VR_Root);
	VR_Left->bDisplayDeviceModel = true;
	VR_Left->MotionSource = FName("Left");
	VR_Left->SetCollisionProfileName(FName("NoCollision"));
	VR_Left->SetIsReplicated(true);

	VR_Right = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("VR_Right"));
	VR_Right->SetupAttachment(VR_Root);
	VR_Right->bDisplayDeviceModel = true;
	VR_Right->MotionSource = FName("Right");
	VR_Right->SetCollisionProfileName(FName("NoCollision"));
	VR_Right->SetIsReplicated(true);


	VR_LeftHand = CreateDefaultSubobject<UHandMeshComponent>(TEXT("VR_LeftHand"));
	VR_LeftHand->SetupAttachment(VR_Left);
	VR_LeftHand->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f)); // Reverse Hand Mesh for Using Left
	VR_LeftHand->SetIsReplicated(true);

	VR_RightHand = CreateDefaultSubobject<UHandMeshComponent>(TEXT("VR_RightHand"));
	VR_RightHand->SetupAttachment(VR_Right);
	VR_RightHand->SetIsReplicated(true);


	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	bCanSeeTrajectory = false;

	WidgetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetRoot"));
	WidgetRoot->SetupAttachment(RootComponent);

	Widget_3D = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget_3D"));
	Widget_3D->SetupAttachment(WidgetRoot);
	Widget_3D->SetWidgetSpace(EWidgetSpace::World);
	Widget_3D->SetDrawSize(FVector2D(1000.0f, 200.0f));
	Widget_3D->SetRelativeLocation(FVector(200.0f, 0.0f, -50.0f));
	Widget_3D->SetRelativeRotation(FRotator(15.0f, 180.0f, 0.0f));
	Widget_3D->SetRelativeScale3D(FVector(0.25f));
	Widget_3D->bOnlyOwnerSee = true;
	
	Ready_3D = CreateDefaultSubobject<UWidgetComponent>(TEXT("Ready_3D"));
	Ready_3D->SetupAttachment(WidgetRoot);
	Ready_3D->SetWidgetSpace(EWidgetSpace::World);
	Ready_3D->bOnlyOwnerSee = true;
	Ready_3D->SetRelativeLocation(FVector(200.0f, 100.0f, 100.0f));
	Ready_3D->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	Ready_3D->SetRelativeScale3D(FVector(0.25f));
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*Boss UMG*/
	CreateUI();

	/*Show Mouse Cursor*/
	
	//if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()) // when VR Play
	//{
	//	bUseControllerRotationYaw = true;
	//}
	//else // When PC Play
	//{
	//	
	//}

	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc && pc->IsLocalPlayerController())
	{
		pc->bShowMouseCursor = true;
		pc->SetInputMode(FInputModeGameAndUI());
	}


	if (HasAuthority())
	{
		//Initialize Spawn Cooldown.
		for (int i = 0; i < ProjectileClasses.Num(); i++)
		{
			SpawnCooldown.Add(0.0f);
			MaxSpawnCooldown.Add(ProjectileClasses[i].GetDefaultObject()->ProjectileInfo.Cooldown);
		}
	}

}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//DrawProjectileTrajectory();

	for (int i = 0; i < SpawnCooldown.Num(); i++)
	{
		SpawnCooldown[i] = FMath::Clamp<float>(SpawnCooldown[i] + DeltaTime, 0.0f, MaxSpawnCooldown[i]);
	}

	//if Client Needed Sync to Server.
	if (!HasAuthority() && IsLocallyControlled())
	{
		SyncHandLocRot(VR_Left->GetRelativeLocation(), VR_Left->GetRelativeRotation(), VR_Right->GetRelativeLocation(), VR_Right->GetRelativeRotation());
		
		/*
		SyncMeshRot(FRotator(0.0f, Camera->GetRelativeRotation().Yaw, 0.0f));

		*/
		//Widget Root Needed Rotate with Camera
		SyncUMGRot(FRotator(0.0f, Camera->GetRelativeRotation().Yaw, 0.0f));
	}
	else if (HasAuthority() && IsLocallyControlled())
	{
		WidgetRoot->SetRelativeRotation(FRotator(0.0f, Camera->GetRelativeRotation().Yaw, 0.0f));
	}


	//PhysicsHandle Needed Update Target Location Per Frame.
	if (PhysicsHandle)
	{
		//Update Hold Location at Left.
		PhysicsHandle->SetTargetLocation(VR_Left->GetComponentLocation());
		
	}

	DrawTrajectoryLine();

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABossCharacter::HandAction);
	PlayerInputComponent->BindAction("OculusLeft", IE_Pressed, this, &ABossCharacter::LeftHandAction);
	PlayerInputComponent->BindAction("OculusRight", IE_Pressed, this, &ABossCharacter::RightHandPress);
	PlayerInputComponent->BindAction("OculusRight", IE_Released, this, &ABossCharacter::RightHandRelease);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABossCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABossCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABossCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABossCharacter::LookUp);

}

//void ABossCharacter::DrawProjectileTrajectory()
//{
//
//	//Adjust Location to ActorLocation & LaunchVelocity... will equal to Projectile Speed I Think?
//	TrajectoryParams.StartLocation = GetActorLocation();
//	TrajectoryParams.LaunchVelocity = UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 3000.0f;
//	UGameplayStatics::PredictProjectilePath(GetWorld(), TrajectoryParams, Trajectoryresult);
//
//}

//void ABossCharacter::FireToSpawn()
//{
//	//Fire Monster Spawn Projectile if Valid.
//	if (MonsterSpawnProjectileClass)
//	{
//		FTransform transform;
//		transform.SetLocation(GetActorLocation() + GetActorForwardVector() * 100.0f);
//		transform.SetRotation(GetControlRotation().Quaternion());
//		GetWorld()->SpawnActor<AMonsterSpawnProjectile>(MonsterSpawnProjectileClass, transform);
//	}
//}

void ABossCharacter::MoveForward(float Value)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);
	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);
	FVector temp = UKismetMathLibrary::GetForwardVector(controlRot);

	AddMovementInput(temp, Value);
}

void ABossCharacter::MoveRight(float Value)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);
	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);
	FVector temp = UKismetMathLibrary::GetRightVector(controlRot);

	AddMovementInput(temp, Value);
}

void ABossCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABossCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABossCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABossCharacter, bIsGrabbed);
		DOREPLIFETIME(ABossCharacter, SpawnCooldown);
		DOREPLIFETIME(ABossCharacter, MaxSpawnCooldown);
}

void ABossCharacter::HoldSpawnProjectile_Implementation(ABossProjectileBase* ProjectileObject)
{
	if (PhysicsHandle && ProjectileObject)
	{
		PhysicsHandle->GrabComponentAtLocation(ProjectileObject->Sphere, NAME_None, ProjectileObject->GetActorLocation());
		UE_LOG(LogClass, Warning, TEXT("Server Hold OK"));
		bIsGrabbed = true;
		VR_LeftHand->HandState = EHandState::Grab;
	}
	else if(!ProjectileObject)
	{
		UE_LOG(LogClass, Warning, TEXT("Hold Failed. object is nullptr"));
	}
	else if (!PhysicsHandle)
	{
		UE_LOG(LogClass, Warning, TEXT("Hold Failed. PhysicsHandle is nullptr"));
	}
}

void ABossCharacter::HandAction()
{
	UE_LOG(LogClass, Warning, TEXT("%d"), IsValid(PhysicsHandle->GrabbedComponent));

	//when hold something. Release Grab.
	if(bIsGrabbed)//if (PhysicsHandle->GrabbedComponent)
	{
		ReleaseHold();
	}
	else //when hold nothing.
	{
		UE_LOG(LogClass, Warning, TEXT("Nothing Hold"));

		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			int32 sizeX, sizeY;
			FVector crosshairLocation, crosshairDirection; // this is world location & direction.
			playerController->GetViewportSize(sizeX, sizeY);
			playerController->DeprojectScreenPositionToWorld(
				sizeX / 2, sizeY / 2, crosshairLocation, crosshairDirection);

			FVector cameraLocation;
			FRotator cameraRotation;
			playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);


			FVector traceStart = cameraLocation;
			FVector traceEnd = traceStart + crosshairDirection * 100000.0f;

			SearchHold(traceStart, traceEnd);

		}
	}
}

void ABossCharacter::LeftHandAction()
{
	//when hold something. Release Grab.
	if (bIsGrabbed)//if (PhysicsHandle->GrabbedComponent)
	{
		ReleaseHold();
	}
	else //when hold nothing.
	{

		// I Don't Thick It Will Work Correctly ..?
		if (VR_Left)
		{
			FVector traceStart = VR_Left->GetComponentLocation();
			FVector traceEnd = traceStart + VR_Left->GetForwardVector() * 5000.0f;

			SearchHold(traceStart, traceEnd);
		}

	}
}

void ABossCharacter::RightHandPress()
{
	//What Will Do?
	bCanSeeTrajectory = true;

}
void ABossCharacter::RightHandRelease()
{
	//What Will Do?
	bCanSeeTrajectory = false;
	TrajectoryLineTeleport();

}

void ABossCharacter::CreateUI()
{
	AUI_PC* PC = Cast<AUI_PC>(GetController());
	if (PC && PC->IsMasterLobby)
	{
		SetReady3DVisibility(true);
		SetWidget3DVisibility(false);
	}
	else
	{
		SetReady3DVisibility(false);
		SetWidget3DVisibility(true);
	}
}

void ABossCharacter::SearchHold_Implementation(FVector TraceStart, FVector TraceEnd)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;

	//ObjectTypeQuery7 == Projectile(custom)
	objects.Add(EObjectTypeQuery::ObjectTypeQuery7);

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

		EDrawDebugTrace::None, 
		//EDrawDebugTrace::ForDuration,

		hit,
		true,
		FLinearColor::Red,
		FLinearColor::Blue,
		2.0f
	);

	if (result)
	{
		//check hit result actor is Projectile or not.
		ABossProjectileBase * projectile = Cast<ABossProjectileBase>(hit.GetActor());
		if (projectile)
		{
			HoldSpawnProjectile(projectile);
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("You Can't Hold this."));
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("failed."));
	}
}

void ABossCharacter::ReleaseHold_Implementation()
{

	ABossProjectileBase* projectile = Cast<ABossProjectileBase>(PhysicsHandle->GrabbedComponent->GetOwner());
	if (projectile) //set Activate value to Spawn monsters. See AMonsterSpawnPRojectile's Onhit.
	{
		projectile->bActivated = true;
		projectile->OnRep_bActivated();// Server Call
	}
	PhysicsHandle->ReleaseComponent();
	UE_LOG(LogClass, Warning, TEXT("Released"));
	bIsGrabbed = false;
	VR_LeftHand->HandState = EHandState::Open;
}

void ABossCharacter::SpawnProjectile_Implementation(int32 Index)
{
	FTransform transform;
	transform.SetLocation(VR_Left->GetComponentLocation());

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//params.Owner = this;

	ABossProjectileBase* projectile = GetWorld()->SpawnActor<ABossProjectileBase>(ProjectileClasses[Index], transform, params);
	if (projectile)
	{
		//UE_LOG(LogClass, Warning, TEXT("Spawn by UI is Success."));
		HoldSpawnProjectile(projectile);
		SpawnCooldown[Index] = 0.0f;
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Projectile Spawn by UI is Failed"));
	}
}

void ABossCharacter::DrawTrajectoryLine()
{
	if (!MeshForSpline)
	{
		UE_LOG(LogClass, Warning, TEXT("You Must Apply Mesh for Spline."));
		return;
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	for (int32 i = 0; i < SplineMeshArray.Num(); i++)
	{
		SplineMeshArray[i]->UnregisterComponent();
		SplineMeshArray[i]->DestroyComponent();
	}
	SplineMeshArray.Reset();

	if (!bCanSeeTrajectory)
	{
		return;
	}

	FPredictProjectilePathParams predict;
	FPredictProjectilePathResult result;

	predict.StartLocation = VR_Right->GetComponentLocation() + VR_Right->GetForwardVector()* 20.0f;
	predict.LaunchVelocity = VR_Right->GetForwardVector() * 750.0f + VR_Right->GetUpVector()*250.0f;

	predict.bTraceWithCollision = true;
	predict.ProjectileRadius = 5.0f;
	predict.MaxSimTime = 2.0f;
	predict.bTraceWithChannel = true;
	predict.TraceChannel = ECollisionChannel::ECC_Visibility;
	predict.SimFrequency = 10.0f;
	predict.OverrideGravityZ = -500.0f;
	//predict.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	//predict.DrawDebugTime = 2.0f;
	bool bCanTeleport = UGameplayStatics::Blueprint_PredictProjectilePath_Advanced(GetWorld(), predict, result);

	TArray<FPredictProjectilePathPointData> points =
		result.PathData;

	TArray<FVector> splineLocations;

	for (int32 i = 0; i < points.Num(); i++)
	{
		splineLocations.Add(points[i].Location);
	}
	
	Spline->SetSplinePoints(splineLocations, ESplineCoordinateSpace::World);

	for (int32 i = 0; i<Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		Spline->SetSplinePointType(i, ESplinePointType::CurveClamped);
	}

	for (int32 i = 1;i< Spline->GetNumberOfSplinePoints() - 2; i++)
	{
		USplineMeshComponent* splineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		splineMeshComp->SetupAttachment(RootComponent);
		splineMeshComp->SetMobility(EComponentMobility::Movable);
		splineMeshComp->SetStaticMesh(MeshForSpline);
		
		if (!bCanTeleport) // if Can't Teleport, Line Color will be Set to red.
		{
			UMaterialInstanceDynamic* dynamicMaterial = splineMeshComp->CreateDynamicMaterialInstance(0);
			if (dynamicMaterial)
			{
				dynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor::Red);
				splineMeshComp->SetMaterial(0, dynamicMaterial);
			}
		}
		
		splineMeshComp->RegisterComponent();

		SplineMeshArray.Add(splineMeshComp);
		
		FVector startPos;
		FVector startTangent;
		Spline->GetLocationAndTangentAtSplinePoint(i, startPos, startTangent, ESplineCoordinateSpace::Local);

		FVector endPos;
		FVector endTangent;
		Spline->GetLocationAndTangentAtSplinePoint(i+1, endPos, endTangent, ESplineCoordinateSpace::Local);
		
		//startPos += FVector(0.0f, 0.0f, 5.0f);
		//endPos += FVector(0.0f, 0.0f, 5.0f);

		splineMeshComp->SetStartAndEnd(startPos,startTangent,endPos,endTangent);
		//UE_LOG(LogClass, Warning, TEXT(" start Tangent :%s || end tangent : %s"),*startTangent.ToString(), *endTangent.ToString());
	}

	if (bCanTeleport)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PositionNiagara, result.HitResult.Location);
		//UE_LOG(LogClass, Warning, TEXT("Niagara Success."));
	}


	//UE_LOG(LogClass, Warning, TEXT("Draw Success?"));
}

void ABossCharacter::TrajectoryLineTeleport_Implementation()
{

	FPredictProjectilePathParams predict;
	FPredictProjectilePathResult result;

	predict.StartLocation = VR_Right->GetComponentLocation() + VR_Right->GetForwardVector()* 20.0f;
	predict.LaunchVelocity = VR_Right->GetForwardVector() * 750.0f + VR_Right->GetUpVector()*250.0f;

	predict.bTraceWithCollision = true;
	predict.ProjectileRadius = 5.0f;
	predict.MaxSimTime = 2.0f;
	predict.bTraceWithChannel = true;
	predict.TraceChannel = ECollisionChannel::ECC_Visibility;
	predict.SimFrequency = 10.0f;
	predict.OverrideGravityZ = -500.0f;
	//predict.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	//predict.DrawDebugTime = 2.0f;
	bool bCanTeleport = UGameplayStatics::Blueprint_PredictProjectilePath_Advanced(GetWorld(), predict, result);


	if (bCanTeleport)
	{
		TeleportTo(result.HitResult.Location, GetActorRotation());
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT(" You Can't Teleport."));
	}
}

void ABossCharacter::SetWidget3DVisibility(bool Value)
{
	if (Widget_3D)
	{
		Widget_3D->SetVisibility(Value);
		Widget_3D->SetHiddenInGame(!Value);
	}
}

void ABossCharacter::SetReady3DVisibility(bool Value)
{
	if (Ready_3D)
	{
		Ready_3D->SetVisibility(Value);
		Ready_3D->SetHiddenInGame(!Value);
	}
}

void ABossCharacter::SyncHandLocRot_Implementation(FVector L_Loc, FRotator L_Rot, FVector R_Loc, FRotator R_Rot)
{
		VR_Left->SetRelativeLocationAndRotation(L_Loc, L_Rot);
		VR_Right->SetRelativeLocationAndRotation(R_Loc, R_Rot);
}

void ABossCharacter::SyncMeshRot_Implementation(FRotator MeshRot)
{
	GetMesh()->SetRelativeRotation(MeshRot);
}

void ABossCharacter::SyncUMGRot_Implementation(FRotator Rot)
{
	WidgetRoot->SetRelativeRotation(Rot);
}
