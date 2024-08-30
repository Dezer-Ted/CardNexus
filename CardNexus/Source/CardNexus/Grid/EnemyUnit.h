// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/UnitBase.h"
#include "EnemyUnit.generated.h"

class APlayerUnit;
/**
 * 
 */
UCLASS()
class CARDNEXUS_API AEnemyUnit : public AUnitBase {
	GENERATED_BODY()
	virtual void StartTurn() override;
	virtual void EndTurn() override;
	virtual void BeginPlay() override;
	virtual void ReachedDestination() override;

	void WalkToTarget();
	bool FindTargetInRange();
	void AttackTarget();
	UPROPERTY()
	APlayerUnit* m_pTarget;
	UPROPERTY(EditDefaultsOnly)
	int32 m_Damage{5};
};
