// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffects/Firebolt.h"

#include "Animation/AnimSequenceHelpers.h"
#include "CardNexus/Combat/CombatGM.h"
#include "CardNexus/Combat/CombatPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFirebolt::UFirebolt()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFirebolt::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UFirebolt::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFirebolt::ResolveEffect(const FVector& pos)
{
	//UE_LOG(LogTemp, Warning, TEXT("Resolved Effect"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);
    
	if (FoundActors.Num() > 0)
	{
		APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
		if (player)
		{
			auto playerPos = player->GetGridPosition();
			FVector2f targetPos;
			targetPos.X = playerPos.X - pos.X;
			targetPos.Y = playerPos.Y - pos.Y;
			targetPos.Normalize();
			
			if(targetPos.X > -0.5 && targetPos.X < 0.5 && targetPos.Y > 0.5)
			{
				UE_LOG(LogTemp, Warning, TEXT("North"));
			}
		}
	}

}

void UFirebolt::ActivateEffect()
{
	Super::ActivateEffect();
	ACombatGM* gameMode{Cast<ACombatGM>(UGameplayStatics::GetGameMode(GetWorld()))};
	auto       pc = Cast<ACombatPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->StartOrientation(this);
}	
