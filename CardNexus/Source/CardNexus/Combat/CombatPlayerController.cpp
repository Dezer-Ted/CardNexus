// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "CardNexus/Cards/Card.h"
#include "CardNexus/Cards/CardEffectLibrary.h"
#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Combat/Initiative/InitiativeList.h"
#include "CardNexus/Combat/Initiative/InitCard.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Initiative/EndTurnButton.h"
#include "Kismet/GameplayStatics.h"

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACombatPlayerController::DetectHit()
{
	if(!m_pPlayer->m_IsTurnPlayer)
		return;
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
				if(m_IsOrientationMode)
				{
					auto pos = hitResult.ImpactPoint;
					m_pCurrentlyResolvingCard->ResolveEffect(pos);
					m_IsOrientationMode = false;
					m_pCurrentlyResolvingCard = nullptr;
					return;
				}
				auto card{Cast<ACard>(hitResult.GetActor())};
				if(card)
					card->ActivateEffect();

				auto cell{Cast<AGridCell>(hitResult.GetActor())};
				if(cell)
				{
					if(cell->m_CurrentUnit != nullptr)
						return;
					m_pPlayer->SetPath(AGrid::FindPath(m_pPlayer->GetGridPosition(), cell->m_CellCord));
				}
			}
		}
	}
}

void ACombatPlayerController::StartOrientation(UCardEffectLibrary* card)
{
	m_IsOrientationMode = true;
	m_pCurrentlyResolvingCard = card;
}

void ACombatPlayerController::AddUnitsToInitList(const TArray<UInitViewEntry*>& units)
{
	/*for(const auto& unit : units)
	{
		auto entry{Cast<UInitCard>(CreateWidget(m_InitList->m_pList, m_InitCardBP))};
		m_InitList->m_pList->AddItem(entry);
	}*/
	m_InitList->m_pList->SetListItems(units);
}

void ACombatPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	m_InitList = CreateWidget<UInitiativeList>(GetWorld(), m_InitListBP);
	m_InitList->AddToViewport();
	m_EndTurnButton = CreateWidget<UEndTurnButton>(GetWorld(), m_EndTurnButtonBP);
	m_EndTurnButton->AddToViewport();
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
