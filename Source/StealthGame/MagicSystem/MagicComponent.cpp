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
	FOnTimelineFloat LevitatingDelegate;
	LevitatingDelegate.BindUFunction(this, FName("levitateOverTime"));

	m_LevitatingTimeline.AddInterpFloat(m_liftingCurveFloat, LevitatingDelegate);

	// ...

}

// Called every frame
void UMagicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_LevitatingTimeline.TickTimeline(DeltaTime);
	//if the levitated obj reached the destination do a smooth up/down movement.
	if (m_bReachedPeakPoint)
	{
		jiggleLevitatedObject();
	}


	// ...
}
//LEVITATING IMPLEMENTATIONS OF MAGICSYSTEM.

//this function lineTraces through CameraViewPoint and see if we hit a MagicActor we determine, then returns it.
AMagicalActor* UMagicComponent::lineTraceFromCamera(ACharacterBase* PC)
{
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerChar is null(UMagicComponent::lineTraceFromCamera)"));
		return false;
	}

	if (UWorld * World{ GetWorld() })
	{
		FVector StartLocation{ PC->getCameraComponent()->GetComponentLocation() };
		FVector LineTraceDirection{ PC->getCameraComponent()->GetForwardVector() };
		FVector EndLocation{ StartLocation + (LineTraceDirection * m_LineTraceRange) };
		//DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 5.f, 0, 5.f);
		FHitResult HitRes;
		FCollisionObjectQueryParams ObjectQueryParams;
		//ECC_GameTraceChannel1 is the channel for levitateable obj,
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
	//UE_LOG(LogTemp, Warning, TEXT("World is null(UMagicComponent::lineTraceFromCamera)"));
	return nullptr;
}
//called in CharacterBase when we hit a levitateable object through linetracing.
void UMagicComponent::setLevitatingValues(AMagicalActor* ActorToLevitate, const FVector& LiftLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Lifting Actor Successfully(UMagicComponent::liftActor)"));
	if (m_liftingCurveFloat)
	{
		m_LevitatingLocation = LiftLocation;
		m_ActorToLevitate = ActorToLevitate;
		if (m_ActorToLevitate)
		{
			m_LevitatedActorMesh = m_ActorToLevitate->FindComponentByClass<UStaticMeshComponent>();
			if (m_LevitatedActorMesh)
			{
				//need to set simulate phsx to false in order to levitate it.
				m_LevitatedActorMesh->SetSimulatePhysics(false);
			}
		}
		//
		m_bCanLevitate = true;
		m_LevitatingTimeline.PlayFromStart();
	}


}
void UMagicComponent::levitateOverTime(float Value)
{

	if (m_ActorToLevitate)
	{
		if (m_ActorToLevitate->GetActorLocation().Z <= m_LevitatingLocation.Z && m_bCanLevitate)
		{
			//the movement is always in Z axis so (0,0,1).
			FVector Direction{ FVector(0.0f,0.0f,1.f) };
			m_ActorToLevitate->AddActorWorldOffset(Direction * GetWorld()->GetDeltaSeconds() * m_LevitateSpeed);

		}
		//	if the reason that the object is not moving up anymore is
		//  because it has reached the peak of levitation.
		else if (m_ActorToLevitate->GetActorLocation().Z >= m_LevitatingLocation.Z)
		{
			m_bReachedPeakPoint = true;
			m_PeakLocation = m_ActorToLevitate->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("ffhu"));
			m_LevitatingTimeline.Stop();
			//we don't want to call this functin anymore at this point, so we just stop the timeline
		}
	}
	//if we reach this else if it mean m_actorToLeavitate is null, and we set it to null when we are dropping it.
	// because we no longer need it.
	else if (!m_bCanLevitate)
	{

		m_LevitatingTimeline.Stop();
	}
}

//move the levitated obj up and down, when it has reached the destination.
//the implemented approach might not be the ideal way though,could change it later.
void UMagicComponent::jiggleLevitatedObject()
{
	if (m_ActorToLevitate)
	{

		FVector CurrentLocation{ m_ActorToLevitate->GetActorLocation() };
		//
		static FVector StartLocation{ m_PeakLocation };
		//
		static FVector Destination{ StartLocation + FVector(0.f, 0.f, m_JiggleHeight) };

		//not sure why had to define Direction as static but it wouldn't work otherwise.
		static FVector Direction{ 0.f,0.f,1.f };
		if (CurrentLocation.Z >= Destination.Z)
		{
			Direction.Z = -1.f;
		}
		else if (CurrentLocation.Z <= StartLocation.Z)
		{
			Direction.Z = 1.f;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Direction : %s"), *Direction.ToString());
		m_ActorToLevitate->AddActorWorldOffset(Direction * GetWorld()->GetDeltaSeconds() * m_JiggleMoveSpeed);
	}
	else UE_LOG(LogTemp, Warning, TEXT("m_ActorToLevitate is null (MagicComponent::JiggleLevitatedObject)"))



}

//

void UMagicComponent::dropObject()
{
	//we set it to false to stop lifting the object if we are lifting it(LiftOverTime() would have been getting called if we were lifting an object)
	//and that checks if m_bCanLevitate is true to keep lifting the object, else it stops.
	m_bCanLevitate = false;
	m_bReachedPeakPoint = false;

	if (m_ActorToLevitate && m_LevitatedActorMesh)
	{
		m_LevitatedActorMesh->SetSimulatePhysics(true);
		//no longer need to track these two vars.
		m_ActorToLevitate = nullptr;
		m_LevitatedActorMesh = nullptr;
	}

}
//END OF LEVITATING-SYSTEM.

// hides/appears a magic obj.
bool UMagicComponent::setActorVisibility(AMagicalActor* MagicalActor)
{
	if (!MagicalActor) return false;

	if (MagicalActor->m_bIsVisible)
	{
		MagicalActor->SetActorHiddenInGame(true);
		MagicalActor->m_bIsVisible = false;
	}
	else
	{
		MagicalActor->SetActorHiddenInGame(false);
		MagicalActor->m_bIsVisible = true;
	}
	return true;
}

bool UMagicComponent::MoveObject(AMagicalActor* MagicalActorToMove,const FVector& MoveDirection)
{
	if (!MagicalActorToMove) return false;




	return true;

}

