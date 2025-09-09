// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Lock.generated.h"

UCLASS()
class DUNGEONGAME_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere)
	UTriggerComponent* triggerComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* keyItemMesh;

	UPROPERTY(EditAnywhere)
	FString keyItemName;

	void SetIsKeyPlaced(bool newIsKeyPlaced);
	bool GetIsKeyPlaced();

private:
	UPROPERTY(VisibleAnywhere)
	bool isKeyPlaced = false;
};
