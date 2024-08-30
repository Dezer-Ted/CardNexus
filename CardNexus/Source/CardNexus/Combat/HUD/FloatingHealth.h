// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingHealth.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class CARDNEXUS_API UFloatingHealth : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta =(BindWidget))
	UProgressBar* m_pProgressBar;
	virtual void NativeConstruct() override;
	UPROPERTY(meta =(BindWidget))
	UTextBlock* m_pHPText;
	
public:
	
	void SetHealth(int32 health,int32 maxHealth) const;
};
