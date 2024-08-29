// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "GameFramework/PlayerController.h"
#include "CombatPlayerController.generated.h"

class UInitiativeList;
class UInitCard;
/**
 * 
 */
UCLASS()
class CARDNEXUS_API ACombatPlayerController : public APlayerController {
private:
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void         DetectHit() const;

public:
	virtual void AddUnitsToInitList(TArray<UInitCard*> units);
	virtual void PostInitializeComponents() override;
	virtual void PlayerTick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerUnit> m_PlayerUnitBP;

	UPROPERTY()
	APlayerUnit* m_pPlayer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInitiativeList> m_InitListBP;
	UPROPERTY()
	UInitiativeList* m_InitList{};
};
