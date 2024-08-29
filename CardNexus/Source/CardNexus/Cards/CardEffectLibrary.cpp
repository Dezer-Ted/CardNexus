// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffectLibrary.h"

// Sets default values for this component's properties
UCardEffectLibrary::UCardEffectLibrary()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffectLibrary::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCardEffectLibrary::ActivateEffect()
{

}

void UCardEffectLibrary::ResolveEffect(const FVector& direction)
{
	//THrow projectile in direction
	
}


// Called every frame
void UCardEffectLibrary::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

