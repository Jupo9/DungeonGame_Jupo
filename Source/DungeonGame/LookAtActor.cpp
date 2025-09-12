// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALookAtActor::ALookAtActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALookAtActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!targetActor)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), targetTag, FoundActors);

		if (FoundActors.Num() > 0)
		{
			targetActor = FoundActors[0];
			UE_LOG(LogTemp, Display, TEXT("%s will look at %s"), *GetName(), *targetActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s could not find any Actor with tag '%s'"), *GetName(), *targetTag.ToString());
		}
	}
}

// Called every frame
void ALookAtActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (targetActor)
	{
		FVector eyeLocation = GetActorLocation();
		FVector targetLocation = targetActor->GetActorLocation();

		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(eyeLocation, targetLocation);
		FRotator currentRotation = GetActorRotation();
		FRotator newRotation = FMath::RInterpConstantTo(currentRotation, lookAtRotation, DeltaTime, rotationSpeed);

		SetActorRotation(newRotation);
	}
}

