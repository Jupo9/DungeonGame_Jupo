// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerComponent.h"
#include "CollectableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Lock.generated.h"

UCLASS()
class DUNGEONGAME_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	ALock();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetIsKeyPlaced(bool newIsKeyPlaced);
	bool GetIsKeyPlaced();

	UPROPERTY(EditAnywhere, Category = "Variants")
	USceneComponent* placementPoint;

	UPROPERTY(EditAnywhere, Category = "Variants")
	TMap<FString, TSubclassOf<ACollectableItem>> variantMap;

	UPROPERTY(EditAnywhere, Category = "Variants")
	TMap<FString, bool> correctKeyMap;

	UPROPERTY()
	AActor* spawnedActor;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere)
	class UTriggerComponent* triggerComp;

	UFUNCTION()
	void PlaceVariant(const FString& ItemKey, AActor* Spawned);

	UFUNCTION()
	FString RemovePlacedVariant();

	UPROPERTY()
	FString placedItemKey;

private:

	UPROPERTY()
	bool isKeyPlaced;

};
