// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "GameFramework/PlayerController.h"
#include "Initiative/InitiativeList.h"
#include "Initiative/InitViewEntry.h"
#include "CombatPlayerController.generated.h"

class UEndTurnButton;
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
	virtual void AddUnitsToInitList(const TArray<UInitViewEntry*>& units);
	virtual void PostInitializeComponents() override;
	virtual void PlayerTick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerUnit> m_PlayerUnitBP;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInitCard> m_InitCardBP;

	UPROPERTY()
	APlayerUnit* m_pPlayer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInitiativeList> m_InitListBP;
	UPROPERTY()
	UInitiativeList* m_InitList{};
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEndTurnButton> m_EndTurnButtonBP;
	UPROPERTY()
	UEndTurnButton* m_EndTurnButton;

	
};
