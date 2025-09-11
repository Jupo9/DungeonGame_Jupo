// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LookAtActor.generated.h"

UCLASS()
class DUNGEONGAME_API ALookAtActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALookAtActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "LookAt")
	AActor* TargetActor;

	// Rotationsgeschwindigkeit in Grad pro Sekunde
	UPROPERTY(EditAnywhere, Category = "LookAt")
	float RotationSpeed = 90.f;

	// Tag, nach dem gesucht wird, falls TargetActor leer ist
	UPROPERTY(EditAnywhere, Category = "LookAt")
	FName TargetTag = "Player";

};
