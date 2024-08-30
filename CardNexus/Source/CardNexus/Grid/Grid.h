// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>

#include "CoreMinimal.h"
#include "GridCell.h"
//#include "IContentBrowserSingleton.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/Actor.h"
#include "Containers/List.h"
#include "Grid.generated.h"

USTRUCT()
struct FAStarHelper {
	GENERATED_BODY()
	int32         gCost{};
	float         hCost{};
	FAStarHelper* pParent{};
	AGridCell*    pOwner{};

	bool operator==(const FAStarHelper& other) const
	{
		return gCost == other.gCost && FMath::IsNearlyEqual(hCost, other.hCost) && pParent == other.pParent;
	}
};

bool CompareNodes(const std::pair<AGridCell*, FAStarHelper>& node1, const std::pair<AGridCell*, FAStarHelper>& node2);
bool FindNode(const std::pair<AGridCell*, FAStarHelper>& element, AGridCell* pCell);

UCLASS()
class CARDNEXUS_API AGrid : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 m_GridWidth{8};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32                                      m_GridLength{20};
	const int32                                m_CellWidth{200};
	static const int32                         m_MovementCost{5};
	void                                       SetupNeighbors(AGridCell* pCell);
	std::pair<bool, AGridCell*>                ValidateCell(const FCellCoord& coords);
	static std::pair<AGridCell*, FAStarHelper> CalculateCost(AGridCell* pCurrentCell, std::pair<AGridCell*, FAStarHelper>* pParent, FCellCoord end);
	static float                               CalculateHeuristicCost(const FCellCoord& coord1, const FCellCoord& coord2);
	//static TArray<AGridCell*> 
	void CreateGrid();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	inline static TArray<TArray<class UChildActorComponent*>> m_Grid{};

	static AGridCell*                           GetCellAtIndex(FCellCoord coords);
	static TArray<AGridCell*>                   FindPath(const FCellCoord& start, const FCellCoord& end);
	static std::pair<AGridCell*, FAStarHelper>* FindNode(const std::list<std::pair<AGridCell*, FAStarHelper>*>& list, AGridCell* target);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AActor> m_CellBP{};
};
