// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include <vector>

#include "CoreMinimal.h"
#include "CardNexus/Grid/UnitBase.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGM.generated.h"

class UCardEffectLibrary;
class UInitCard;
class APlayerUnit;
class AEnemyUnit;
/**
 * 
 */
UENUM()
enum class ECombatUnitType {
	PlayerUnit UMETA(DisplayName ="PlayerUnit"),
	EnemyUnit UMETA(DisplayName = "EnemyUnit")
};

USTRUCT()
struct FUnitData {
	GENERATED_BODY()
	FCellCoord      m_Coords{};
	ECombatUnitType m_Type{};
};

UCLASS()
class CARDNEXUS_API ACombatGM : public AGameModeBase {
	using InitEntry = std::pair<AUnitBase*, int32>;

public:
	void AdvanceInitiative();
	

protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	APlayerController* m_pPlayerController;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> m_EnemyUnitBP;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> m_PlayerUnitBP;
	void                LoadInit(const TArray<FUnitData>& units);
	GENERATED_BODY()
	InitEntry*              m_TurnUnit{nullptr};
	std::list<InitEntry>    m_Initiative;
	std::vector<InitEntry*> m_ActiveInit;
	inline static int32     m_EnemyCount{};
	void                    AddToInitiative(const InitEntry& entry);
	void                    StartInitiative();
	void                    UpdateInitHud();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInitCard> m_InitCardBP;
};
