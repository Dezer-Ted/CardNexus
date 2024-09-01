// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandPawn.h"
#include "Camera/CameraComponent.h"
#include "Cards/PlayerHand.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grid/Grid.h"
#include "Grid/PlayerUnit.h"
#include "Kismet/GameplayStatics.h"

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
	m_OriginalLocation = GetActorLocation();
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
	InputComponent->BindAxis("CameraMovementY",this,&APlayerHandPawn::MoveCameraYaxis);
	InputComponent->BindAxis("CameraMovementX",this,&APlayerHandPawn::MoveCameraXaxis);
}

void APlayerHandPawn::DrawCard()
{
	m_pHand->DrawCard();
}

void APlayerHandPawn::MoveCameraYaxis(float num)
{
	if(num == 0.0f)
		return;

	//BP_HandPawn has a camera
	FVector nextLocation = GetActorLocation();
	nextLocation += FVector( 0.0f,num * m_speed * GetWorld()->GetDeltaSeconds(),  0.0f);
	if(nextLocation.Y >  m_OriginalLocation.Y - 1000.0f && nextLocation.Y < m_OriginalLocation.Y + 1000.0f)
		SetActorLocation(nextLocation);
}

void APlayerHandPawn::MoveCameraXaxis(float num)
{
	if(num == 0.0f)
		return;

	//BP_HandPawn has a camera
	FVector nextLocation = GetActorLocation();
	nextLocation += FVector(num * m_speed * GetWorld()->GetDeltaSeconds(),  0.0f, 0.0f);
	if(nextLocation.X >  m_OriginalLocation.X - 1000.0f && nextLocation.X < m_OriginalLocation.X + 400.0f)
		SetActorLocation(nextLocation);
}