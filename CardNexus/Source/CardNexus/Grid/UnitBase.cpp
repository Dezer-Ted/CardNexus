// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/UnitBase.h"

#include "Grid.h"
#include "GridCell.h"
#include "CardNexus/Combat/CombatGM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitBase::AUnitBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
	m_HitPoints = m_MaxHitPoints;
	StartTurn();
}

void AUnitBase::FollowPath(float dt)
{
	if(!m_Target)
	{
		if(m_Path.IsEmpty())
		{
			m_MoveToPath = false;
			return;
		}
		m_Target = m_Path[0];
		m_Path.RemoveAt(0);
		m_TargetPos = m_Target->GetActorLocation();
		m_StartPos = GetActorLocation();
	}

	m_TimePassed += dt;
	float normalizedLerpAlpha{FMath::Clamp(FMath::GetRangePct(0.f, m_TimePerSegment, m_TimePassed), 0, 1)};

	auto lerpedPos{FMath::Lerp(m_StartPos, m_TargetPos, normalizedLerpAlpha)};
	SetActorLocation(lerpedPos);
	if(FMath::IsNearlyEqual(normalizedLerpAlpha, 1.f))
	{
		m_TimePassed = 0;
		m_Target = nullptr;
	}
}

void AUnitBase::EndTurn() const
{
	m_pCombatGM->AdvanceInitiative();
}

int32 AUnitBase::GetHitPoints() const
{
	return m_HitPoints;
}

void AUnitBase::AddHitPoints(int32 hpDelta)
{
	m_HitPoints += hpDelta;
	if(m_HitPoints >= m_MaxHitPoints)
		m_HitPoints = m_MaxHitPoints;
	else if(m_HitPoints <= 0)
	{
		DeathEvent.Broadcast();
	}

}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(m_MoveToPath)
	{
		FollowPath(DeltaTime);
	}
}

void AUnitBase::SetPath(TArray<AGridCell*> path)
{
	if(m_CurrentMovementSpeed == 0)
		return;
	m_Path = path;
	if(m_Path.IsEmpty())
		return;
	Algo::Reverse(m_Path);
	if(m_Path.Num() >= m_CurrentMovementSpeed)
		m_Path.SetNum(m_CurrentMovementSpeed, EAllowShrinking::Yes);
	m_CurrentMovementSpeed -= m_Path.Num();
	m_MoveToPath = true;
	m_GridPos = m_Path.Top()->m_CellCord;
	m_Path.Top()->m_CurrentUnit = this;
}

FCellCoord AUnitBase::GetGridPosition() const
{
	return m_GridPos;
}

void AUnitBase::SetGridPosition(FCellCoord coord)
{
	m_GridPos = coord;
}

void AUnitBase::StartTurn()
{
	m_CurrentMovementSpeed = m_MovementSpeed;
}
