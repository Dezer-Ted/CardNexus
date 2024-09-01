// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/Card.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "CardBase.h"
#include "CardEffectLibrary.h"
#include "Deck.h"
#include "PlayerHand.h"
#include "Kismet/GameplayStatics.h"

ACard::ACard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	m_pDemoCardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card"));
	m_pDemoCardMesh->SetupAttachment(RootComponent);
}

void ACard::Initialize(const FName& cardName, ADeck* deck)
{
	//TOOD: FIX IF MORE THAN ONE PLAYER HAND


	m_pHand = Cast<APlayerHand>(UGameplayStatics::GetActorOfClass(GetWorld(), m_PlayerHandBP));
	m_pItemDataTable = LoadObject<UDataTable>(nullptr,TEXT("DataTable'/Game/Resources/DataTables/CardDataNew.CardDataNew'"));
	static const FString contextString(TEXT("Card Table Context"));
	m_pCardData = m_pItemDataTable->FindRow<FCardData>(cardName, contextString, true);
	m_CardName = cardName;
	//m_pNameRenderer->SetText(FText::FromString(m_CardName.ToString()));
	FString tagString{};
	for(const auto& tag : m_pCardData->m_Tags)
	{
		FString enumString{UEnum::GetValueAsString(tag)};
		int32   colonIndex{};
		enumString.FindLastChar(':', colonIndex);
		tagString += enumString.Mid(colonIndex + 1) + " | ";
	}
	FString enumString{UEnum::GetValueAsString(m_pCardData->m_AffectedArea)};
	int32   colonIndex{};
	enumString.FindLastChar(':', colonIndex);
	tagString += enumString.Mid(colonIndex + 1);
	if(m_pCardData->m_Range != 0)
	{
		tagString += " | " + FString::FromInt(m_pCardData->m_Range) + "sq.";
	}
	/*m_pTagRenderer->SetText(FText::FromString(tagString));
	m_pEffectRenderer->SetText(m_pCardData->m_EffectText);
	m_pImageMesh->SetMaterial(0, m_pCardData->m_pImage);*/
	m_pCardEFfect = NewObject<UCardEffectLibrary>(this, m_pCardData->m_Effect);
	m_pCardEFfect->RegisterComponent();
	m_pDemoCardMesh->SetMaterial(0, m_pCardData->m_pImage);
	AddInstanceComponent(m_pCardEFfect);

}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FCardData ACard::GetCardData() const
{
	return *m_pCardData;
}

void ACard::ActivateEffect()
{
	m_pCardEFfect->ActivateEffect();
}

void ACard::DiscardThisCard()
{
	m_pHand->RemoveCard(this);
}
