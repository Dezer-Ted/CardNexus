// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Grid/GridCell.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateGrid();
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	for(const auto& col : m_Grid)
	{
		for(const auto& cell : col)
		{
			cell->SetChildActorClass(m_CellBP);
		}
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::CreateGrid()
{
	m_Grid.SetNum(m_GridLength);
	for(auto& cols : m_Grid)
	{
		cols.SetNum(m_GridWidth);
	}
	FVector posOffset{GetActorLocation()};
	for(int32 i = 0; i < m_GridLength; ++i)
	{
		for(int32 j = 0; j < m_GridWidth; ++j)
		{
			UChildActorComponent* pCell{};
			pCell = CreateDefaultSubobject<UChildActorComponent>(FName{TEXT("Cell ") + FString::FromInt(i * m_GridLength + j)});
			pCell->SetupAttachment(RootComponent);
			pCell->SetRelativeLocation(posOffset);
			m_Grid[i][j] = pCell;
			posOffset.X += m_CellWidth;
		}
		posOffset.Y += m_CellWidth;
		posOffset.X = GetActorLocation().X;
	}


}
