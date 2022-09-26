// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MagicComponent.generated.h"

class AMagicalActor;
//this component is used to implement any kind of magic ability that can be cast.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STEALTHGAME_API UMagicComponent : public UActorComponent
{
	GENERATED_BODY()
		friend class ACharacterBase;
		friend class AMagicalActor;
public:
	// Sets default values for this component's properties.
	UMagicComponent();

	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//LEVITATING-SYSTEM FUNCTIONS.
	void setLevitatingValues(AMagicalActor* ActorToLevitate);
	
	//calls Fall() on the LevitatedActor.
	void dropObject();
	//END OF LEVITATING-SYSTEM FUNCTIONS.


	//VANISHING-SYSTEM FUNCTIONS.

	// calls setActorVisibiilty on the magic obj.
	void setActorVisibility(AMagicalActor* MagicalActor);

	//VANISHING-SYSTEM FUNCTIONS.

	//this function is used to pull/push objects.
	void callSetMovementValues(AMagicalActor* MagicalActorToMove, const FVector& StartLocation, const FVector& MoveDirection, const FVector& Destination);

	void MoveObject();
	//
	//used in different magic systems so we don't place it under any system comments.
	AMagicalActor* lineTraceFromCamera(class ACharacterBase* PC);
protected:
	
	// Called when the game starts.
	virtual void BeginPlay() override;

	// **PULL/PUSH PROPERTIES** //.
	
	//used to determine how far will the MagicalActor be pushed away from its current location.
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float m_PushDistance{ 500.f };

	//this variable is used to determine how far will the destination of the pulled MagicalActor be from the player.
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float m_DestinationOffSet{ 100.f };

	UPROPERTY(EditDefaultsOnly, Category = Movement)
		//make sure that this value is at least as big as m_DestinationOffSet.
		float MinimumJourneyLength{ 250.f };

	//used as the speed to pull/push MagicalActors.
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float m_MovementSpeed{ 800.f };
	
	// **END OF PULL/PUSH PROPERTIES** //.
	

	// **LEVITATING-MAGIC PROPERTIES** //.

	//is multiplied by the upVector of the actor we wanna lift.
	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		float m_LevitationHeight{ 200.f };

	UPROPERTY(EditDefaultsOnly, Category = Levitation)
		float m_LevitationSpeed{ 100.f };

	//used to determine how up will the levitated obj move from the levitationLocation and come back.
	UPROPERTY(EditDefaultsOnly, Category = Levitation)
		float m_MaxHeight{ 50.f };
	
	//this is the speed of the levitated Obj when it's doing a slow movement of going up and down after reaching the LevitationLocation.
	UPROPERTY(EditDefaultsOnly, Category = Levitation)
		float m_SlowMovementSpeed{ 30.f };
	
	///used to see if there is an object to levitate within the range of player's viewpoint.
	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		float m_LineTraceRange{ 1000.f };

	//stores the current/last levitated actor.
	UPROPERTY()
		AMagicalActor* m_LevitatedActor {
		nullptr
	};

	//need this to turn on/off physics on the LevitatedActor.
	class UStaticMeshComponent* m_LevitatedActorMesh{ nullptr };

	// **END OF LEVITATING PROPERTIES** //.


};
