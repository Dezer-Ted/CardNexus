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
}

void UFirebolt::ResolveEffect(const FVector& pos)
{
	auto            player{GetPlayer()};
	EGridDirections direction = DetermineDirection(pos);
	auto            target = GetProjectileTarget(direction, m_MaxTilesTravelled, player->GetGridPosition());
	ApplyDamage({target}, m_Damage);
	Super::ResolveEffect(pos);
}

void UFirebolt::ActivateEffect()
{
	Super::ActivateEffect();
	auto pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}

void UFirebolt::ProjectEffect(const FVector& pos)
{
	m_IsProjecting = true;
	auto            player{GetPlayer()};
	EGridDirections direction = DetermineDirection(pos);
	if(direction == m_ProjectionDirection)
		return;
	DisableProjection();
	m_ProjectionDirection = direction;
	auto target = GetProjectileTarget(direction, m_MaxTilesTravelled, player->GetGridPosition());
	if(target == nullptr)
		return;
	m_HighlightedCells.Add(target);
	target->EnableHighlight(ETileHighLightingMode::AbilityHighlight);
}
