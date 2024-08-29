// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InitCard.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API UInitCard : public UUserWidget, public IUserObjectListEntry {
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* m_InitCounter;
	UPROPERTY(meta =(BindWidget))
	UTextBlock* m_UnitName;
};
