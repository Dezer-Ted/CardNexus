// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffects/Thunderwave.h"

#include "CardNexus/Combat/CombatPlayerController.h"
#include "CardNexus/Grid/Grid.h"
#include "Kismet/GameplayStatics.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "CardNexus/Grid/GridCell.h"

// Sets default values for this component's properties
UThunderwave::UThunderwave()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UThunderwave::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UThunderwave::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UThunderwave::ActivateEffect()
{
	Super::ActivateEffect();
	auto       pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}

void UThunderwave::ResolveEffect(const FVector& pos)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);
    
	if (FoundActors.Num() == 0)
		return;
    
	APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
	if (player == nullptr)
		return;
	
    
	TArray<AGridCell*> SurroundingCells;
    
	//Get Surrounding player cells
	FCellCoord playerCoord = player->GetGridPosition();
	AGridCell* playerCell = AGrid::GetCellAtIndex(playerCoord);
	SurroundingCells.Emplace(playerCell->m_NeighborMap[EGridDirections::NORTH]);
	SurroundingCells.Emplace(playerCell->m_NeighborMap[EGridDirections::EAST]);
	SurroundingCells.Emplace(playerCell->m_NeighborMap[EGridDirections::SOUTH]);
	SurroundingCells.Emplace(playerCell->m_NeighborMap[EGridDirections::WEST]);

	if(SurroundingCells[0] != nullptr && SurroundingCells[1] != nullptr)
	{
		SurroundingCells.Emplace(SurroundingCells[0]->m_NeighborMap[EGridDirections::EAST]);
	}
	if(SurroundingCells[1] != nullptr && SurroundingCells[2] != nullptr)
	{
		SurroundingCells.Emplace(SurroundingCells[1]->m_NeighborMap[EGridDirections::SOUTH]);
	}
	if(SurroundingCells[2] != nullptr && SurroundingCells[3] != nullptr)
	{
		SurroundingCells.Emplace(SurroundingCells[2]->m_NeighborMap[EGridDirections::WEST]);
	}
	if(SurroundingCells[3] != nullptr && SurroundingCells[0] != nullptr)
	{
		SurroundingCells.Emplace(SurroundingCells[3]->m_NeighborMap[EGridDirections::NORTH]);
	}
	
	//Do Damage to surrounding player cells if there is a unit
	for(int i = 0; i < SurroundingCells.Num(); i++)
	{
		if(SurroundingCells[i] != nullptr)
		{
			AUnitBase* unit;
			unit = SurroundingCells[i]->m_CurrentUnit;
			if(unit)
			{
				unit->AddHitPoints(m_Damage);
			}
		}
	}
}