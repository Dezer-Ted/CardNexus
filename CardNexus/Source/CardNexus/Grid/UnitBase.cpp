// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/UnitBase.h"

#include "GridCell.h"
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
	m_Path = path;
	if(m_Path.IsEmpty())
		return;
	m_Path.Pop();
	Algo::Reverse(m_Path);
	m_MoveToPath = true;
}
