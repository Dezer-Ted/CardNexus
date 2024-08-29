// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/CombatGM.h"

#include <string>

#include "CombatPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CardNexus/Grid/EnemyUnit.h"
#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "Components/TextBlock.h"
#include "Initiative/InitCard.h"
#include "Initiative/InitiativeList.h"
#include "Initiative/InitViewEntry.h"

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
	}
	else
	{
		m_TurnUnit = *maxElem;
	}
	m_TurnUnit->first->StartTurn();
	UpdateInitHud();
}



void ACombatGM::BeginPlay()
{
	Super::BeginPlay();
	TArray<FUnitData> units{};
	units.Add(FUnitData{0, 0, ECombatUnitType::PlayerUnit});
	units.Add(FUnitData{5, 5, ECombatUnitType::EnemyUnit});
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
		case ECombatUnitType::PlayerUnit:
			{
				unitType = m_PlayerUnitBP;
				auto playerUnit = Cast<APlayerUnit>(
					GetWorld()->SpawnActor<AActor>(unitType, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
				InitEntry entry{InitEntry{playerUnit, 5}};
				AddToInitiative(entry);
				playerUnit->m_pCombatGM = this;
				playerUnit->m_UnitName = FName{TEXT("Player")};
				Cast<ACombatPlayerController>(m_pPlayerController)->m_pPlayer = playerUnit;
				break;
			}
		case ECombatUnitType::EnemyUnit:
			{
				unitType = m_EnemyUnitBP;
				auto enemyUnit = Cast<
					AEnemyUnit>(GetWorld()->SpawnActor<AActor>(unitType, cell->GetActorLocation(), FRotator::ZeroRotator, spawnParams));
				AddToInitiative(InitEntry{enemyUnit, FMath::RandRange(0, 8)});
				cell->m_CurrentUnit = enemyUnit;
				enemyUnit->SetGridPosition(unit.m_Coords);
				enemyUnit->m_pCombatGM = this;
				enemyUnit->m_UnitName = FName{TEXT("Enemy ") + FString::FromInt(m_EnemyCount)};
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
