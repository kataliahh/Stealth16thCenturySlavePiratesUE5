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
	void pullObject(AMagicalActor* MagicalActorToMove, const FVector& StartLocation, const FVector& MoveDirection, const FVector& Destination);

	void MoveObject();
	//
	//used in different magic systems so we don't place it under any system comments.
	AMagicalActor* lineTraceFromCamera(class ACharacterBase* PC);
protected:
	
	// Called when the game starts.
	virtual void BeginPlay() override;

	//LEVITATING-MAGIC PROPERTIES.

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		///used to see if there is an object to levitate within the range of player's viewpoint.
		float m_LineTraceRange{ 1000.f };

	UPROPERTY()
		//stores the current/last levitated actor.
		AMagicalActor* m_LevitatedActor {
		nullptr
	};
	class UStaticMeshComponent* m_LevitatedActorMesh{ nullptr };
	//this function is called through the LevitatingTimeline Delegate.

	//END OF LEVITATING PROPERTIES.


};
