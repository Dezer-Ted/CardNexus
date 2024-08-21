// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardBase.h"
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
