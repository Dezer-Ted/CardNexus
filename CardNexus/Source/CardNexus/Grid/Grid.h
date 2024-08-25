// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class CARDNEXUS_API AGrid : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 m_GridWidth{8};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 m_GridLength{20};

	const int32 m_CellWidth{210};

	TArray<TArray<class UChildActorComponent*>> m_Grid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AActor> m_CellBP{};

	void CreateGrid();
};
