// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/GridCell.h"
#include "Components/ActorComponent.h"
#include "CardEffectLibrary.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CARDNEXUS_API UCardEffectLibrary : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffectLibrary();

protected:
	// Called when the game starts
	virtual void                     BeginPlay() override;
	virtual EGridDirections          DetermineDirection(FVector mousePos);
	virtual TArray<class AGridCell*> GetSurroundingCells(const FCellCoord& pos);
	virtual void                     ApplyDamage(const TArray<AGridCell*>& affectedCells, int32 damage);
	virtual class APlayerUnit*       GetPlayer();
	virtual AGridCell*               GetProjectileTarget(const EGridDirections& direction, int32 range, const FCellCoord& startPos);
	virtual TArray<AGridCell*>       GetConeAffectedCells(const FCellCoord& startPos, int32 range, const EGridDirections& direction);
	virtual void                     AddConeNeighbors(EGridDirections direction, TArray<AGridCell*>& effectedCells, AGridCell* currentCell, int maxidx);
	UPROPERTY()
	TArray<AGridCell*> m_HighlightedCells;
	EGridDirections    m_ProjectionDirection{};
	bool               m_IsProjecting{false};

public:
	// Called every frame
	virtual void DisableProjection();
	virtual void ActivateEffect();
	virtual void ProjectEffect(const FVector& pos);
	virtual void ResolveEffect(const FVector& pos);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
