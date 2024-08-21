// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/Deck.h"

// Sets default values
ADeck::ADeck()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ADeck::BeginPlay()
{
	Super::BeginPlay();
	InitBaseCardList();
}

void ADeck::InitBaseCardList()
{
	std::vector<FName> names{FName{TEXT("Firebreath")}, FName{TEXT("Firebolt")}, FName{TEXT("Haste")}, FName{TEXT("Thunderwave")}, FName{TEXT("Heal")}};
	UWorld* world {GetWorld()};
	if(!world)
		return;
	FActorSpawnParameters spawnParams{};
	spawnParams.Owner = this;
	
	for(int32 j{}; j < names.size(); ++j)
	{
		
		for(int32 i = 0; i < m_MaxCardsPerDeck; ++i)
		{
			auto* card{world->SpawnActor<ACardBase>(m_CardActor,FVector{0,0,0},FRotator::ZeroRotator,spawnParams)};
			card->Initialize(names[j]);
		}
	}
}

// Called every frame
void ADeck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
