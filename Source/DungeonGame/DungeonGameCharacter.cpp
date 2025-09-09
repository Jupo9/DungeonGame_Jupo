// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DungeonGame.h"
#include "CollectableItem.h"
#include "Lock.h"

ADungeonGameCharacter::ADungeonGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeonGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Started means that the action was just started/pressed - this means if you use this it only works when the button is first pressed
		//Completed means that the action was just finished/released - this means if you use this it only works when the button is released
		//Triggered means that the action is being triggered - this means if you use this it works when the button is pressed and held down

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeonGameCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeonGameCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonGameCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonGameCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeonGameCharacter::LookInput);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADungeonGameCharacter::Interact);
	}
	else
	{
		UE_LOG(LogDungeonGame, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADungeonGameCharacter::Interact()
{
	FVector start = FirstPersonCameraComponent->GetComponentLocation();
	FVector end = start + (FirstPersonCameraComponent->GetForwardVector() * maxInteractionDistance);
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f);

	FCollisionShape interactionSphere = FCollisionShape::MakeSphere(interactionSphereRadius);
	DrawDebugSphere(GetWorld(), end, interactionSphereRadius, 20, FColor::Blue, false, 5.0f);
	/*Draw a sphere from players view
		DrawDebugSphere(GetWorld(), start, interactionSphereRadius, 20, FColor::Green, false, 5.0f);
	*/

	FHitResult hitResult;
	//Trace channel name is found in Project Folder - Config - DefaultEngine.ini - with strg + F find "Interact" and its trace channel
	bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, interactionSphere);

	if (hasHit)
	{
		AActor* hitActor = hitResult.GetActor();

		if (hitActor->ActorHasTag("CollectableItem"))
		{
			//hitActor is a collectable item

			ACollectableItem* collectableItem =  Cast<ACollectableItem>(hitActor);
			if (collectableItem)
			{
				itemList.Add(collectableItem->itemName);

				collectableItem->Destroy();
			}
		}
		else if (hitActor->ActorHasTag("Lock"))
		{
			//hitActor is a lock

			ALock* lockActor = Cast<ALock>(hitActor);
			if (lockActor)
			{
				if (!lockActor->GetIsKeyPlaced())
				{
					//lock is empty
					int32 itemRemoved = itemList.RemoveSingle(lockActor->keyItemName);
					if (itemRemoved)
					{
						lockActor->SetIsKeyPlaced(true);
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("There is no fitting item in inventory"));
					}
				}
				else
				{
					itemList.Add(lockActor->keyItemName);
					lockActor->SetIsKeyPlaced(false);
				}
			}
		}

	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit!"));
	}
}


void ADungeonGameCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADungeonGameCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ADungeonGameCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeonGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeonGameCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADungeonGameCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
