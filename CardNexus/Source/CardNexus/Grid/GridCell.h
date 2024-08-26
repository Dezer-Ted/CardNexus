// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.generated.h"

class AUnitBase;

UENUM()
enum class EGridDirections {
	NORTH UMETA(DisplayName="North"),
	SOUTH UMETA(DisplayName="South"),
	EAST UMETA(DisplayName="East"),
	WEST UMETA(DisplayName="West")
};

USTRUCT()
struct FCellCoord {
	GENERATED_BODY()
	int32 X;
	int32 Y;
};

UCLASS()
class CARDNEXUS_API AGridCell : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TMap<EGridDirections, AGridCell*> m_NeighborMap{};
	FCellCoord                        m_CellCord{};
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	AUnitBase* m_CurrentUnit{};
};
