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

	bool operator==(const FCellCoord& other) const
	{
		return X == other.X && Y == other.Y;
	}
};

UENUM()
enum class ETileHighLightingMode {
	Base UMETA(DisplayName="Base"),
	AbilityHighlight UMETA(DisplayName="AbilityHighlight"),
	PathInRange UMETA(DisplayName="PathInRange"),
	PathOutOfRange UMETA(DisplayName="PathOutOfRange")
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
	void         EnableHighlight(const ETileHighLightingMode& mode);
	UPROPERTY()
	AUnitBase* m_CurrentUnit{};
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_pCellMesh;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* m_pBaseMat{};
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* m_pHighlight{};
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* m_pPathInRangeMat{};
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* m_pPathOutOfRangeMat{};
};
