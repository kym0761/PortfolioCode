// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/MonsterDataTable.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Normal			=	0	UMETA(Display = "Normal"),
	Battle			=	1	UMETA(Display = "Battle"),
	Chase			=	2	UMETA(Display = "Chase"),
	Death			=	3	UMETA(Display = "Death"),
};

UCLASS()
class TEAMPORTFOLIO_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//S
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Status")
	EMonsterState CurrentState;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void ProcessSeenPawn(APawn* Pawn);

	UFUNCTION()
	void ProcessHeardPawn(APawn* Pawn, const FVector&Location, float Volume);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetCurrentState(EMonsterState NewState);
	void SetCurrentState_Implementation(EMonsterState NewState);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float Speed);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float WalkSpeed = 150.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float RunSpeed = 400.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Status")
	class UBehaviorTree* MonsterBT;

	//S
	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = "OnRep_HPChanged", Category = "State")
	float CurrentHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	EMonsterType MonType;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "UI")
	//class UMonsterHPWidgetBase* HPBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class USceneComponent* Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class UWidgetComponent* HPBar3D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UMG")
	TSubclassOf<class UMonsterHPWidgetBase> HpWidgetClass;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	void DamageProcess();

	UFUNCTION(Server, Reliable)
	void C2S_DamageProcess();
	void C2S_DamageProcess_Implementation();

	UFUNCTION()
	void OnRep_HPChanged();

	UFUNCTION(NetMultiCast, Reliable)
	void S2A_DisableUI(EMonsterState NewState);
	void S2A_DisableUI_Implementation(EMonsterState NewState);
};
