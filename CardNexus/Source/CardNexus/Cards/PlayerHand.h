// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerHand.generated.h"

UCLASS()
class CARDNEXUS_API APlayerHand : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerHand();

protected:
	// Called when the game starts or when spawned
	virtual void         BeginPlay() override;
	TArray<class ACard*> m_HandCards{};
	class ADeck*         m_pDeck{nullptr};
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<AActor>   deckType{};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void DrawCard();
};
