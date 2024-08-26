// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatPlayerController.h"

#include "CardNexus/Cards/Card.h"
#include "CardNexus/Grid/Grid.h"

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACombatPlayerController::DetectHit()
{
	FVector2D mousePos{};
	FVector   worldLocation{};
	FVector   worldDirection{};
	if(DeprojectMousePositionToWorld(worldLocation, worldDirection))
	{
		FVector               start{PlayerCameraManager->GetCameraLocation()};
		FVector               end{start + worldDirection * 10000.f};
		FHitResult            hitResult{};
		FCollisionQueryParams collisionQuery{};
		collisionQuery.AddIgnoredActor(GetPawn());

		if(GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionQuery))
		{
			if(hitResult.GetActor())
			{
				auto card{Cast<ACard>(hitResult.GetActor())};
				if(card)
					card->ActivateEffect();

			}
		}

	}


}

void ACombatPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if(WasInputKeyJustReleased(EKeys::LeftMouseButton))
	{
		DetectHit();
	}
	if(WasInputKeyJustReleased(EKeys::RightMouseButton))
	{
		AGrid::GetPathTo(FCellCoord{0, 0}, FCellCoord{10, 5});
	}
}
