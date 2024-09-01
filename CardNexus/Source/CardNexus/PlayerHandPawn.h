// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerHandPawn.generated.h"

UCLASS()
class CARDNEXUS_API APlayerHandPawn : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerHandPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY()
	class APlayerHand* m_pHand{};
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActorComponent> m_HandBlueprint;
	void                         DrawCard();
	void                         MoveCamera(float num);
private:
	FVector m_OriginalLocation;
	const float m_speed = 1200.0f;
};
