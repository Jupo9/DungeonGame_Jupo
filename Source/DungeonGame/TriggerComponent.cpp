// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : moverActors)
	{
		if (actor)
		{
			UMover* moverComp = actor->FindComponentByClass<UMover>();
			if (moverComp)
			{
				movers.Add(moverComp);
				UE_LOG(LogTemp, Display, TEXT("Added mover from %s"), *actor->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s has no UMover component"), *actor->GetName());
			}
		}
	}

	if (isPressurcePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::Trigger(bool newTriggerValue)
{
	isTriggered = newTriggerValue;

	if (movers.Num() > 0)
	{
		for (UMover* mover : movers)
		{
			if (mover)
			{
				mover->SetShouldMove(isTriggered);
			}
		}
		UE_LOG(LogTemp, Display, TEXT("Triggered %d movers"), movers.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no movers to trigger"), *GetOwner()->GetActorNameOrLabel());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		activatorCount++;

		if (!isTriggered)
		{
			Trigger(true);
		}
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		activatorCount--;

		if (isTriggered && activatorCount == 0)
		{
			Trigger(false);
		}
	}
}
