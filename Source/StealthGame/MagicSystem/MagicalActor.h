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

	bool m_bIsVisible{ true };
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
