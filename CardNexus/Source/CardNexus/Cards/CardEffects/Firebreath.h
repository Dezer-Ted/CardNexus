// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardNexus/Cards/CardEffectLibrary.h"
#include "Components/ActorComponent.h"
#include "Firebreath.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CARDNEXUS_API UFirebreath : public UCardEffectLibrary {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFirebreath();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ResolveEffect(const FVector& pos) override;
	virtual void ActivateEffect() override;
private:
	const int32 m_Damage{-4};
};
