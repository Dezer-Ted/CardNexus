// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffects/Firebolt.h"
//#include "CardNexus/Combat/CombatGM.h"
#include "CardNexus/Combat/CombatPlayerController.h"
#include "CardNexus/Grid/Grid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFirebolt::UFirebolt()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFirebolt::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UFirebolt::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFirebolt::ResolveEffect(const FVector& pos)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);

	if(FoundActors.Num() > 0)
	{
		APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
		if(player)
		{
			FCellCoord playerCoord = player->GetGridPosition();
			AGridCell* playerCell = AGrid::GetCellAtIndex(playerCoord);
			EGridDirections direction = DetermineDirection(pos);
			AGridCell* nextCell;
			AGridCell* currentCell = playerCell;
			for(int i = 0; i < m_MaxTilesTravelled; i++)
			{
				nextCell = currentCell->m_NeighborMap[direction];
				if(nextCell == nullptr)
				{
					//goes off the grid
					break;
				}
				if(nextCell->m_CurrentUnit != nullptr)
				{
					//hit player
					nextCell->m_CurrentUnit->AddHitPoints(m_Damage);
					break;
				}
				currentCell = nextCell;
			}
		}
	}
}

void UFirebolt::ActivateEffect()
{
	Super::ActivateEffect();
	auto       pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}
