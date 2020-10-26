// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHUD.h"
#include "Engine/Canvas.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"

void ABasicHUD::DrawHUD()
{
	Super::DrawHUD();

	//Draw2DLine(100, 100, 200, 200, FColor::Red);
	//DrawLine(300, 300, 400, 400, FLinearColor::Green, 10.0f);
	if (Crosshair)
	{
		float sizeX = (Canvas->SizeX / 2) - (Crosshair->GetSizeX() / 2);
		float sizeY = (Canvas->SizeY / 2) - (Crosshair->GetSizeY() / 2);

		DrawTextureSimple(Crosshair, sizeX, sizeY);
	}
	//DrawTextureSimple(,);
}

void ABasicHUD::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager loader;

	T_Crosshair.SetPath(TEXT("Texture2D'/Game/UI/crosshair.crosshair'"));
	//Crosshair = loader.LoadSynchronous<UTexture2D>(T_Crosshair); Texture2D'/Game/UI/crosshair.crosshair'
	loader.RequestAsyncLoad(T_Crosshair, FStreamableDelegate::CreateUObject(this, &ABasicHUD::LoadHUDTexture));
	
}

void ABasicHUD::LoadHUDTexture()
{
	Crosshair = Cast<UTexture2D>(T_Crosshair.TryLoad());
}
