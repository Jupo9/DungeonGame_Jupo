// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(rootComp);

	triggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("TriggerComponent"));
	triggerComp->SetupAttachment(rootComp);

	keyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyItemMesh"));
	keyItemMesh->SetupAttachment(rootComp);

	Tags.Add("Lock");
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	
	SetIsKeyPlaced(false);
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALock::SetIsKeyPlaced(bool newIsKeyPlaced)
{
	isKeyPlaced = newIsKeyPlaced;

	triggerComp->Trigger(newIsKeyPlaced);
	keyItemMesh->SetVisibility(newIsKeyPlaced);
}

bool ALock::GetIsKeyPlaced()
{
	return isKeyPlaced;
}


