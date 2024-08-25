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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> m_pCamera{};
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> m_pSpringArm{};
};
