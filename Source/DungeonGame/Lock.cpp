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

    PlacementPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PlacementPoint"));
    PlacementPoint->SetupAttachment(rootComp);

    Tags.Add("Lock");

    isKeyPlaced = false;
    SpawnedActor = nullptr;
    PlacedItemKey.Empty();
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
    PlacedItemKey = ItemKey;
    SpawnedActor = Spawned;
    SetIsKeyPlaced(true);
}

FString ALock::RemovePlacedVariant()
{
    FString KeyToReturn = PlacedItemKey;

    if (SpawnedActor)
    {
        SpawnedActor->Destroy();
        SpawnedActor = nullptr;
    }

    PlacedItemKey.Empty();
    SetIsKeyPlaced(false);

    return KeyToReturn;
}
