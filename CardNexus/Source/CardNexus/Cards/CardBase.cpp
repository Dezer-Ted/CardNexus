// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardBase.h"

#include <string>

#include "UObject/ConstructorHelpers.h"
// Sets default values
ACardBase::ACardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACardBase::Initialize(const FName& cardName)
{

	m_pItemDataTable = LoadObject<UDataTable>(nullptr,TEXT("DataTable'/Game/Resources/DataTables/CardDataNew.CardDataNew'"));
	static const FString contextString(TEXT("Card Table Context"));
	m_pCardData = m_pItemDataTable->FindRow<FCardData>(cardName, contextString, true);
	m_CardName = cardName;
	m_pNameRenderer->SetText(FText::FromString(m_CardName.ToString()));
	FString tagString{};
	for(const auto& tag : m_pCardData->m_Tags)
	{
		FString enumString{UEnum::GetValueAsString(tag)};
		int32   colonIndex{};
		enumString.FindLastChar(':', colonIndex);

		tagString += enumString.Mid(colonIndex) + " | ";
	}
	FString enumString{UEnum::GetValueAsString(m_pCardData->m_AffectedArea)};
	int32   colonIndex{};
	enumString.FindLastChar(':', colonIndex);
	tagString += enumString.Mid(colonIndex) + " | ";
	if(m_pCardData->m_Range != 0)
	{
		tagString += FString::FromInt(m_pCardData->m_Range) + "sq.";
	}
	m_pTagRenderer->SetText(FText::FromString(tagString));
	m_pEffectRenderer->SetText(m_pCardData->m_EffectText);
	m_pImageMesh->SetMaterial(0, m_pCardData->m_pImage);
}

// Called when the game starts or when spawned
void ACardBase::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ACardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FCardData ACardBase::GetCardData() const
{
	return *m_pCardData;
}
