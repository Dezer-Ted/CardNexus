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
	auto pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}

void UThunderwave::ProjectEffect(const FVector& pos)
{
	if(m_IsProjecting)
		return;
	m_IsProjecting = true;
	DisableProjection();
	auto       player = GetPlayer();
	FCellCoord playerCoord = player->GetGridPosition();
	m_HighlightedCells = GetSurroundingCells(playerCoord);
	for(const auto& cell : m_HighlightedCells)
	{
		if(cell != nullptr)
			cell->EnableHighlight(true);
	}
}

void UThunderwave::ResolveEffect(const FVector& pos)
{
	auto               player = GetPlayer();
	FCellCoord         playerCoord = player->GetGridPosition();
	TArray<AGridCell*> SurroundingCells = GetSurroundingCells(playerCoord);
	ApplyDamage(SurroundingCells, m_Damage);
	Super::ResolveEffect(pos);
}
