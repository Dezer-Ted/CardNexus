// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/EnemyUnit.h"

#include "Grid.h"
#include "PlayerUnit.h"
#include "Kismet/GameplayStatics.h"

void AEnemyUnit::StartTurn()
{
	Super::StartTurn();
	FTimerHandle timerHand;
	if(FindTargetInRange())
	{
		AttackTarget();
	}
	else
	{
		WalkToTarget();
	}
}

void AEnemyUnit::EndTurn()
{
	Super::EndTurn();
	UE_LOG(LogTemp, Warning, TEXT("EndedTurn"));
}

void AEnemyUnit::BeginPlay()
{
	Super::BeginPlay();
	m_pTarget = Cast<APlayerUnit>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerUnit::StaticClass()));
}

void AEnemyUnit::ReachedDestination()
{
	if(FindTargetInRange())
		AttackTarget();
	else
	{
		EndTurn();
	}
}

void AEnemyUnit::WalkToTarget()
{
	FCellCoord targetCoord{m_pTarget->GetGridPosition()};
	UE_LOG(LogTemp, Warning, TEXT("TargetCoords: X%i,Y%i"), targetCoord.X, targetCoord.Y);
	SetPath(AGrid::FindPath(GetGridPosition(), targetCoord));
}

bool AEnemyUnit::FindTargetInRange()
{
	for(const auto& neighbor : AGrid::GetCellAtIndex(GetGridPosition())->m_NeighborMap)
	{
		if(!neighbor.Value)
			continue;
		if(neighbor.Value->m_CurrentUnit != m_pTarget)
			continue;
		return true;
	}
	return false;
}

void AEnemyUnit::AttackTarget()
{
	m_pTarget->AddHitPoints(-m_Damage);
	EndTurn();
}
