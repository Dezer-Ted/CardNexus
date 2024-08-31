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
	const int32 m_CardWidth{225};
	const int32 m_DistanceBetweenCards{-50};
	const int32 m_RotationStep{5};
	const int32 m_MaxHandSize{10};

	virtual void         BeginPlay() override;
	TArray<class ACard*> m_HandCards{};
	UPROPERTY()
	class ADeck* m_pDeck{nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> deckType{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> m_pSceneComp{};
	virtual void                      PostInitializeComponents() override;

public:
	// Called every frame
	void         ConstructHand();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void DrawCard();
	void RemoveCard(ACard* card);

	UPROPERTY(EditDefaultsOnly)
	int32 m_StartingHandSize{5};
};
