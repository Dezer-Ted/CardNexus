// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardBase.h"
#include "GameFramework/Actor.h"
class ACard;
#include "Deck.generated.h"

UCLASS()
class CARDNEXUS_API ADeck : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeck();

protected:
	// Called when the game starts or when spawned
	const int32  m_MaxCardsPerDeck{3};
	const int32  m_ShuffleCycles{10};
	const SIZE_T m_StartingDeckSize{15};
	virtual void BeginPlay() override;
	void         InitBaseCardList();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Cards")
	TArray<ACard*> m_DeckList;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Cards")
	TArray<FName> m_CardNames{};
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Cards")
	TArray<ACard*> m_DiscardPile{};
	void ShuffleDiscardIntoDeck();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Cards")
	TSubclassOf<ACard> m_CardActor;
	void               Shuffle();
	ACard*             DrawCard();
	void               DiscardCard(ACard* pCard);
};
