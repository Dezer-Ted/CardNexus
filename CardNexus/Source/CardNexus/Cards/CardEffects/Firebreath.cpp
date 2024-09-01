// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffects/Firebreath.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
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
	auto pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}

void UFirebreath::ProjectEffect(const FVector& pos)
{
	m_IsProjecting = true;
	auto            player{GetPlayer()};
	FCellCoord      playerCoord = player->GetGridPosition();
	EGridDirections direction = DetermineDirection(pos);
	if(direction == m_ProjectionDirection)
		return;
	DisableProjection();
	m_ProjectionDirection = direction;
	auto cardData = Cast<ACard>(GetOwner())->GetCardData();
	m_HighlightedCells = GetConeAffectedCells(playerCoord, cardData.m_Range, direction);
	for(const auto& cell : m_HighlightedCells)
	{
		if(cell != nullptr)
			cell->EnableHighlight(ETileHighLightingMode::AbilityHighlight);
	}
}

void UFirebreath::ResolveEffect(const FVector& pos)
{
	auto               player{GetPlayer()};
	FCellCoord         playerCoord = player->GetGridPosition();
	EGridDirections    direction = DetermineDirection(pos);
	auto               cardData = Cast<ACard>(GetOwner())->GetCardData();
	TArray<AGridCell*> affectedCells = GetConeAffectedCells(playerCoord, cardData.m_Range, direction);
	FRotator           rotator{};
	switch(direction)
	{
	case EGridDirections::NORTH:
		rotator = FRotator{0, 270, 90};
		break;
	case EGridDirections::SOUTH:
		rotator = FRotator{0, 90, 90};
		break;
	case EGridDirections::EAST:
		rotator = FRotator{0, 0, 90};
		break;
	case EGridDirections::WEST:
		rotator = FRotator{0, 180, 90};
		break;
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cast<ACard>(GetOwner())->m_pFlameBreath, player->GetActorLocation(), rotator, FVector{1},
	                                               true, true, ENCPoolMethod::None, true);
	ApplyDamage(affectedCells, m_Damage);
	Super::ResolveEffect(pos);
}
