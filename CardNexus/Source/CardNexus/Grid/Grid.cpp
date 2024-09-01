// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/Grid.h"

#include <list>

#include "UnitBase.h"
#include "Components/ChildActorComponent.h"
#include "Containers/List.h"

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

}

void AGrid::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	for(int i = 0; i < m_Grid.Num(); ++i)
	{
		for(int j = 0; j < m_Grid[i].Num(); ++j)
		{
			m_Grid[i][j]->SetChildActorClass(m_CellBP);
			Cast<AGridCell>(m_Grid[i][j]->GetChildActor())->m_CellCord = FCellCoord{i, j};

		}
	}
	for(int i = 0; i < m_Grid.Num(); ++i)
	{
		for(int j = 0; j < m_Grid[i].Num(); ++j)
		{
			SetupNeighbors(Cast<AGridCell>(m_Grid[i][j]->GetChildActor()));
		}
	}
	for(int i = 0; i < m_Grid.Num(); ++i)
	{
		for(int j = 0; j < m_Grid[i].Num(); ++j)
		{
			if(j == 0 || i == 0 || j == m_Grid[i].Num() - 1 || i >= m_Grid.Num() - 1 ||
				(i > 5 && j < 4) || (j >= m_Grid[i].Num() - 2 && i < 7) || (i < 4 && j > m_Grid[0].Num() - 5) || (j < 4 && i > 4) || (i > m_Grid.Num() -
					3 && j < 5)
			)
			{
				if(i == 3 && j == 0)
					continue;
				m_Grid[i][j]->SetHiddenInGame(true);
			}

		}
	}
	m_Grid[6][m_Grid[0].Num() - 3]->SetHiddenInGame(true);
	m_Grid[6][m_Grid[0].Num() - 8]->SetHiddenInGame(true);
	m_Grid[6][m_Grid[0].Num() - 7]->SetHiddenInGame(true);
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AGridCell* AGrid::GetCellAtIndex(FCellCoord coords)
{
	return Cast<AGridCell>(m_Grid[coords.X][coords.Y]->GetChildActor());
}


void AGrid::SetupNeighbors(AGridCell* pCell)
{
	FCellCoord coord = pCell->m_CellCord;
	{
		coord.X -= 1;
		auto [valid,pTarget] = ValidateCell(coord);
		if(valid)
		{
			pCell->m_NeighborMap.Add(EGridDirections::WEST, pTarget);
		}
		else
		{
			pCell->m_NeighborMap.Add(EGridDirections::WEST, nullptr);
		}
	}
	{
		coord = pCell->m_CellCord;
		coord.X += 1;
		auto [valid,pTarget] = ValidateCell(coord);
		if(valid)
		{
			pCell->m_NeighborMap.Add(EGridDirections::EAST, pTarget);
		}
		else
		{
			pCell->m_NeighborMap.Add(EGridDirections::EAST, nullptr);
		}
	}
	{
		coord = pCell->m_CellCord;
		coord.Y += 1;
		auto [valid,pTarget] = ValidateCell(coord);
		if(valid)
		{
			pCell->m_NeighborMap.Add(EGridDirections::NORTH, pTarget);
		}
		else
		{
			pCell->m_NeighborMap.Add(EGridDirections::NORTH, nullptr);
		}
	}
	{
		coord = pCell->m_CellCord;
		coord.Y -= 1;
		auto [valid,pTarget] = ValidateCell(coord);
		if(valid)
		{
			pCell->m_NeighborMap.Add(EGridDirections::SOUTH, pTarget);
		}
		else
		{
			pCell->m_NeighborMap.Add(EGridDirections::SOUTH, nullptr);
		}
	}

}

std::pair<bool, AGridCell*> AGrid::ValidateCell(const FCellCoord& coords)
{
	if(coords.X < 0 || coords.Y < 0)
		return std::make_pair<bool, AGridCell*>(false, nullptr);
	if(coords.X >= m_GridLength || coords.Y >= m_GridWidth)
		return std::make_pair<bool, AGridCell*>(false, nullptr);

	AGridCell* outCell = GetCellAtIndex(coords);
	if(outCell->m_CurrentUnit)
		return std::make_pair<bool, AGridCell*>(false, std::move(outCell));
	return std::make_pair<bool, AGridCell*>(true, std::move(outCell));
}

