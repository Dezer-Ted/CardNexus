// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/UnitBase.h"
#include "InvisUnit.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API AInvisUnit : public AUnitBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
