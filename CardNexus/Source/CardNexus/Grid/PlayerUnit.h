// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/UnitBase.h"
#include "PlayerUnit.generated.h"

class UGameOverScreen;
/**
 * 
 */
UCLASS()
class CARDNEXUS_API APlayerUnit : public AUnitBase {
	GENERATED_BODY()

private:
	virtual void Death() override;

};
