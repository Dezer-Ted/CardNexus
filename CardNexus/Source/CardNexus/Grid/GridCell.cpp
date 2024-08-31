// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/GridCell.h"

// Sets default values
AGridCell::AGridCell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	m_pCellMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cell");
	m_pCellMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGridCell::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGridCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridCell::EnableHighlight(const ETileHighLightingMode& mode)
{
	switch(mode)
	{
	case ETileHighLightingMode::Base:
		m_pCellMesh->SetMaterial(0, m_pBaseMat);
		break;
	case ETileHighLightingMode::AbilityHighlight:
		m_pCellMesh->SetMaterial(0, m_pHighlight);
		break;
	case ETileHighLightingMode::PathInRange:
		m_pCellMesh->SetMaterial(0, m_pPathInRangeMat);
		break;
	case ETileHighLightingMode::PathOutOfRange:
		m_pCellMesh->SetMaterial(0, m_pPathOutOfRangeMat);
		break;
	}

}