std::pair<AGridCell*, FAStarHelper> AGrid::CalculateCost(AGridCell* pCurrentCell, std::pair<AGridCell*, FAStarHelper>* pParent, FCellCoord end)
{

	FAStarHelper helper{};
	if(pCurrentCell == pParent->first)
		return std::make_pair(nullptr, FAStarHelper{});


	//helper.gCost = pParent->second.gCost + m_MovementCost;
	helper.hCost = CalculateHeuristicCost(pParent->first->m_CellCord, end);
	helper.pParent = &pParent->second;
	helper.pOwner = pCurrentCell;

	return std::make_pair(pCurrentCell, helper);
}

float AGrid::CalculateHeuristicCost(const FCellCoord& coord1, const FCellCoord& coord2)
{

	FVector2D pos1{static_cast<double>(coord1.X), static_cast<double>(coord1.Y)};
	FVector2D pos2{static_cast<double>(coord2.X), static_cast<double>(coord2.Y)};
	return abs((pos1 - pos2).Length() * 5);

}

TArray<AGridCell*> AGrid::FindPath(const FCellCoord& start, const FCellCoord& end)
{
	using node = std::pair<AGridCell*, FAStarHelper>;
	std::list<node> allNodes;

	std::list<node*> openList;
	std::list<node*> closedList;
	allNodes.push_back(std::make_pair(GetCellAtIndex(start), FAStarHelper{0, CalculateHeuristicCost(start, end), nullptr, GetCellAtIndex(start)}));
	openList.push_back(&allNodes.front());
	node* current{openList.back()};

	while(!openList.empty())
	{
		float lowestPath{10000};
		for(const auto& node : openList)
		{
			float fcost = node->second.gCost + node->second.hCost;
			if(fcost < lowestPath)
			{
				lowestPath = fcost;
				current = node;
			}
		}
		if(current->first == GetCellAtIndex(end))
		{
			break;
		}
		closedList.push_back(current);
		openList.remove(current);
		for(auto neighbor : current->first->m_NeighborMap)
		{
			if(neighbor.Value == nullptr)
				continue;
			if(FindNode(closedList, neighbor.Value) || neighbor.Value->m_CurrentUnit != nullptr)
			{
				if(neighbor.Value->m_CellCord != end)
					continue;
			}
			int32 gCost = current->second.gCost + m_MovementCost;
			auto  pSuccessor = FindNode(openList, neighbor.Value);
			if(!pSuccessor)
			{
				node newNode{
					std::make_pair(neighbor.Value, FAStarHelper{gCost, CalculateHeuristicCost(neighbor.Value->m_CellCord, end), &current->second,
						               neighbor.Value})};
				allNodes.push_back(newNode);
				openList.push_back(&allNodes.back());
			}
			else
			{
				if(gCost < pSuccessor->second.gCost)
				{
					pSuccessor->second.pParent = &current->second;
					pSuccessor->second.gCost = gCost;
				}
			}
		}
	}
	TArray<AGridCell*> path;
	FAStarHelper*      pHelper{&current->second};
	while(pHelper->pParent != nullptr)
	{

		path.Add(pHelper->pOwner);
		pHelper = pHelper->pParent;
	}
	return path;
}

std::pair<AGridCell*, FAStarHelper>* AGrid::FindNode(const std::list<std::pair<AGridCell*, FAStarHelper>*>& list, AGridCell* target)
{
	for(auto elem : list)
	{
		if(elem->first->m_CellCord.X == target->m_CellCord.X && elem->first->m_CellCord.Y == target->m_CellCord.Y)
		{
			return elem;
		}
	}
	return nullptr;
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
			pCell = CreateDefaultSubobject<UChildActorComponent>(FName{TEXT("Cell ") + FString::FromInt(i * m_GridWidth + j)});
			pCell->SetupAttachment(RootComponent);
			pCell->SetRelativeLocation(posOffset);
			m_Grid[i][j] = pCell;
			posOffset.X += m_CellWidth;
		}
		posOffset.Y += m_CellWidth;
		posOffset.X = GetActorLocation().X;
	}
}

bool CompareNodes(const std::pair<AGridCell*, FAStarHelper>& node1, const std::pair<AGridCell*, FAStarHelper>& node2)
{
	return node1.second.gCost + node1.second.hCost < node2.second.gCost + node2.second.hCost;
}

bool FindNode(const std::pair<AGridCell*, FAStarHelper>& element, AGridCell* pCell)
{

	return element.first->m_CellCord.X == pCell->m_CellCord.X && element.first->m_CellCord.Y == pCell->m_CellCord.Y;
}
