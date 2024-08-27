// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"

class AGridCell;

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

	UPROPERTY()
	TArray<AGridCell*> m_Path;
	void               FollowPath(float dt);
	UPROPERTY()
	AGridCell* m_Target{};
	FVector    m_StartPos{};
	FVector    m_TargetPos{};
	float      m_TimePassed{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_TimePerSegment{2.f};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void         SetPath(TArray<AGridCell*> path);
};
