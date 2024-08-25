// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API ACombatPlayerController : public APlayerController
{
	
private:
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	void DetectHit();
public:
	virtual void PlayerTick(float DeltaTime) override;
	

};
