// Copyright Epic Games, Inc. All Rights Reserved.

#include "CryptRaiderPickUpComponent.h"

UCryptRaiderPickUpComponent::UCryptRaiderPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UCryptRaiderPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UCryptRaiderPickUpComponent::OnSphereBeginOverlap);
}

void UCryptRaiderPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ACryptRaiderCharacter* Character = Cast<ACryptRaiderCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
