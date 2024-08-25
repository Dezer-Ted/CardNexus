// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/PlayerHand.h"

#include "Kismet/GameplayStatics.h"
#include "CardNexus/Cards/Deck.h"
// Sets default values
APlayerHand::APlayerHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerHand::BeginPlay()
{
	Super::BeginPlay();
	m_HandCards.Reserve(10);
	TArray<AActor*> decks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), deckType, decks);
	m_pDeck = Cast<ADeck>(decks[0]);

	//m_HandCards.Add(m_pDeck->DrawCard());
}

// Called every frame
void APlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerHand::DrawCard()
{
	m_HandCards.Add(m_pDeck->DrawCard());
}
