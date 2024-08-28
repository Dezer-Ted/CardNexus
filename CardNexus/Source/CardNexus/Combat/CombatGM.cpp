// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatGM.h"

#include "CombatPlayerController.h"
#include "CardNexus/Grid/EnemyUnit.h"
#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Grid/PlayerUnit.h"

void ACombatGM::AdvanceInitiative()
{
	m_ActiveInit.erase(std::remove(m_ActiveInit.begin(), m_ActiveInit.end(), m_TurnUnit));
	auto maxElem = *std::max_element(m_ActiveInit.begin(), m_ActiveInit.end(), [](InitEntry* entry1, InitEntry* entry2)
	{
		return entry1->second < entry2->second;

	});
	if(maxElem == nullptr)
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
	}
	else
	{
		m_TurnUnit = maxElem;
	}
}

void ACombatGM::BeginPlay()
{
	Super::BeginPlay();
	TArray<FUnitData> units{};
	units.Add(FUnitData{0, 0, EUnitType::PlayerUnit});
	units.Add(FUnitData{5, 5, EUnitType::EnemyUnit});
	m_pPlayerController = GetWorld()->GetFirstPlayerController();
	LoadInit(units);
	StartInitiative();
}

void ACombatGM::LoadInit(const TArray<FUnitData>& units)
{
	for(const auto& unit : units)
	{
		auto                  cell{AGrid::GetCellAtIndex(unit.m_Coords)};
		TSubclassOf<AActor>   unitType;
		FActorSpawnParameters spawnParams{};
		switch(unit.m_Type)
		{
		case EUnitType::PlayerUnit:
			{
				unitType = m_PlayerUnitBP;
				auto playerUnit = Cast<APlayerUnit>(
					GetWorld()->SpawnActor<AActor>(unitType, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
				InitEntry entry{InitEntry{playerUnit, 5}};
				AddToInitiative(entry);
				playerUnit->m_pCombatGM = this;
				Cast<ACombatPlayerController>(m_pPlayerController)->m_pPlayer = playerUnit;
				break;
			}
		case EUnitType::EnemyUnit:
			{
				unitType = m_EnemyUnitBP;
				auto enemyUnit = Cast<
					AEnemyUnit>(GetWorld()->SpawnActor<AActor>(unitType, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
				AddToInitiative(InitEntry{enemyUnit, FMath::RandRange(0, 8)});
				cell->m_CurrentUnit = enemyUnit;
				enemyUnit->SetGridPosition(unit.m_Coords);
				enemyUnit->m_pCombatGM = this;
				break;
			}
		}
	}
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
	m_TurnUnit = &m_Initiative.front();
	m_TurnUnit->first->StartTurn();
}
