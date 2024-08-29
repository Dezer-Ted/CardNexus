// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/EnviromentUnit.h"

#include "Grid.h"

void AEnviromentUnit::BeginPlay()
{
	Super::BeginPlay();
	auto cell = AGrid::GetCellAtIndex({m_XPos, m_YPos});
	cell->m_CurrentUnit = this;
	SetActorLocation(cell->GetActorLocation());
}
