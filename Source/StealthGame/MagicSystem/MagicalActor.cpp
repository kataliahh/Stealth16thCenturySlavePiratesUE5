// Fill out your copyright notice in the Description page of Project Settings.
#include "MagicalActor.h"
#include "Components/StaticMeshComponent.h"
#include "StealthGame/Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AMagicalActor::AMagicalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

// Called when the game starts or when spawned
void AMagicalActor::BeginPlay()
{
	Super::BeginPlay();
	m_GlobalLevitationLocation = GetTransform().TransformPosition(m_LevitationLocation);
}
// Called every frame
void AMagicalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	levitate();
	Displace();

}

void AMagicalActor::levitate()
{
	if (m_bShouldLevitate)
	{

		if (GetActorLocation().Z <= m_GlobalLevitationLocation.Z && !m_bReachedPeakPoint)
		{
			FVector Direction{ 0.f,0.f,1.f };
			AddActorWorldOffset(Direction * GetWorld()->GetDeltaSeconds() * m_LevitationSpeed);
		}
		else
		{
			moveUpAndDown();
		}
	}

}
void AMagicalActor::moveUpAndDown()
{
	m_bReachedPeakPoint = true;
	//since the whole movement happens in Z direction we will only work with Z value in this function.
	static FVector Direction{ 0.f,0.f,1.f };
	static FVector Destination{ m_GlobalLevitationLocation.X,m_GlobalLevitationLocation.Y,m_GlobalLevitationLocation.Z + m_MaxHeight };
	FVector CurrentLocation{ GetActorLocation() };
	if (CurrentLocation.Z >= Destination.Z)
	{
		Direction = { 0.f,0.f,-1.f };
		UE_LOG(LogTemp, Warning, TEXT("-1 called"));
	}
	else if (CurrentLocation.Z <= m_GlobalLevitationLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("1 called"));
		Direction = { 0.f,0.f,1.f };
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Direction.ToString());
	AddActorWorldOffset(Direction * GetWorld()->GetDeltaSeconds() * m_SlowMovementSpeed);
}

void AMagicalActor::setActorVisibility()
{
	if (m_bIsVisible)
	{
		m_bIsVisible = false;
		SetActorHiddenInGame(true);
	}
	else
	{
		m_bIsVisible = true;
		SetActorHiddenInGame(false);
	}
}
void AMagicalActor::fall()
{
	if (m_StaticMeshComponent)
	{
		m_bShouldLevitate = false;
		m_bReachedPeakPoint = false;
		m_StaticMeshComponent->SetSimulatePhysics(true);
	}
}

void AMagicalActor::setMovementValues(const FVector& StartLocation, const FVector& MovementDirection, const FVector& Destination)
{
	//can use the following so everytime we call this function m_bShouldMove is reversed to toggle between moving and stopping.
	//m_bShouldMove = !m_bShouldMove;
	m_bShouldMove = true;
	m_StartLocation = StartLocation;
	m_MovementDirection = MovementDirection;
	m_Destination = Destination;
	
}
   /*
	move to another specified location if m_bshouldMove is set by the player(by an actionBinding call),
	the location and m_bshouldMove along with a few other params are all set in setMovementValues.
	*/
void AMagicalActor::Displace()
{
	if (m_bShouldMove)
	{
		double JourneyLength{ (m_Destination - m_StartLocation).Size() };
		double JourneyTravelled{ (GetActorLocation() - m_StartLocation).Size() };
		//UE_LOG(LogTemp, Warning, TEXT("%f"), JourneyLength);
		if (JourneyTravelled <= JourneyLength && (JourneyLength > MinimumJourneyLength) )
		{
			m_StaticMeshComponent->SetSimulatePhysics(false);
			AddActorWorldOffset(m_MovementDirection * GetWorld()->GetDeltaSeconds() * m_MovementSpeed);
		}
		/*if we reached the destination.
		else if(JourneyTravelled >= JourneyLength)
		{
			m_StaticMeshComponent->SetSimulatePhysics(true);
			m_bShouldMove = false;
		}
		
		for any other possibility just set m_bshouldmove to false.*/
		else
		{
			m_bShouldMove = false;
		}
	}
}