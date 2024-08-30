// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/HUD/GameOverScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGameOverScreen::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UGameOverScreen::OnPlayAgainClicked()
{
	FLatentActionInfo latentInfo;
	FName levelname = GetWorld()->GetFName();
	UGameplayStatics::LoadStreamLevel(this, levelname,true, true, latentInfo);
}

void UGameOverScreen::gameWon(bool won)
{
	if (won == true)
	{
		m_pGameOverOrWonText->SetText(FText::FromString(FString("You won!")));
		m_pGameOverOrWonText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	}
	else
	{
		m_pGameOverOrWonText->SetText(FText::FromString(FString("Game over!")));
		m_pGameOverOrWonText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
}


void UGameOverScreen::NativeConstruct()
{
	if(m_pQuitButton)
	{
		m_pQuitButton->OnClicked.AddDynamic(this,&UGameOverScreen::OnQuitClicked);
	}
	if(m_pPlayAgainButton)
	{
		m_pPlayAgainButton->OnClicked.AddDynamic(this, &UGameOverScreen::OnPlayAgainClicked);
	}
}
