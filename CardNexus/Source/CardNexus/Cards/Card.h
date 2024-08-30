// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Card.generated.h"

class UCardEffectLibrary;
class APlayerHand;
class UStaticMeshComponent;
class UTextRenderComponent;

struct FCardData;

UCLASS()
class CARDNEXUS_API ACard : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACard();
	UFUNCTION(BlueprintCallable)
	void Initialize(const FName& cardName, class ADeck* deck);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	UDataTable* m_pItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	FName m_CardName{};

	FCardData* m_pCardData{};


	UFUNCTION(BlueprintCallable)
	FCardData GetCardData() const;

	void ActivateEffect();
	void DiscardThisCard();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CardData")
	TObjectPtr<UTextRenderComponent> m_pNameRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CardData", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> m_pTagRenderer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CardData", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> m_pEffectRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CardData")
	TObjectPtr<UStaticMeshComponent> m_pImageMesh;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AActor> m_PlayerHandBP{};
	APlayerHand* m_pHand{};
	ADeck* m_pDeck{};

	UPROPERTY()
	UCardEffectLibrary* m_pCardEFfect;
};
