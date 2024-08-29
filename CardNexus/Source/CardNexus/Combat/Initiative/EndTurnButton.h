// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndTurnButton.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class CARDNEXUS_API UEndTurnButton : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta=(BindWidget))
	UButton* m_EndTurn;
	UFUNCTION()
	void OnClicked();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> m_PlayerBP;
	
};
