// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/PlayerHand.h"

#include "HairStrandsInterface.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CardNexus/Cards/Deck.h"
#include "CardNexus/Cards/Card.h"
// Sets default values
APlayerHand::APlayerHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	m_pSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	m_pSceneComp->SetupAttachment(RootComponent);
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

void APlayerHand::ConstructHand()
{
	if(m_HandCards.Num() == 0)
	{
		return;
	}
	else if(m_HandCards.Num() == 1)
	{
		m_HandCards[0]->SetActorLocation(this->GetActorLocation());
	}

	for(auto cards : m_HandCards)
		cards->SetActorLocation(this->GetActorLocation());
	int32 counter{};
	for(int32 i = -(m_HandCards.Num() / 2); i < m_HandCards.Num() / 2 + 1; ++i)
	{
		if(i == 0)
			++i;
		FVector currentPos{m_HandCards[counter]->GetActorLocation()};
		int32   offSet{(m_CardWidth + m_DistanceBetweenCards) * i};
		if(m_HandCards.Num() % 2 == 0)
		{
			if(offSet > 0)
			{
				offSet -= m_CardWidth / 2;
			}
			else
			{
				offSet += m_CardWidth / 2;
			}
		}
		currentPos.Y += offSet;
		m_HandCards[counter]->SetActorLocation(currentPos);
		++counter;
	}

}


// Called every frame
void APlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerHand::DrawCard()
{
	auto                      card{m_pDeck->DrawCard()};
	FAttachmentTransformRules atRules{FAttachmentTransformRules::SnapToTargetNotIncludingScale};
	card->AttachToActor(this, atRules,TEXT("Cards"));
	m_HandCards.Add(card);
	ConstructHand();
}

void APlayerHand::RemoveCard(ACard* card)
{
	auto idx{m_HandCards.Find(card)};
	m_pDeck->DiscardCard(card);
	card->SetActorLocation(FVector{0, 0, -100});
	m_HandCards.RemoveAt(idx);
	ConstructHand();
}