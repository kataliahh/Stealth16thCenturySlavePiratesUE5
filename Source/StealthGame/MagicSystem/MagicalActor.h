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

	UPROPERTY()
		class UMagicComponent* m_MagicComponent{ nullptr };
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
	//is called thrugh MagicComponent to set the values.
	void setMovementValues(const FVector& StartLocation, const FVector& MoveDirection, const FVector& Destination);

	//called when m_bShouldMove is set true(through MagicComponent by the player).
	void Displace();
	// END OF MOVEMENT.


};