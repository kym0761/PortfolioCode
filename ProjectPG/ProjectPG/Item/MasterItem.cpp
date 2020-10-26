// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "ItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "ItemDataTable.h"
#include "Net/UnrealNetwork.h"
#include "../Basic/BasicCharacter.h"
// Sets default values
AMasterItem::AMasterItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetSphereRadius(150.0f);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Sphere);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	

	if (ItemComponent->ItemDataTable)
	{

		if (HasAuthority()) // only host can do
		{
			ItemIndex = FMath::RandRange(1, 6) * 10;
		}

		ItemData = ItemComponent->GetItemData(ItemIndex);

		if (ItemIndex != 0)
		{
			FStreamableManager loader;
			ItemMesh->SetStaticMesh(loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		}
		
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::ProcessBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::ProcessEndOverlap);
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AMasterItem, ItemIndex);
}

void AMasterItem::ProcessBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")) )
	{
		ABasicCharacter* player = Cast<ABasicCharacter>(OtherActor);
		if (player && player->IsLocallyControlled())
		{
			player->AddPickItem(this);
		}
	}
}

void AMasterItem::ProcessEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ABasicCharacter* player = Cast<ABasicCharacter>(OtherActor);
		if (player && player->IsLocallyControlled())
		{
			player->RemovePickItem(this);
		}
	}
}

