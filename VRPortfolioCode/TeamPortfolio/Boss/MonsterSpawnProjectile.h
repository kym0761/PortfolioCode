// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BossProjectileBase.h"
#include "MonsterSpawnProjectile.generated.h"

UCLASS()
class TEAMPORTFOLIO_API AMonsterSpawnProjectile : public ABossProjectileBase
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AMonsterSpawnProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;


	//Override for Spawn Monster
	virtual void StartFunction_Implementation(const FHitResult& Hit) override;

};