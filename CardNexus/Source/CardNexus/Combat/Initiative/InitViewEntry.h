// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InitViewEntry.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API UInitViewEntry : public UObject {
	GENERATED_BODY()
public:
	FText m_Name{};
	FText m_Init{};
};
