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
	
	if (!TargetActor)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, FoundActors);

		if (FoundActors.Num() > 0)
		{
			TargetActor = FoundActors[0];
			UE_LOG(LogTemp, Display, TEXT("%s will look at %s"), *GetName(), *TargetActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s could not find any Actor with tag '%s'"), *GetName(), *TargetTag.ToString());
		}
	}
}

// Called every frame
void ALookAtActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (TargetActor)
	{
		// Aktuelle Position des Actors
		FVector EyeLocation = GetActorLocation();

		// Position des Ziel-Actors
		FVector TargetLocation = TargetActor->GetActorLocation();

		// Rotation berechnen, die X-Achse auf Target zeigt
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EyeLocation, TargetLocation);

		// Sanfte Rotation: Interpolieren zwischen aktueller Rotation und Zielrotation
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, LookAtRotation, DeltaTime, RotationSpeed);

		// Neue Rotation setzen
		SetActorRotation(NewRotation);
	}
}

