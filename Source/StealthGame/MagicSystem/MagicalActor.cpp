// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicalActor.h"
#include "Components/StaticMeshComponent.h"
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
	
}

// Called every frame
void AMagicalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

