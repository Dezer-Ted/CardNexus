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
		UE_LOG(LogTemp, Warning, TEXT("genereatedNEighbors"))
	}
	UE_LOG(LogTemp, Warning, TEXT("genereatedNEighbors"))
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

TArray<AGridCell*> AGrid::GetPathTo(FCellCoord start, FCellCoord end)
{
	/*TArray<AGridCell*>                             path;
	std::list<std::pair<AGridCell*, FAStarHelper>> openList;
	std::list<std::pair<AGridCell*, FAStarHelper>> closedList;
	openList.push_back(std::make_pair<AGridCell*, FAStarHelper>(GetCellAtIndex(start), FAStarHelper{0, 0, nullptr, GetCellAtIndex(start)}));
	AGridCell* pEndCell{GetCellAtIndex(end)};
	while(!openList.empty())
	{
		auto pCurrentCell = Algo::MinElement(openList, &CompareNodes);

		UE_LOG(LogTemp, Warning, TEXT("CurrentCell  X: %d, Y:%d gCost: %d hCost: %f"), pCurrentCell->first->m_CellCord.X,
		       pCurrentCell->first->m_CellCord.Y, pCurrentCell->second.gCost, pCurrentCell->second.hCost);

		if(pCurrentCell->first == pEndCell)
		{
			auto* currentHelper = &pCurrentCell->second;
			path.Add(pCurrentCell->first);

			while(currentHelper->pParent != nullptr)
			{

				currentHelper = currentHelper->pParent;
				if(currentHelper == currentHelper->pParent)
				{
					UE_LOG(LogTemp, Error, TEXT("Error: Loop detected "));
					break;
				}
				if(currentHelper->pOwner == nullptr)
				{
					// Sanity check to avoid accessing a nullptr, which could indicate an issue
					UE_LOG(LogTemp, Error, TEXT("Error: Found a nullptr in the parent chain!"));
					break;
				}
				path.Add(currentHelper->pOwner);
				//UE_LOG(LogTemp, Warning, TEXT("currentCell  X: %d, Y:%d"), currentHelper->pOwner->m_CellCord.X, currentHelper->pOwner->m_CellCord.Y);
			}
			return path;
		}
		closedList.push_back(*pCurrentCell);
		openList.remove(*pCurrentCell);
		/*openList.RemoveAll([pCurrentCell](const std::pair<AGridCell*, FAStarHelper>& elem)
		{
			return elem.first == pCurrentCell->first;
		});#1#
		for(const auto& neighbor : pCurrentCell->first->m_NeighborMap)
		{
			if(!neighbor.Value)
			{

				UE_LOG(LogTemp, Warning, TEXT("SkippedNeighbor"));

				continue;
			}

			auto pNeighborData{CalculateCost(neighbor.Value, pCurrentCell, end)};
			if(!pNeighborData.first)
				continue;
			auto pEntry = Algo::FindByPredicate(closedList, [pNeighborData](const std::pair<AGridCell*, FAStarHelper>& elem)
			{
				return FindNode(elem, pNeighborData.first);
			});
			UE_LOG(LogTemp, Warning, TEXT("NeighborCell  X: %d, Y:%d gCost: %d hCost: %f"), pNeighborData.first->m_CellCord.X,
			       pNeighborData.first->m_CellCord.Y, pNeighborData.second.gCost, pNeighborData.second.hCost);

			if(!pEntry)
			{

				auto pOpenListEntry = Algo::FindByPredicate(openList, [&pNeighborData](const std::pair<AGridCell*, FAStarHelper>& elem)
				{
					return FindNode(elem, pNeighborData.first);
				});
				if(!pOpenListEntry)
				{

					UE_LOG(LogTemp, Warning, TEXT("addedCell  X: %d, Y:%d gCost: %d hCost: %f"), pNeighborData.first->m_CellCord.X,
					       pNeighborData.first->m_CellCord.Y, pNeighborData.second.gCost, pNeighborData.second.hCost);
					openList.push_back(pNeighborData);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("pOpenListEntryCell  X: %d, Y:%d gCost: %d hCost: %f"), pOpenListEntry->first->m_CellCord.X,
					       pOpenListEntry->first->m_CellCord.Y, pOpenListEntry->second.gCost, pOpenListEntry->second.hCost);

					if(pOpenListEntry->second.gCost > pCurrentCell->second.gCost + m_MovementCost)
					{
						UE_LOG(LogTemp, Warning, TEXT("Reparented node at X:%d Y:%d to X:%d Y:%d"), pOpenListEntry->first->m_CellCord.X,
						       pOpenListEntry->first->m_CellCord.Y, pNeighborData.first->m_CellCord.X, pNeighborData.first->m_CellCord.Y)

						if(pCurrentCell->second.pParent != &pNeighborData.second)
							continue;
						pOpenListEntry->second = pNeighborData.second;
						pOpenListEntry->second.pParent = &pCurrentCell->second;
						pOpenListEntry->second.gCost = pCurrentCell->second.gCost + m_MovementCost;
					}
				}
			}
			/*else
			{

				UE_LOG(LogTemp, Warning, TEXT("pEntryCell  X: %d, Y:%d gCost: %d hCost: %f"), pEntry->first->m_CellCord.X,
				       pEntry->first->m_CellCord.Y, pEntry->second.gCost, pEntry->second.hCost);
				if(pEntry->second.gCost < pCurrentCell->second.gCost)
				{
					*pEntry = pNeighborData;
				}
			}#1#

		}
	}

	return path;*/
	return TArray<AGridCell*>{};
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
	if(coords.X >= m_GridLength - 1 || coords.Y >= m_GridWidth - 1)
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
	UE_LOG(LogTemp, Warning, TEXT("SETTING PARENT CurrentCell X:%d Y:%d ParrentCell X:%d,Y:%d"), pCurrentCell->m_CellCord.X, pCurrentCell->m_CellCord.Y,
	       pParent->first->m_CellCord.X, pParent->first->m_CellCord.Y);
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
		/*auto currIT{std::min_element(openList.begin(), openList.end(), [](const node* node1, const node* node2)
		{
			return node1->second.gCost + node1->second.hCost < node2->second.gCost + node2->second.hCost;
		})};
		if(currIT == openList.end())
		{
			continue;
		}
		current = *currIT;*/
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
		//current = *std::min_element(openList.begin(), openList.end(), [](const node* node1, const node* node2)
		//);
		/*current = *Algo::MinElement(openList, [](const auto& node1, const auto& node2)
		{
			CompareNodes(*node1, *node2);
		});*/
		//Found Target
		if(current->first == GetCellAtIndex(end))
		{

			UE_LOG(LogTemp, Warning, TEXT("finished"));
			break;
		}
		//Bugged list input
		closedList.push_back(current);
		openList.remove(current);
		UE_LOG(LogTemp, Warning, TEXT("NewNode"))
		for(auto neighbor : current->first->m_NeighborMap)
		{
			if(neighbor.Value == nullptr)
				continue;
			/*if(auto pCell = *Algo::FindByPredicate(closedList, [neighbor](const auto& elem)
			{
				return FindNode(elem, neighbor.Value);
			}))*/

			if(FindNode(closedList, neighbor.Value))
			{
				continue;
			}

			UE_LOG(LogTemp, Warning, TEXT("NeighborCoordinate: X:%d,Y:%d"), neighbor.Value->m_CellCord.X, neighbor.Value->m_CellCord.Y);
			int32 gCost = current->second.gCost + m_MovementCost;
			/*auto  successor{*Algo::FindByPredicate(openList, [neighbor](const auto& elem)
			{
				return FindNode(*elem, neighbor.Value);
			})};*/
			/*auto successor = std::find_if(openList.begin(), openList.end(), [neighbor](const auto* node)
			{
				if(!neighbor.Value || !node)
					return false;
				return node->first->m_CellCord.X == neighbor.Value->m_CellCord.X && node->first->m_CellCord.Y == neighbor.Value->m_CellCord.Y;
			});*/
			auto pSuccessor = FindNode(openList, neighbor.Value);
			if(!pSuccessor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Prepared for open List X:%d Y:%d | parent X:%d Y:%d"), neighbor.Value->m_CellCord.X,
				       neighbor.Value->m_CellCord.Y,
				       current->first->m_CellCord.X, current->first->m_CellCord.Y);
				node newNode{
					std::make_pair(neighbor.Value, FAStarHelper{gCost, CalculateHeuristicCost(neighbor.Value->m_CellCord, end), &current->second,
						               neighbor.Value})};

				UE_LOG(LogTemp, Warning, TEXT("AddedToOpenList X:%d Y:%d | parent X:%d Y:%d"), newNode.first->m_CellCord.X,
				       newNode.first->m_CellCord.Y,
				       newNode.second.pParent->pOwner->m_CellCord.X, newNode.second.pParent->pOwner->m_CellCord.Y);
				allNodes.push_back(newNode);
				openList.push_back(&allNodes.back());
			}
			else
			{
				if(gCost < pSuccessor->second.gCost)
				{
					UE_LOG(LogTemp, Warning, TEXT("ReParented X:%d Y:%d | parent X:%d Y:%d"), pSuccessor->first->m_CellCord.X,
					       pSuccessor->first->m_CellCord.Y,
					       current->first->m_CellCord.X, current->first->m_CellCord.Y);
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
		UE_LOG(LogTemp, Warning, TEXT("current X:%d Y:%d | parent X:%d Y:%d"), pHelper->pOwner->m_CellCord.X, pHelper->pOwner->m_CellCord.Y,
		       pHelper->pParent->pOwner->m_CellCord.X, pHelper->pParent->pOwner->m_CellCord.Y);
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
