// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatGM.h"

#include <string>

#include "CombatPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CardNexus/Cards/PlayerHand.h"
#include "CardNexus/Grid/EnemyUnit.h"
#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Grid/InvisUnit.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "Components/TextBlock.h"
#include "HUD/GameOverScreen.h"
#include "Initiative/InitCard.h"
#include "Initiative/InitiativeList.h"
#include "Initiative/InitViewEntry.h"
#include "Kismet/GameplayStatics.h"

void ACombatGM::AdvanceInitiative()
{
	m_ActiveInit.erase(std::remove(m_ActiveInit.begin(), m_ActiveInit.end(), m_TurnUnit));

	auto maxElem = std::max_element(m_ActiveInit.begin(), m_ActiveInit.end(), [](InitEntry* entry1, InitEntry* entry2)
	{
		return entry1->second < entry2->second;

	});
	if(maxElem == m_ActiveInit.end())
	{
		for(auto& elem : m_Initiative)
		{
			m_ActiveInit.push_back(&elem);
		}
		std::sort(m_ActiveInit.begin(), m_ActiveInit.end(), [](InitEntry* entry1, InitEntry* entry2)
		{
			return entry1->second > entry2->second;
		});
		m_TurnUnit = m_ActiveInit.front();
		++m_TurnCounter;
	}
	else
	{
		m_TurnUnit = *maxElem;
	}
	m_TurnUnit->first->StartTurn();
	UpdateInitHud();
	if(Cast<APlayerUnit>(m_TurnUnit->first))
	{
		m_pPlayerHand->DrawCard();
	}
}

void ACombatGM::OnUnitDeath(AUnitBase* pUnit)
{
	if(Cast<APlayerUnit>(pUnit))
	{
		m_pGameOver = Cast<UGameOverScreen>(CreateWidget(m_pPlayerController, m_pGameOverBP));
		m_pGameOver->gameWon(false);
		m_pGameOver->AddToViewport();
	}

	auto eraseIT = std::remove_if(m_ActiveInit.begin(), m_ActiveInit.end(), [pUnit](InitEntry* entry)
	{
		return entry->first == pUnit;
	});
	if(eraseIT != m_ActiveInit.end())
		m_ActiveInit.erase(eraseIT);
	m_Initiative.remove_if([pUnit](const InitEntry& entry)
	{
		return entry.first == pUnit;
	});
	if(Cast<AEnemyUnit>(pUnit))
	{
		int32 counter{};
		for(const auto& elem : m_Initiative)
		{
			if(Cast<AEnemyUnit>(elem.first))
			{
				++counter;
			}
		}
		if(counter == 0)
		{
			m_pGameOver = Cast<UGameOverScreen>(CreateWidget(m_pPlayerController, m_pGameOverBP));
			m_pGameOver->gameWon(true);
			m_pGameOver->AddToViewport();
		}

	}
	UpdateInitHud();
}


void ACombatGM::BeginPlay()
{
	Super::BeginPlay();
	TArray<FUnitData> units{};
	units.Add(FUnitData{3, 0, ECombatUnitType::PlayerUnit});
	units.Add(FUnitData{11, 5, ECombatUnitType::EnemyUnit});
	units.Add(FUnitData{12, 5, ECombatUnitType::EnemyUnit});
	units.Add(FUnitData{12, 7, ECombatUnitType::EnemyUnit});
	m_pPlayerController = GetWorld()->GetFirstPlayerController();
	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ACombatGM::InitHand, 1.f, false);

	LoadInit(units);
	StartInitiative();
}

