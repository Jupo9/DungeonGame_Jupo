// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrowingPlant.generated.h"

UCLASS()
class DUNGEONGAME_API AGrowingPlant : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrowingPlant();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* collisionBox;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* plantMesh;

	UPROPERTY(EditAnywhere, Category = "Plant")
	float growthSpeed = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Plant")
	FVector maxScale = FVector(2.f, 2.f, 2.f);

	UPROPERTY(EditAnywhere, Category = "Plant")
	FVector minScale = FVector(1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, Category = "Plant")
	AActor* actorToActivate;

private:
	bool bInLight = false;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
