// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandPawn.h"

#include "NavigationSystemTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerHandPawn::APlayerHandPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HandCamera"));
	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	m_pSpringArm->SetupAttachment(RootComponent);
	m_pCamera->SetupAttachment(m_pSpringArm);

}

// Called when the game starts or when spawned
void APlayerHandPawn::BeginPlay()
{
	Super::BeginPlay();

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

}
