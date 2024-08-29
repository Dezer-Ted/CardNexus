// Fill out your copyright notice in the Description page of Project Settings.

#include "CardNexus/Cards/CardEffects/Heal.h"
#include "Kismet/GameplayStatics.h"
#include "CardNexus/Grid/PlayerUnit.h"


// Sets default values for this component's properties
UHeal::UHeal()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHeal::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHeal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHeal::ActivateEffect()
{
	Super::ActivateEffect();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
		if (player)
		{
			player->AddHitPoints(m_HealAmount);
		}
	}
}

