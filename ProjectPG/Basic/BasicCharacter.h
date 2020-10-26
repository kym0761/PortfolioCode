// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWeaponComponent;
class AMasterItem;
UCLASS()
class PROJECTPG_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWeaponComponent* Weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SprintSpeed = 600.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void Sprint();
	void Unsprint();


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "status")
		uint64 bIsFire : 1;

	UFUNCTION(Server, Reliable)
	void C2S_SetFire(bool State);
	void C2S_SetFire_Implementation(bool State);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "status")
		uint64 bIsSprint : 1;

	UFUNCTION(Server, Reliable)
		void C2S_SetSprint(bool State);
	void C2S_SetSprint_Implementation(bool State);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "status")
		uint64 bIsIronsight : 1;

	UFUNCTION(Server, Reliable)
		void C2S_SetIronsight(bool State);
	void C2S_SetIronsight_Implementation(bool State);

	void StartFire();
	void EndFire();

	void OnFire();

	void StartIronsight();
	void EndIronsight();

	void StartCrouch(); // toggle crouch

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_CurrentHP", Category = "HP")
	float CurrentHP;

	UFUNCTION()
	void OnRep_CurrentHP();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "HP")
	float MaxHP;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* HitReactMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	FVector NormalSpringArmPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	FVector CrouchSpringArmPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LineTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;


	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	USoundBase* WeaponSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* ReloadMontage;

	void Reload();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Decal")
		UMaterialInstance* NormalDecal;


	void StartLeftLean();
	void EndLeftLean();


	UFUNCTION(Server, Reliable)
		void C2S_SetLeftLean(bool State);
	void C2S_SetLeftLean_Implementation(bool State);

	void StartRightLean();
	void EndRightLean();

	UFUNCTION(Server, Reliable)
		void C2S_SetRightLean(bool State);
	void C2S_SetRightLean_Implementation(bool State);


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "status")
	uint8 bLeftLean : 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "status")
	uint8 bRightLean : 1;

	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UCameraShake> WorldCameraShake;


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void C2S_ProcessFire(FVector TraceStart, FVector TraceEnd);
	void C2S_ProcessFire_Implementation(FVector TraceStart, FVector TraceEnd);

	//---
	UFUNCTION(NetMulticast, UnReliable)
	void S2A_SpawnMuzzleAndFlash();
	void S2A_SpawnMuzzleAndFlash_Implementation();


	UFUNCTION(NetMulticast, UnReliable)
	void S2A_SpawnHitEffectAndDecal(FHitResult OutHit);
	void S2A_SpawnHitEffectAndDecal_Implementation(FHitResult OutHit);

	UFUNCTION(NetMulticast, UnReliable)
		void S2A_PlayHitMontage(int32 AnimNum);
	void S2A_PlayHitMontage_Implementation(int32 AnimNum);

	UFUNCTION(NetMulticast, UnReliable)
		void S2A_PlayDeathMontage(int32 AnimNum);
	void S2A_PlayDeathMontage_Implementation(int32 AnimNum);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "status")
		uint8 bIsReloaded : 1;

	UFUNCTION(Server, Reliable)
		void C2S_SetReload(bool ReloadState);
	void C2S_SetReload_Implementation(bool ReloadState);


	TArray<AMasterItem*> PickItemList;

	void AddPickItem(AMasterItem* AddItem);
	void RemovePickItem(AMasterItem* RemoveItem);

	void Pickup();


	UFUNCTION(Server, Reliable)
	void C2S_CheckPickupItem(AMasterItem* PickupItem);
	void C2S_CheckPickupItem_Implementation(AMasterItem* PickupItem);

	UFUNCTION(Client, Reliable)
	void S2C_InsertItem(AMasterItem* PickupItem);
	void S2C_InsertItem_Implementation(AMasterItem* PickupItem);

	TArray<AMasterItem*> Inventory;

	void AddToInventory(AMasterItem* Item);
	void RemoveFromInventory(AMasterItem* Item);

};
