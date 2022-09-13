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
	// Sets default values for this component's properties
	UMagicComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//LEVITATING-SYSTEM FUNCTIONS.
	void setLevitatingValues(AMagicalActor* ActorToLevitate, const FVector& LiftLocation);
	
	//END OF LEVITATING-SYSTEM FUNCTIONS.


	//VANISHING-SYSTEM FUNCTIONS.

	//used to vanish/appear objects.
	bool setActorVisibility(AMagicalActor* MagicalActor);

	//VANISHING-SYSTEM FUNCTIONS.

	//used in different magic systems so we don't place it under any system comments.
	AMagicalActor* lineTraceFromCamera(class ACharacterBase* PC);
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	//LEVITATING-MAGIC PROPERTIES.

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		///used to see if there is an object to levitate within the range of player's viewpoint.
		float m_LineTraceRange{ 1000.f };

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		class UCurveFloat* m_liftingCurveFloat{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		float m_LevitateSpeed{ 100.f };

	bool m_bCanLevitate{ false };
	//used to check if the levitated obj has reached the top point of levitation, so we can add jiggling movement to it.
	bool m_bReachedPeakPoint{ false };

	FTimeline m_LevitatingTimeline;

	FVector m_LevitatingLocation{};

	FVector m_PeakLocation{};

	UPROPERTY(EditDefaultsOnly)
		float m_JiggleHeight{ 1500.f };

	UPROPERTY(EditDefaultsOnly)
		float m_JiggleMoveSpeed{ 5.f };

	UPROPERTY()
	AActor* m_ActorToLevitate{ nullptr };

	class UStaticMeshComponent* m_LevitatedActorMesh{ nullptr };
	//this function is called through the LevitatingTimeline Delegate.
	UFUNCTION()
		void levitateOverTime(float Value);

	void jiggleLevitatedObject();

	void dropObject();
	//END OF LEVITATING PROPERTIES.


	





};
