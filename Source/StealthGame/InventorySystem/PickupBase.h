// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class STEALTHGAME_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupBase();
	FORCEINLINE TSubclassOf<class UItemBase> GetItemClass() { return ItemClass; }

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp{ nullptr };
	//
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollision{ nullptr };

	//used to create an object of the UItemBase and then add it to the inventory mostly.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UItemBase> ItemClass{ nullptr };
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
