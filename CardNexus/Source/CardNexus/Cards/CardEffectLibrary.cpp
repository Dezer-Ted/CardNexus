// Fill out your copyright notice in the Description page of Project Settings.


#include "CardNexus/Cards/CardEffectLibrary.h"

#include "InteractiveToolManager.h"
#include "CardNexus/Grid/PlayerUnit.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCardEffectLibrary::UCardEffectLibrary()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffectLibrary::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UCardEffectLibrary::DetermineDirection(FVector mousePos)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerUnit::StaticClass(), FoundActors);

	if(FoundActors.Num() > 0)
	{
		APlayerUnit* player = Cast<APlayerUnit>(FoundActors[0]);
		if(player)
		{
			auto      playerPos = player->GetActorLocation();
			FVector2f targetPos;
			targetPos.X = mousePos.X - playerPos.X;
			targetPos.Y = mousePos.Y - playerPos.Y;
			targetPos.Normalize();
			//East
			// Tolerance value for comparison
			float Tolerance = 0.5f;
			UE_LOG(LogTemp, Warning, TEXT("X:%f,Y:%f"), targetPos.X, targetPos.Y);
			if(FMath::Abs(targetPos.X - 1.0f) < Tolerance && FMath::Abs(targetPos.Y) < Tolerance)
			{
				UE_LOG(LogTemp, Warning, TEXT("The vector points to the Down"));
			}
			else if(FMath::Abs(targetPos.X + 1.0f) < Tolerance && FMath::Abs(targetPos.Y) < Tolerance)
			{
				UE_LOG(LogTemp, Warning, TEXT("The vector points to the Top"));
			}
			else if(FMath::Abs(targetPos.Y - 1.0f) < Tolerance && FMath::Abs(targetPos.X) < Tolerance)
			{
				UE_LOG(LogTemp, Warning, TEXT("The vector points Left"));
			}
			else if(FMath::Abs(targetPos.Y + 1.0f) < Tolerance && FMath::Abs(targetPos.X) < Tolerance)
			{
				UE_LOG(LogTemp, Warning, TEXT("The vector points Right"));
			}

		}
	}
}

void UCardEffectLibrary::ActivateEffect()
{

}

void UCardEffectLibrary::ResolveEffect(const FVector& direction)
{
	//THrow projectile in direction

}


// Called every frame
void UCardEffectLibrary::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
