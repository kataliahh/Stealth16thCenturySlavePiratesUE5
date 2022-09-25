// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGame/MagicSystem/MagicComponent.h"
#include "StealthGame/Characters/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "MagicalActor.h"
// Sets default values for this component's properties
UMagicComponent::UMagicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UMagicComponent::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void UMagicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
//LEVITATING IMPLEMENTATIONS OF MAGICSYSTEM.

//this function lineTraces through CameraViewPoint and see if we hit a MagicActor we determine, then returns it.
AMagicalActor* UMagicComponent::lineTraceFromCamera(ACharacterBase* PC)
{
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerChar is null(UMagicComponent::lineTrace)"));
		return false;
	}
	if (UWorld * World{ GetWorld() })
	{
		FVector StartLocation{ PC->getCameraComponent()->GetComponentLocation() };
		FVector Direction{ PC->getCameraComponent()->GetForwardVector() };
		FVector EndLocation{ StartLocation + (Direction * m_LineTraceRange) };
		//DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 5.f, 0, 5.f);
		FHitResult HitRes;
		FCollisionObjectQueryParams ObjectQueryParams;
		//ECC_GameTraceChannel1 is the channel for Magicalobj,
		//so we are making sure to only Hit objects of those types.
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		bool bLineTraceHitSomething{ World->LineTraceSingleByObjectType(HitRes, StartLocation, EndLocation, ObjectQueryParams, CollisionQueryParams) };

		if (bLineTraceHitSomething)
		{
			return Cast<AMagicalActor>(HitRes.GetActor());
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("World is null(UMagicComponent::lineTrace)"));
	return nullptr;
}
//called in CharacterBase when we hit a levitateable object through linetracing.
void UMagicComponent::setLevitatingValues(AMagicalActor* ActorToLevitate)
{
	if (!ActorToLevitate)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorToLevitate is null(UMagicComponent::setLevitatingValues)"));
		return;
	}
	m_LevitatedActor = ActorToLevitate;
	//
	UStaticMeshComponent* LevitatedActorMesh{ ActorToLevitate->FindComponentByClass<UStaticMeshComponent>() };
	if (LevitatedActorMesh)
	{
		LevitatedActorMesh->SetSimulatePhysics(false);
		ActorToLevitate->m_bShouldLevitate = true;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("LevitatedActorMesh is null(UMagicComponent::setLevitatingValues)"));
}
void UMagicComponent::dropObject()
{
	if (m_LevitatedActor)
		m_LevitatedActor->fall();
	else
		UE_LOG(LogTemp, Warning, TEXT("m_LevitatedActor is null(MagicComponent::DropObject)"));
}
//END OF LEVITATING-SYSTEM.
void UMagicComponent::setActorVisibility(AMagicalActor* MagicalActor)
{
	if (!MagicalActor)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicalActor is null(UMagicComponent::setActorVisibility)"));
		return;
	}
	MagicalActor->setActorVisibility();
}

void UMagicComponent::pullObject(AMagicalActor* MagicalActorToMove,const FVector& StartLocation, const FVector& MoveDirection, const FVector& Destination)
{
	if (!MagicalActorToMove)
	{
		UE_LOG(LogTemp, Warning, TEXT("MagicalActorToMove is null MagicComponent::MagicalActorToMove"));
		return;
	}
	MagicalActorToMove->setMovementValues(StartLocation,MoveDirection, Destination);
}





