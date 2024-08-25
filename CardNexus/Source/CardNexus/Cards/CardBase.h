// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "Components/TextRenderComponent.h"
#include  "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Delegates/Delegate.h"

#include "CardBase.generated.h"

UENUM()
enum class ECardAffectedArea {
	Cone UMETA(DisplayName = "Cone"),
	Projectile UMETA(DisplayName ="Projectile"),
	Self UMETA(DisplayName ="Self"),
	Surrounding UMETA(DisplayName ="Surrounding")
};

UENUM()
enum class ECardTag {
	Attack UMETA(DisplayName = "Attack"),
	Enchantment UMETA(DisplayName="Enchantment")
};

USTRUCT()
struct FCardData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName m_Name{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* m_pImage{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ECardTag> m_Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECardAffectedArea m_AffectedArea{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText m_EffectText{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCardEffectComponent* m_PEffectComponent;
};

UCLASS()
class CARDNEXUS_API ACardBase : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardBase();
	UFUNCTION(BlueprintCallable)
	void Initialize(const FName& cardName);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	UTextRenderComponent* m_NameRender{};

	UFUNCTION(BlueprintCallable)
	FCardData GetCardData() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	UTextRenderComponent* m_pNameRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	UTextRenderComponent* m_pTagRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	UTextRenderComponent* m_pEffectRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardData")
	UStaticMeshComponent* m_pImageMesh;

};

