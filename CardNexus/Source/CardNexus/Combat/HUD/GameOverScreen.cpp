// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Combat/HUD/GameOverScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

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

void UGameOverScreen::NativeConstruct()
{
	
}
