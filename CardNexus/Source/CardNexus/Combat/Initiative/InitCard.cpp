// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/Initiative/InitCard.h"

#include "InitViewEntry.h"
#include "Components/TextBlock.h"

void UInitCard::NativeConstruct()
{

}

void UInitCard::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	auto entry{Cast<UInitViewEntry>(ListItemObject)};
	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *entry->m_Name.ToString(), *entry->m_Init.ToString());
	m_InitCounter->SetText(entry->m_Init);
	m_UnitName->SetText(entry->m_Name);
}

void UInitCard::UpdateCard(const FInitListEntry& entry)
{
	m_InitCounter->SetText(entry.m_Init);
	m_UnitName->SetText(entry.m_Name);
}
