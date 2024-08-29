// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Grid/EnemyUnit.h"

void AEnemyUnit::StartTurn()
{
	Super::StartTurn();
	FTimerHandle timerHand;
	GetWorldTimerManager().SetTimer(timerHand, this, &AEnemyUnit::EndTurn, 3.f, false);
	UE_LOG(LogTemp, Warning, TEXT("StartedTurn"));
}

void AEnemyUnit::EndTurn()
{
	Super::EndTurn();
	UE_LOG(LogTemp, Warning, TEXT("EndedTurn"));
}
