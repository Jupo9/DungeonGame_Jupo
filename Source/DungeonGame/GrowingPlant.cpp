// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowingPlant.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AGrowingPlant::AGrowingPlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->SetBoxExtent(FVector(50.f, 50.f, 100.f));
	collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	plantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMesh"));
	plantMesh->SetupAttachment(RootComponent);
	plantMesh->SetRelativeScale3D(minScale);

	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGrowingPlant::OnOverlapBegin);
	collisionBox->OnComponentEndOverlap.AddDynamic(this, &AGrowingPlant::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AGrowingPlant::BeginPlay()
{
	Super::BeginPlay();
	
	plantMesh->SetRelativeScale3D(minScale);
}

// Called every frame
void AGrowingPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInLight)
	{
		FVector CurrentScale = plantMesh->GetRelativeScale3D();
		FVector NewScale = CurrentScale + growthSpeed * DeltaTime * FVector(1.f, 1.f, 1.f);

		NewScale.X = FMath::Clamp(NewScale.X, minScale.X, maxScale.X);
		NewScale.Y = FMath::Clamp(NewScale.Y, minScale.Y, maxScale.Y);
		NewScale.Z = FMath::Clamp(NewScale.Z, minScale.Z, maxScale.Z);

		plantMesh->SetRelativeScale3D(NewScale);

		if (NewScale.Equals(maxScale))
		{
			if (actorToActivate && actorToActivate->IsHidden())
			{
				actorToActivate->SetActorHiddenInGame(false);
				actorToActivate->SetActorEnableCollision(true);
				actorToActivate->SetActorTickEnabled(true);
			}

			Destroy();
		}
	}
}

void AGrowingPlant::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			bInLight = true;
		}
	}
}

void AGrowingPlant::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			bInLight = false;
		}
	}
}

