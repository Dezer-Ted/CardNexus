// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/HUD/FloatingHealth.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UFloatingHealth::SetHealth(int32 health, int32 maxHealth) const
{
	m_pHPText->SetText(FText::FromString(FString::FromInt(health) + TEXT(" / " + FString::FromInt(maxHealth))));
	m_pProgressBar->SetPercent(static_cast<float>(health) / static_cast<float>(maxHealth));
}

void UFloatingHealth::NativeConstruct()
{

}
