// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "CardNexus/Cards/Card.h"
#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Combat/Initiative/InitiativeList.h"
#include "CardNexus/Combat/Initiative/InitCard.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACombatPlayerController::DetectHit() const
{
	FVector worldLocation{};
	FVector worldDirection{};
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

				auto cell{Cast<AGridCell>(hitResult.GetActor())};
				if(cell)
				{
					m_pPlayer->SetPath(AGrid::FindPath(m_pPlayer->GetGridPosition(), cell->m_CellCord));
				}
			}
		}
	}
}

void ACombatPlayerController::AddUnitsToInitList(TArray<UInitCard*> units)
{
	for(auto unit : units)
	{
		m_InitList->m_pList->AddItem(unit);
	}
}

void ACombatPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	m_InitList = CreateWidget<UInitiativeList>(GetWorld(), m_InitListBP);
	m_InitList->AddToViewport();
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

	}
}
