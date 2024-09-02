// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "CardNexus/Cards/Card.h"
#include "CardNexus/Cards/CardEffectLibrary.h"
#include "CardNexus/Cards/PlayerHand.h"
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
	m_pPlayerHand = Cast<APlayerHand>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerHand::StaticClass()));
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
					m_pCurrentlyResolvingCard->DisableProjection();
					m_IsOrientationMode = false;
					m_pCurrentlyResolvingCard = nullptr;
					return;
				}
				auto card{Cast<ACard>(hitResult.GetActor())};
				if(card)
				{
					m_pPlayerHand->ConstructHand();
					card->ActivateEffect();
				}

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

void ACombatPlayerController::ProjectRange()
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
				m_pCurrentlyResolvingCard->ProjectEffect(hitResult.ImpactPoint);
			}
		}
	}
}

void ACombatPlayerController::ProjectPath()
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
				auto destinationCell = Cast<AGridCell>(hitResult.GetActor());
				if(destinationCell)
				{
					if(m_PathDestination == destinationCell->m_CellCord)
						return;
					DisablePathProjection();
					m_PathDestination = destinationCell->m_CellCord;
					m_HighlightedPath = AGrid::FindPath(m_pPlayer->GetGridPosition(), destinationCell->m_CellCord);
					for(int i = 0; i < m_HighlightedPath.Num(); ++i)
					{
						if(m_HighlightedPath.Num() - i <= m_pPlayer->m_CurrentMovementSpeed)
						{
							m_HighlightedPath[i]->EnableHighlight(ETileHighLightingMode::PathInRange);
						}
						else
						{
							m_HighlightedPath[i]->EnableHighlight(ETileHighLightingMode::PathOutOfRange);
						}
					}
				}
				else
				{
					DisablePathProjection();
				}
				auto card{Cast<ACard>(hitResult.GetActor())};
				if(card)
				{
					if(m_pHighlightedCard == card)
						return;
					m_pPlayerHand->ConstructHand();
					m_pHighlightedCard = card;
					FVector splitOffSet{card->GetActorLocation()};
					FVector localPosition{m_pPlayerHand->GetTransform().InverseTransformPosition(splitOffSet)};
					localPosition.Z += 200;
					localPosition.X = 0;
					m_pHighlightedCard->SetActorRelativeLocation(localPosition);
					m_pHighlightedCard->SetActorRelativeRotation({0, 0, 0});
				}
				else
				{
					m_pPlayerHand->ConstructHand();
				}
			}
		}
	}
}


void ACombatPlayerController::DisablePathProjection()
{
	for(const auto& cell : m_HighlightedPath)
	{
		cell->EnableHighlight(ETileHighLightingMode::Base);
	}
}

void ACombatPlayerController::StartOrientation(UCardEffectLibrary* card)
{
	m_IsOrientationMode = true;
	m_pCurrentlyResolvingCard = card;
	DisablePathProjection();
}

void ACombatPlayerController::AddUnitsToInitList(const TArray<UInitViewEntry*>& units)
{
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
	if(!m_pPlayer)
		return;
	if(!m_pPlayer->m_IsTurnPlayer)
	{
		DisablePathProjection();
		return;
	}
	if(m_IsOrientationMode)
	{
		ProjectRange();
	}
	else
	{
		ProjectPath();
	}
	if(WasInputKeyJustReleased(EKeys::LeftMouseButton))
	{
		DetectHit();
	}
	if(WasInputKeyJustReleased(EKeys::RightMouseButton))
	{
		if(m_IsOrientationMode)
		{
			m_IsOrientationMode = false;
			m_pCurrentlyResolvingCard->DisableProjection();
			m_pCurrentlyResolvingCard = nullptr;
		}
	}
}
