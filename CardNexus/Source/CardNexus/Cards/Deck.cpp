// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/Deck.h"

#include <random>

#include "CardNexus/Cards/Card.h"

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
	m_DeckList.Reserve(m_StartingDeckSize);
	std::vector<FName> names{FName{TEXT("Firebreath")}, FName{TEXT("Firebolt")}, FName{TEXT("Haste")}, FName{TEXT("Thunderwave")}, FName{TEXT("Heal")}};
	UWorld*            world{GetWorld()};
	if(!world)
		return;
	FActorSpawnParameters spawnParams{};
	spawnParams.Owner = this;

	for(int32 j{}; j < names.size(); ++j)
	{

		for(int32 i = 0; i < m_MaxCardsPerDeck; ++i)
		{
			auto* card{world->SpawnActor<ACard>(m_CardActor, FVector{0, 0, 0}, FRotator::ZeroRotator, spawnParams)};
			card->Initialize(names[j], this);
			m_DeckList.Add(card);
		}
	}
	Shuffle();
}

void ADeck::ShuffleDiscardIntoDeck()
{
	m_DeckList = m_DiscardPile;
	m_DiscardPile.Reset(0);
	Shuffle();
}

// Called every frame
void ADeck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeck::Shuffle()
{
	for(int32 k = 0; k < m_ShuffleCycles; ++k)
	{
		for(int32 i = m_DeckList.Num() - 1; i > 0; i--)
		{
			int32  j = FMath::Floor(FMath::Rand() * (i + 1)) % m_DeckList.Num();
			ACard* temp = m_DeckList[i];
			m_DeckList[i] = m_DeckList[j];
			m_DeckList[j] = temp;
		}
	}

}

ACard* ADeck::DrawCard()
{
	if(m_DeckList.Num() == 0)
		ShuffleDiscardIntoDeck();
	return m_DeckList.Pop(EAllowShrinking::No);
}

void ADeck::DiscardCard(ACard* pCard)
{
	m_DiscardPile.Add(pCard);

}
