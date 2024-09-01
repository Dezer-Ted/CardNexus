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
		{
			if(m_HandCards.Num() % 2 == 0)
			{
				++i;
			}
			else
			{
				m_HandCards[counter]->SetActorRelativeRotation(FRotator{0, 0, 0});
				FVector splitOffSet{m_HandCards[counter]->GetActorLocation()};
				FVector testValue{this->GetActorTransform().InverseTransformPosition(splitOffSet)};
				testValue.X = counter * -20;
				m_HandCards[counter]->SetActorRelativeLocation(testValue);
				counter++;
				continue;
			}
		}
		FVector currentPos{m_HandCards[counter]->GetActorLocation()};
		int32   offSet{(m_CardWidth + m_DistanceBetweenCards) * i};
		int32   rotation{m_RotationStep * i};
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


		FVector splitOffSet{m_HandCards[counter]->GetActorLocation()};
		FVector testValue{this->GetActorTransform().InverseTransformPosition(splitOffSet)};
		testValue += FVector{0, 0, static_cast<double>(abs(FMath::Pow(i, 2.f))) * -10};
		testValue.X = counter * -20;
		m_HandCards[counter]->SetActorRelativeLocation(testValue);
		m_HandCards[counter]->SetActorRelativeRotation(FRotator{0, 0, 0});
		m_HandCards[counter]->SetActorRelativeRotation(FRotator{0, 0, static_cast<double>(rotation)});
		++counter;
	}

}

void APlayerHand::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}


// Called every frame
void APlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerHand::DrawCard()
{
	if(m_HandCards.Num() >= m_MaxHandSize)
		return;
	auto card{m_pDeck->DrawCard()};
	if(card == nullptr)
		return;
	FAttachmentTransformRules atRules{FAttachmentTransformRules::SnapToTargetNotIncludingScale};
	card->AttachToActor(this, atRules,TEXT("Cards"));
	m_HandCards.Add(card);
	ConstructHand();
}

void APlayerHand::RemoveCard(ACard* card)
{
	auto idx{m_HandCards.Find(card)};
	m_pDeck->DiscardCard(card);
	card->SetActorLocation(FVector{0, 0, 10000});
	m_HandCards.RemoveAt(idx);
	ConstructHand();
}
