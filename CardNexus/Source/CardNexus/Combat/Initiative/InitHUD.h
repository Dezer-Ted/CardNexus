// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "InitHUD.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API UInitHUD : public UUserWidget 
{
	
	GENERATED_BODY()
	virtual void NativeConstruct() override;
};
