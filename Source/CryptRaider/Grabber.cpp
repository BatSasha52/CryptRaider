// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	
	if (physicsHandle && physicsHandle->GetGrabbedComponent() != nullptr){
	FVector targetLocation = GetComponentLocation() + GetForwardVector() * holdDistance;
	physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab(){
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	FHitResult hitResult;
	if(physicsHandle == nullptr){
		UE_LOG(LogTemp, Error, TEXT("There is no UPhysicsHandleComponent"));
		return;
	}

	if(GetGrabbableInReach(hitResult)){
	UPrimitiveComponent* hitComponent = hitResult.GetComponent();
	hitComponent->SetSimulatePhysics(true);
	hitComponent->WakeAllRigidBodies();
	AActor* hitActor = hitResult.GetActor();
	hitActor->Tags.Add("Grabbed");
	hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FRotator newRotation = hitComponent->GetComponentRotation();
    newRotation.Yaw -= 90.0f;

	physicsHandle->GrabComponentAtLocationWithRotation(
		hitComponent,
		NAME_None,
		hitResult.ImpactPoint,
		newRotation);
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * maxGrabDistance;
	//DrawDebugLine(GetWorld(), start, end, FColor::Red);

	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	return GetWorld()->SweepSingleByChannel(
		outHitResult, 
		start, end, 
		FQuat::Identity, 
		ECC_GameTraceChannel2,
		sphere);
}

void UGrabber::Release(){
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	if (physicsHandle && physicsHandle->GetGrabbedComponent() != nullptr){
		AActor* grabbedActor = physicsHandle->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove("Grabbed");
		physicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		physicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

