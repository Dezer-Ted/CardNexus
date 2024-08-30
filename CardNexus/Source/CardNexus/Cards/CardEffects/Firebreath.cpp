// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffects/Firebreath.h"

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

void UFirebreath::ResolveEffect(const FVector& pos)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);
    
	if (FoundActors.Num() > 0)
	{
		APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
		if (player)
		{
			FCellCoord playerCoord = player->GetGridPosition();
			AGridCell* playerCell = AGrid::GetCellAtIndex(playerCoord);
			EGridDirections direction = DetermineDirection(pos);
			AGridCell* currentCell = playerCell;
			AGridCell* nextCell = playerCell->m_NeighborMap[direction];
			if(nextCell != nullptr)
			{
				nextCell->m_CurrentUnit->AddHitPoints(m_Damage);
				AGridCell* midTCell = nextCell->m_NeighborMap[direction];
				if(midTCell != nullptr)
				{
					midTCell->m_CurrentUnit->AddHitPoints(m_Damage);
					if(direction == EGridDirections::NORTH || direction == EGridDirections::SOUTH)
					{
						AGridCell* leftTCell = midTCell->m_NeighborMap[EGridDirections::WEST];
						if(leftTCell != nullptr)
						{
							leftTCell->m_CurrentUnit->AddHitPoints(m_Damage);
						}
						AGridCell* rightTCell = midTCell->m_NeighborMap[EGridDirections::EAST];
						if(rightTCell != nullptr)
						{
							rightTCell->m_CurrentUnit->AddHitPoints(m_Damage);
						}
					}
					else if(direction == EGridDirections::EAST || direction == EGridDirections::WEST)
					{
						AGridCell* upTCell = midTCell->m_NeighborMap[EGridDirections::NORTH];
						if(upTCell != nullptr)
						{
							upTCell->m_CurrentUnit->AddHitPoints(m_Damage);
						}
						AGridCell* downTCell = midTCell->m_NeighborMap[EGridDirections::SOUTH];
						if(upTCell != nullptr)
						{
							downTCell->m_CurrentUnit->AddHitPoints(m_Damage);
						}
					}
				}
			}
		}
	}
}
