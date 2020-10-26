// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal	 = 0 UMETA(Display = "Normal"),
	Battle	 = 1 UMETA(Display = "Battle"),
	Chase	 = 2 UMETA(Display = "Chase"),
	Dead	 = 3 UMETA(Display = "Dead"),

};

UCLASS()
class PROJECTPG_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		EZombieState CurrentState;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float WalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float RunSpeed = 300.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float Attack = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "status")
		float AttackRange = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
		void ProcessSeenPawn(APawn* Pawn);

	UFUNCTION()
		void ProcessHeardPawn(APawn* Pawn, const FVector& Location, float Volume);

	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EZombieState NewState);

	UFUNCTION(BlueprintCallable)
	void SetSpeed();

	void SetSpeed(float Speed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* ZombieBT;

};
