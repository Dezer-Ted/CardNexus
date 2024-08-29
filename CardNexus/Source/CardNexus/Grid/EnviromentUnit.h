// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/UnitBase.h"
#include "EnviromentUnit.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API AEnviromentUnit : public AUnitBase {
	GENERATED_BODY()
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_XPos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_YPos;
};