void ACombatGM::LoadInit(const TArray<FUnitData>& units)
{

	FActorSpawnParameters spawnParams{};
	for(int i = 0; i < AGrid::m_Grid.Num(); ++i)
	{
		for(int j = 0; j < AGrid::m_Grid[i].Num(); ++j)
		{
			if(j == 0 || i == 0 || j == AGrid::m_Grid[i].Num() - 1 || i >= AGrid::m_Grid.Num() - 1 ||
				(i > 5 && j < 4) || (j >= AGrid::m_Grid[i].Num() - 2 && i < 7) || (i < 4 && j > AGrid::m_Grid[0].Num() - 5) || (j < 4 && i > 4) ||
				(i > AGrid::m_Grid.Num() - 3 && j < 5) || (i == AGrid::m_Grid.Num() - 5 && j == AGrid::m_Grid[0].Num() - 5)
				)
			{
				if(i == 3 && j == 0)
					continue;
				CreateInvisUnitAtIndex(FCellCoord{i, j});
			}
		}
	}
	CreateInvisUnitAtIndex(FCellCoord{6, AGrid::m_Grid[0].Num() - 3});
	CreateInvisUnitAtIndex(FCellCoord{6, AGrid::m_Grid[0].Num() - 8});
	CreateInvisUnitAtIndex(FCellCoord{6, AGrid::m_Grid[0].Num() - 7});

	for(const auto& unit : units)
	{
		auto                cell{AGrid::GetCellAtIndex(unit.m_Coords)};
		TSubclassOf<AActor> unitType;
		switch(unit.m_Type)
		{
		case ECombatUnitType::PlayerUnit:
			{
				unitType = m_PlayerUnitBP;
				auto playerUnit = Cast<APlayerUnit>(
					GetWorld()->SpawnActor<AActor>(unitType, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
				m_Units.Add(playerUnit);
				InitEntry entry{InitEntry{playerUnit, 5}};
				AddToInitiative(entry);
				playerUnit->m_pCombatGM = this;
				playerUnit->m_UnitName = FName{TEXT("Player")};
				playerUnit->SetGridPosition(unit.m_Coords);
				cell->m_CurrentUnit = playerUnit;
				Cast<ACombatPlayerController>(m_pPlayerController)->m_pPlayer = playerUnit;
				break;
			}
		case ECombatUnitType::EnemyUnit:
			{
				unitType = m_EnemyUnitBP;
				auto enemyUnit = Cast<
					AEnemyUnit>(GetWorld()->SpawnActor<AActor>(unitType, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
				m_Units.Add(enemyUnit);
				AddToInitiative(InitEntry{enemyUnit, FMath::RandRange(1, 4)});
				cell->m_CurrentUnit = enemyUnit;
				enemyUnit->SetGridPosition(unit.m_Coords);

				cell->m_CurrentUnit = enemyUnit;
				enemyUnit->m_pCombatGM = this;
				enemyUnit->m_UnitName = FName{TEXT("Enemy ") + FString::FromInt(m_EnemyCount)};
				++m_EnemyCount;
				break;
			}
		}
	}
}

void ACombatGM::CreateInvisUnitAtIndex(const FCellCoord& pos)
{
	FActorSpawnParameters spawnParams{};
	auto                  cell = AGrid::GetCellAtIndex(pos);
	auto                  invisUnit = Cast<AInvisUnit>(
		GetWorld()->SpawnActor<AActor>(m_InvisUnitBP, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
	m_Units.Add(invisUnit);
	cell->m_CurrentUnit = invisUnit;
}

void ACombatGM::AddToInitiative(const InitEntry& entry)
{
	m_Initiative.push_back(entry);

}

void ACombatGM::StartInitiative()
{

	for(auto& elem : m_Initiative)
	{
		m_ActiveInit.push_back(&elem);
	}

	std::sort(m_ActiveInit.begin(), m_ActiveInit.end(), [](InitEntry* entry1, InitEntry* entry2)
	{
		return entry1->second > entry2->second;
	});
	m_TurnUnit = m_ActiveInit.front();
	UE_LOG(LogTemp, Warning, TEXT("TurnUnit: %s"), *m_TurnUnit->first->m_UnitName.ToString());
	m_TurnUnit->first->StartTurn();
	UpdateInitHud();
}

void ACombatGM::UpdateInitHud()
{
	TArray<UInitViewEntry*> cards;
	for(auto card : m_ActiveInit)
	{
		UInitViewEntry* tempCard{NewObject<UInitViewEntry>()};
		tempCard->m_Name = FText::FromString(card->first->m_UnitName.ToString());
		tempCard->m_Init = FText::AsNumber(card->second);
		cards.Add(tempCard);
	}
	for(const auto& card : m_Initiative)
	{
		UInitViewEntry* tempCard{NewObject<UInitViewEntry>()};
		tempCard->m_Name = FText::FromString(card.first->m_UnitName.ToString());
		tempCard->m_Init = FText::AsNumber(card.second);
		cards.Add(tempCard);
	}
	Cast<ACombatPlayerController>(m_pPlayerController)->AddUnitsToInitList(cards);
}

void ACombatGM::InitHand()
{
	m_pPlayerHand = Cast<APlayerHand>(UGameplayStatics::GetActorOfClass(GetWorld(), m_pPlayerHandBP));
	for(int i = 0; i < m_pPlayerHand->m_StartingHandSize; ++i)
	{
		m_pPlayerHand->DrawCard();
	}
}

void ACombatGM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}
