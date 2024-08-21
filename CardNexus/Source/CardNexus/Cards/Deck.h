// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardBase.h"
#include "GameFramework/Actor.h"
#include "Deck.generated.h"

UCLASS()
class CARDNEXUS_API ADeck : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeck();

protected:
	// Called when the game starts or when spawned
	const int32 m_MaxCardsPerDeck{3};
	virtual void BeginPlay() override;
	void InitBaseCardList();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Cards")
	TArray<ACardBase*> m_DeckList;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Cards")
	TArray<FName> m_CardNames;
	UPROPERTY(EditDefaultsOnly,Category="Cards")
	TSubclassOf<ACardBase> m_CardActor;
};
