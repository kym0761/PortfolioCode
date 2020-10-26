// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPoint.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"

#if WITH_EDITORONLY_DATA

AItemPoint::AItemPoint()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> icontexture(TEXT("Texture2D'/Game/Items/Thumb/Medipack.Medipack'"));

	if (icontexture.Succeeded())
	{
		GetSpriteComponent()->SetSprite(icontexture.Object);
	}

	//if (NewIcon)
	//	GetSpriteComponent()->SetSprite(NewIcon);

}
#endif

void AItemPoint::BeginPlay()
{
	Super::BeginPlay();


}
