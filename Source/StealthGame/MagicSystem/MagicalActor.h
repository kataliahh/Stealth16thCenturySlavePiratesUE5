// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicalActor.generated.h"

UCLASS()
class STEALTHGAME_API AMagicalActor : public AActor
{
	GENERATED_BODY()

		friend class UMagicComponent;
public:
	// Sets default values for this actor's properties
	AMagicalActor();

protected:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* m_StaticMeshComponent{ nullptr };
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//VISIBILITY.
	bool m_bIsVisible{ true };

	//END OF VISIBILITY.

	//LEVITATION.
	bool m_bShouldLevitate{ false };
	//
	bool m_bReachedPeakPoint{ false };
	//
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = Levitation)
		FVector m_LevitationLocation {};
	//m_LevitationLocation has a local value(relative to the actor),
	//so we use the variable below as the global location of and use it.
	FVector m_GlobalLevitationLocation{};

	UPROPERTY(EditDefaultsOnly, Category = Levitation)
		float m_LevitationSpeed{ 100.f };

	UPROPERTY(EditDefaultsOnly, Category = Levitation)
		//used to determine how up will the levitated obj move from the levitationLocation and come back.
		float m_MaxHeight{ 50.f };
	//
	UPROPERTY(EditDefaultsOnly, Category = Levitation)
		float m_SlowMovementSpeed{ 30.f };
	//levitates itself if m_bShouldLevitate is set to true by the player((by an actionBinding call)). 
	void levitate();

	//making slow movement of going up and down for a levitated object.
	void moveUpAndDown();

	//END OF LEVITATION.

	//MOVEMENT.
	bool m_bShouldMove{ false };
	//
	FVector m_StartLocation{};
	//
	FVector m_Destination{};
	//
	FVector m_MovementDirection{};
	//
	
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		//used as the speed to Move The Object.
		float m_MovementSpeed{ 100.f };

	UPROPERTY(EditDefaultsOnly, Category = Movement)
		//make sure that this value is at least as big as m_DestinationOffSet in CharacterBase.
		float MinimumJourneyLength{ 250.f };
	 
	//END OF MOVEMENT.


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//VISIBILITY.
	//
	void setActorVisibility();

	//END OF VISIBILITY.

	//LEVITATION.
	//
	void fall();

	///END OF LEVITATION.

	//MOVEMENT.
	//
	void setMovementValues(const FVector& StartLocation, const FVector& MoveDirection, const FVector& Destination);

	/*
	move to another specified location if m_bshouldMove is set by the player(by an actionBinding call),
	the location and m_bshouldMove along with a few other params are all set in setMovementValues.
	*/
	void Displace();
	// END OF MOVEMENT.


};