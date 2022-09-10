// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MagicComponent.generated.h"


struct FTimeline;
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
	void setLevitatingValues(AActor* ActorToLevitate, const FVector& LiftLocation);
	AActor* findObjectToLevitate(class ACharacterBase* PC);

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		///used to see if there is an object to levitate within the range of player's viewpoint.
		float m_LineTraceRange{ 1000.f };

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		class UCurveFloat* m_liftingCurveFloat{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = LevitatingMagic)
		float m_LevitateSpeed{ 100.f };

	bool m_bCanLevitate{ false };

	FTimeline m_LevitatingTimeline;
	FVector m_LevitatingLocation{};
	
	//UPROPERTY()
		AActor* m_ActorToLevitate;

	//this function is called through the LiftingTimeline Delegate.
	UFUNCTION()
		void LevitateOverTime(float Value);

	void dropObject();




};
