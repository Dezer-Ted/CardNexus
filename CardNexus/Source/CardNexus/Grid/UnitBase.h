// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardNexus/Grid/GridCell.h"
#include "UnitBase.generated.h"

class UFloatingHealth;
class ACombatGM;
class AGridCell;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS()
class CARDNEXUS_API AUnitBase : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool         m_MoveToPath{false};
	virtual void Death();
	UPROPERTY()
	TArray<AGridCell*> m_Path;
	void               FollowPath(float dt);
	UPROPERTY()
	AGridCell* m_Target{};
	FVector    m_StartPos{};
	FVector    m_TargetPos{};
	float      m_TimePassed{};
	FCellCoord m_GridPos{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_TimePerSegment{2.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 m_HitPoints{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 m_MaxHitPoints{30};
	int32 CheckIfValidPath();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFloatingHealth> m_FloatingHealthBP;
	UPROPERTY()
	UFloatingHealth* m_pFloatingHealth;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* m_pHpBarWidget;

public:
	// Called every frame
	UPROPERTY()
	ACombatGM* m_pCombatGM{nullptr};
	UFUNCTION(BlueprintCallable)
	int32 GetHitPoints() const;
	virtual void  AddHitPoints(int32 hpDelta);
	FName m_UnitName{};
	int32 m_MovementSpeed{5};
	int32 m_CurrentMovementSpeed{};
	bool  m_IsTurnPlayer{};
	UPROPERTY(BlueprintAssignable)
	FDeathDelegate     DeathEvent;
	virtual void       Tick(float DeltaTime) override;
	virtual void       SetPath(TArray<AGridCell*> path);
	virtual FCellCoord GetGridPosition() const;
	virtual void       SetGridPosition(FCellCoord coord);
	virtual void       ReachedDestination();
	UFUNCTION(BlueprintCallable)
	virtual void StartTurn();

	virtual void EndTurn();
};
