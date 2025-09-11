// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	startLocation = GetOwner()->GetActorLocation();
	startRotation = GetOwner()->GetActorRotation();
	SetShouldMove(false);
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* Show the time passed in game (in Output) 
		GetWorld()->TimeSeconds;
		UE_LOG(LogTemp, Warning, TEXT("Time: %f"), GetWorld()->TimeSeconds);
	*/

	FVector currentLocation = GetOwner()->GetActorLocation();
	FRotator currentRotation = GetOwner()->GetActorRotation();

	reachedTarget = currentLocation.Equals(targetLocation) && currentRotation.Equals(targetRotation);;

	if (!reachedTarget)
	{
		float speed = moveOffset.Length() / moveTime;

		FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, DeltaTime, speed);

		float rotationSpeed = rotateOffset.Euler().Size() / moveTime;

		FRotator newRotation = FMath::RInterpConstantTo(currentRotation, targetRotation, DeltaTime, rotationSpeed);

		GetOwner()->SetActorLocationAndRotation(newLocation, newRotation);
	}
}

bool UMover::GetShouldMove()
{
	return shouldMove;
}

void UMover::SetShouldMove(bool newShouldMove)
{
	shouldMove = newShouldMove;

	if (shouldMove)
	{
		targetLocation = startLocation + moveOffset;
		targetRotation = startRotation + rotateOffset;

		if (shouldDestroy)
		{
			GetOwner()->Destroy();
		}
	}
	else
	{
		targetLocation = startLocation;
		targetRotation = startRotation;
	}
}

