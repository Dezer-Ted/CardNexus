// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverScreen.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CARDNEXUS_API UGameOverScreen : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	//UPROPERTY(meta = (BindWidget))
	//UButton* m_pQuitButton;
	//UPROPERTY(meta = (BindWidget))
	//UButton* m_pPlayAgainButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_pGameOverOrWonText;
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* m_pPlayAgainText;
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* m_pQuitText;

	//UFUNCTION()
	//void OnQuitClicked();
	//UFUNCTION()
	//void OnPlayAgainClicked();
	void gameWon(bool won);
};
