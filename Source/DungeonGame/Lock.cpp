// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"
#include "CollectableItem.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TriggerComponent.h"

// Sets default values
ALock::ALock()
{
    PrimaryActorTick.bCanEverTick = true;

    rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(rootComp);

    triggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("TriggerComponent"));
    triggerComp->SetupAttachment(rootComp);

    placementPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PlacementPoint"));
    placementPoint->SetupAttachment(rootComp);

    Tags.Add("Lock");

    isKeyPlaced = false;
    spawnedActor = nullptr;
    placedItemKey.Empty();
}

void ALock::BeginPlay()
{
	Super::BeginPlay();
	
	SetIsKeyPlaced(false);
}

void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALock::SetIsKeyPlaced(bool newIsKeyPlaced)
{
    isKeyPlaced = newIsKeyPlaced;

    if (triggerComp)
    {
        triggerComp->Trigger(newIsKeyPlaced);
    }
}

bool ALock::GetIsKeyPlaced()
{
	return isKeyPlaced;
}

void ALock::PlaceVariant(const FString& ItemKey, AActor* Spawned)
{
    placedItemKey = ItemKey;
    spawnedActor = Spawned;

    if (correctKeyMap.Contains(ItemKey) && correctKeyMap[ItemKey])
    {
        SetIsKeyPlaced(true); 
    }
    else
    {
        SetIsKeyPlaced(false); 
        UE_LOG(LogTemp, Warning, TEXT("Placed incorrect key variant on lock: %s"), *ItemKey);
    }
}

FString ALock::RemovePlacedVariant()
{
    FString KeyToReturn = placedItemKey;

    if (spawnedActor)
    {
        spawnedActor->Destroy();
        spawnedActor = nullptr;
    }

    placedItemKey.Empty();
    SetIsKeyPlaced(false);

    return KeyToReturn;
}
