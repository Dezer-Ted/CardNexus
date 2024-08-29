// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/Initiative/EndTurnButton.h"

#include "CardNexus/Grid/PlayerUnit.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UEndTurnButton::NativeConstruct()
{
	Super::NativeConstruct();
	if(m_EndTurn)
	{
		m_EndTurn->OnClicked.AddDynamic(this, &UEndTurnButton::OnClicked);
	}
}

void UEndTurnButton::OnClicked()
{

	auto player{Cast<APlayerUnit>(UGameplayStatics::GetActorOfClass(GetWorld(), m_PlayerBP))};
	if(player->m_IsTurnPlayer == false)
		return;
	player->EndTurn();

}
