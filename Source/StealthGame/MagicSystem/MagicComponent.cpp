// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGame/MagicSystem/MagicComponent.h"
#include "StealthGame/Characters/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
	FOnTimelineFloat LiftingDelegate;
	LiftingDelegate.BindUFunction(this, FName("LevitateOverTime"));
	m_LevitatingTimeline.AddInterpFloat(m_liftingCurveFloat, LiftingDelegate);
	// ...

}

// Called every frame
void UMagicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("MagicComponent Ticking Successfully"));
	m_LevitatingTimeline.TickTimeline(DeltaTime);
	//if (m_ActorToLevitate) UE_LOG(LogTemp, Warning, TEXT("%s"), *m_LevitatingLocation.ToString())

	// ...
}
//this function lineTraces through CameraViewPoint and see if we hit something to lift.
AActor* UMagicComponent::findObjectToLevitate(ACharacterBase* PC)
{
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerChar is null(UMagicComponent::findObjectToLevitate)"));
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
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		bool bLineTraceHitSomething{ World->LineTraceSingleByObjectType(HitRes, StartLocation, EndLocation, ObjectQueryParams, CollisionQueryParams) };

		if (bLineTraceHitSomething)
		{
			return HitRes.GetActor();
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("World is null(UMagicComponent::findObjectToLevitate)"));
	return nullptr;
}
void UMagicComponent::setLevitatingValues(AActor* ActorToLevitate, const FVector& LiftLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Lifting Actor Successfully(UMagicComponent::liftActor)"));
	if (m_liftingCurveFloat)
	{
		m_LevitatingLocation = LiftLocation;
		m_ActorToLevitate = ActorToLevitate;
		if (m_ActorToLevitate)
		{
			if (UStaticMeshComponent * LiftedActorMesh{ m_ActorToLevitate->FindComponentByClass<UStaticMeshComponent>() })
			{
				LiftedActorMesh->SetSimulatePhysics(false);
			}
		}
		//
		m_bCanLevitate = true;
		m_LevitatingTimeline.PlayFromStart();
	}


}
void UMagicComponent::LevitateOverTime(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("LiftOverTime Called %i"), Value);
	if (m_ActorToLevitate)
	{
		if (m_ActorToLevitate->GetActorLocation().Z <= m_LevitatingLocation.Z && m_bCanLevitate)
		{
			UE_LOG(LogTemp, Warning, TEXT("%f"), m_LevitatingLocation.Z);
			FVector Direction{ m_ActorToLevitate->GetActorUpVector()};
			m_ActorToLevitate->AddActorLocalOffset(Direction * GetWorld()->GetDeltaSeconds() * m_LevitateSpeed);
		}
		else
		{
			//when the actor reached the desired Location we no longer need the Timeline to work.
			m_LevitatingTimeline.Stop();
		}
	}
	//else UE_LOG(LogTemp, Error, TEXT("m_ActorToLevitate is null(MagicComponent::LiftOverTime)"))
}

void UMagicComponent::dropObject()
{
	//we set it to false to stop lifting the object if we are lifting it(LiftOverTime() would have been getting called if we were lifting an object)
	//and that checks if m_bCanLevitate is true to keep lifting the object, else it stops.
	m_bCanLevitate = false;

		if (m_ActorToLevitate)
		{
			UStaticMeshComponent* LiftedActorMesh{ m_ActorToLevitate->FindComponentByClass<UStaticMeshComponent>() };
			LiftedActorMesh->SetSimulatePhysics(true);
			m_ActorToLevitate = nullptr; //no longer need to track it.
			
		}
	
}



