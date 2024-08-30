// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Grid/GridCell.h"
#include "Components/ActorComponent.h"
#include "CardEffectLibrary.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARDNEXUS_API UCardEffectLibrary : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCardEffectLibrary();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual EGridDirections DetermineDirection(FVector mousePos);
public:	
	// Called every frame
	virtual void ActivateEffect();
	virtual void ResolveEffect(const FVector& pos);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
