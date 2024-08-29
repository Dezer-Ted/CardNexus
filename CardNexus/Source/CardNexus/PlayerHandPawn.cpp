// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandPawn.h"
#include "Camera/CameraComponent.h"
#include "Cards/PlayerHand.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerHandPawn::APlayerHandPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APlayerHandPawn::BeginPlay()
{
	Super::BeginPlay();
	auto pChildActor {Cast<UChildActorComponent>(this->GetComponentByClass(m_HandBlueprint))};
	m_pHand = Cast<APlayerHand>(pChildActor->GetChildActor());
	
}

// Called every frame
void APlayerHandPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerHandPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("DebugDraw",IE_Pressed,this,&APlayerHandPawn::DrawCard);
}

void APlayerHandPawn::DrawCard()
{
	m_pHand->DrawCard();
}
