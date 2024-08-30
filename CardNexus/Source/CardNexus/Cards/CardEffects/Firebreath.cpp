// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffects/Firebreath.h"

#include "CardNexus/Cards/Card.h"
#include "CardNexus/Cards/CardBase.h"
#include "CardNexus/Combat/CombatPlayerController.h"
#include "CardNexus/Grid/Grid.h"
#include "Kismet/GameplayStatics.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "CardNexus/Grid/GridCell.h"

// Sets default values for this component's properties
UFirebreath::UFirebreath()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UFirebreath::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFirebreath::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFirebreath::ActivateEffect()
{
	Super::ActivateEffect();
	auto       pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}

void UFirebreath::AddNeighbors(EGridDirections direction, TArray<AGridCell*>& effectedCells, AGridCell* currentCell, int maxidx)
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

void UFirebreath::ResolveEffect(const FVector& pos)
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), foundActors);
    
    if(foundActors.Num() == 0)
        return;
    
    
	APlayerUnit* player = Cast<APlayerUnit>(foundActors[0]);
	if (player == nullptr)
	    return;
	
	TArray<AGridCell*> effectedCells;
	
	//FCard* card = GetCardData();
	FCellCoord playerCoord = player->GetGridPosition();
	AGridCell* playerCell = AGrid::GetCellAtIndex(playerCoord);
	EGridDirections direction = DetermineDirection(pos);
	AGridCell* currentCell = playerCell;

	auto cardData = Cast<ACard>(GetOwner())->GetCardData();
	//Get the first line from the player to the Max Range
	AddNeighbors(direction, effectedCells, currentCell, cardData.m_Range);
	
	// Get the cells on the sides
	int beforeArrSize = effectedCells.Num();
	for(int i = 1; i < beforeArrSize; i++)
	{
		currentCell = effectedCells[i];
	    for(int j = 0; j < i; j++)
	    {
	        if(direction == EGridDirections::NORTH || direction == EGridDirections::SOUTH)
            {
	        	//add neighbors West of cell
	        	AddNeighbors(EGridDirections::WEST, effectedCells, currentCell, i);

	        	//add neigbors East of cell
	        	AddNeighbors(EGridDirections::EAST, effectedCells, currentCell, i);
            }
            else
            {
            	//add neighbors North of cell
            	AddNeighbors(EGridDirections::NORTH, effectedCells, currentCell, i);

            	//add neigbors South of cell
            	AddNeighbors(EGridDirections::SOUTH, effectedCells, currentCell, i);
            }
	    }
	}
	
	//Deal damage to the units in the effected cells
	for(int i = 0; i < effectedCells.Num(); i++)
	{
	    if(effectedCells[i]->m_CurrentUnit)
	    {
	        effectedCells[i]->m_CurrentUnit->AddHitPoints(m_Damage);
	    }
	}
}
