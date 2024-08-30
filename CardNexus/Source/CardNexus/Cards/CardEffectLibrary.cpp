// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffectLibrary.h"

#include "Card.h"
#include "InteractiveToolManager.h"
#include "CardNexus/Grid/Grid.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCardEffectLibrary::UCardEffectLibrary()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffectLibrary::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

EGridDirections UCardEffectLibrary::DetermineDirection(FVector mousePos)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);

	if(FoundActors.Num() > 0)
	{
		APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
		if(player)
		{
			auto      playerPos = player->GetActorLocation();
			FVector2f targetPos;
			targetPos.X = mousePos.X - playerPos.X;
			targetPos.Y = mousePos.Y - playerPos.Y;
			targetPos.Normalize();
			//East
			// Tolerance value for comparison
			float Tolerance = 0.5f;
			UE_LOG(LogTemp, Warning, TEXT("X:%f,Y:%f"), targetPos.X, targetPos.Y);
			if(FMath::Abs(targetPos.X - 1.0f) < Tolerance && FMath::Abs(targetPos.Y) < Tolerance)
			{
				return EGridDirections::NORTH;
			}
			else if(FMath::Abs(targetPos.X + 1.0f) < Tolerance && FMath::Abs(targetPos.Y) < Tolerance)
			{
				return EGridDirections::SOUTH;
			}
			else if(FMath::Abs(targetPos.Y - 1.0f) < Tolerance && FMath::Abs(targetPos.X) < Tolerance)
			{
				return EGridDirections::EAST;
			}
			else if(FMath::Abs(targetPos.Y + 1.0f) < Tolerance && FMath::Abs(targetPos.X) < Tolerance)
			{
				return EGridDirections::WEST;
			}
		}
	}
	return EGridDirections::NORTH;
}

TArray<class AGridCell*> UCardEffectLibrary::GetSurroundingCells(const FCellCoord& pos)
{
	TArray<AGridCell*> affectedCells{};

	AGridCell* playerCell = AGrid::GetCellAtIndex(pos);
	affectedCells.Emplace(playerCell->m_NeighborMap[EGridDirections::NORTH]);
	affectedCells.Emplace(playerCell->m_NeighborMap[EGridDirections::EAST]);
	affectedCells.Emplace(playerCell->m_NeighborMap[EGridDirections::SOUTH]);
	affectedCells.Emplace(playerCell->m_NeighborMap[EGridDirections::WEST]);

	if(affectedCells[0] != nullptr && affectedCells[1] != nullptr)
	{
		affectedCells.Emplace(affectedCells[0]->m_NeighborMap[EGridDirections::EAST]);
	}
	if(affectedCells[1] != nullptr && affectedCells[2] != nullptr)
	{
		affectedCells.Emplace(affectedCells[1]->m_NeighborMap[EGridDirections::SOUTH]);
	}
	if(affectedCells[2] != nullptr && affectedCells[3] != nullptr)
	{
		affectedCells.Emplace(affectedCells[2]->m_NeighborMap[EGridDirections::WEST]);
	}
	if(affectedCells[3] != nullptr && affectedCells[0] != nullptr)
	{
		affectedCells.Emplace(affectedCells[3]->m_NeighborMap[EGridDirections::NORTH]);
	}
	affectedCells.RemoveAll([](AGridCell* cell)
	{
		return cell == nullptr;
	});
	return affectedCells;
}


void UCardEffectLibrary::ApplyDamage(const TArray<AGridCell*>& affectedCells, int32 damage)
{
	for(int i = 0; i < affectedCells.Num(); ++i)
	{
		if(affectedCells[i]->m_CurrentUnit == nullptr)
			continue;
		affectedCells[i]->m_CurrentUnit->AddHitPoints(-damage);
	}
}

APlayerUnit* UCardEffectLibrary::GetPlayer()
{

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);

	if(FoundActors.Num() == 0)
		return nullptr;

	APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
	if(player == nullptr)
		return nullptr;
	return player;
}

AGridCell* UCardEffectLibrary::GetProjectileTarget(const EGridDirections& direction, int32 range, const FCellCoord& startPos)
{
	AGridCell* currentCell{AGrid::GetCellAtIndex(startPos)};
	AGridCell* nextCell;
	for(int i = 0; i < range; i++)
	{
		nextCell = currentCell->m_NeighborMap[direction];
		if(nextCell == nullptr)
		{
			//goes off the grid
			return nullptr;
		}
		if(nextCell->m_CurrentUnit != nullptr)
		{
			//hit player
			return nextCell;
		}
		currentCell = nextCell;
	}
	return currentCell;
}

void UCardEffectLibrary::AddConeNeighbors(EGridDirections direction, TArray<AGridCell*>& effectedCells, AGridCell* currentCell, int maxidx)
{
	for(int i = 0; i < maxidx; i++)
	{
		auto neighbourCell = currentCell->m_NeighborMap[direction];
		if(neighbourCell)
		{
			effectedCells.Add(neighbourCell);
		}
		else
			break;

		currentCell = neighbourCell;
	}
}

TArray<AGridCell*> UCardEffectLibrary::GetConeAffectedCells(const FCellCoord& startPos, int32 range, const EGridDirections& direction)
{
	TArray<AGridCell*> affectedCells;
	AGridCell*         currentCell = AGrid::GetCellAtIndex(startPos);
	AddConeNeighbors(direction, affectedCells, currentCell, range);

	// Get the cells on the sides
	int beforeArrSize = affectedCells.Num();
	for(int i = 1; i < beforeArrSize; i++)
	{
		currentCell = affectedCells[i];

		if(direction == EGridDirections::NORTH || direction == EGridDirections::SOUTH)
		{
			//add neighbors West of cell
			AddConeNeighbors(EGridDirections::WEST, affectedCells, currentCell, i);

			//add neighbors East of cell
			AddConeNeighbors(EGridDirections::EAST, affectedCells, currentCell, i);
		}
		else if(direction == EGridDirections::EAST || direction == EGridDirections::WEST)
		{
			//add neighbors North of cell
			AddConeNeighbors(EGridDirections::NORTH, affectedCells, currentCell, i);

			//add neighbors South of cell
			AddConeNeighbors(EGridDirections::SOUTH, affectedCells, currentCell, i);
		}

	}
	return affectedCells;

}

void UCardEffectLibrary::DisableProjection()
{
	m_IsProjecting = false;
	for(const auto& cell : m_HighlightedCells)
	{
		if(cell != nullptr)
			cell->EnableHighlight(false);
	}
	m_HighlightedCells.SetNum(0);
}

void UCardEffectLibrary::ActivateEffect()
{

}

void UCardEffectLibrary::ProjectEffect(const FVector& pos)
{
}

void UCardEffectLibrary::ResolveEffect(const FVector& direction)
{
	Cast<ACard>(GetOwner())->DiscardThisCard();
}


// Called every frame
void UCardEffectLibrary::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
