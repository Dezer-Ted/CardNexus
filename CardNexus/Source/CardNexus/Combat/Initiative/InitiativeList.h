// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserListEntry.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InitiativeList.generated.h"

/**
 * 
 */
UCLASS()
class CARDNEXUS_API UInitiativeList : public UUserWidget {
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta =(BindWidget))
	class UListView* m_pList{};
};