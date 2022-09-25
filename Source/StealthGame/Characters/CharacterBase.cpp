// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StealthGame/InventorySystem/InventoryComponent.h"
#include "StealthGame/InventorySystem/InventoryWidget.h"
#include "StealthGame/MagicSystem/MagicComponent.h"
#include "StealthGame/MagicSystem/MagicalActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("m_SpringArmComp");
	m_SpringArmComp->SetupAttachment(RootComponent);
	m_SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	m_CameraComp = CreateDefaultSubobject<UCameraComponent>("m_CameraComp");
	m_CameraComp->SetupAttachment(m_SpringArmComp);
	m_CameraComp->bUsePawnControlRotation = false;
	m_InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	//
	m_MagicComponent = CreateDefaultSubobject<UMagicComponent>(TEXT("MagicComponent"));
	//

}


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward/Backward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("Move Right/Left", this, &ThisClass::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right/Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up/Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("Lift/Drop"), IE_Pressed, this, &ThisClass::startLiftingProcess);
	PlayerInputComponent->BindAction(TEXT("Lift/Drop"), IE_Released, this, &ThisClass::callDropObject);
	PlayerInputComponent->BindAction(TEXT("Vanish/Appear"), IE_Pressed, this, &ThisClass::callSetActorVisibility);
	PlayerInputComponent->BindAction(TEXT("Pull"), IE_Pressed, this, &ThisClass::callPullObject);
	PlayerInputComponent->BindAction(TEXT("Push"), IE_Pressed, this, &ThisClass::setPushingValues);

}
//this function is responsible to call functions of m_MagicComponent that are related to lifting.
void ACharacterBase::startLiftingProcess()
{
	if (m_MagicComponent)
	{
		AMagicalActor* ActorToLevitate{ m_MagicComponent->lineTraceFromCamera(this) };
		if (ActorToLevitate)
		{
			//FVector LevitatingDirection{ FVector(0.f,0.f,1.f) };
			////TODO:possibly change m_LevitationHeight to a fvector that can be altered in UE viewport by exposing it in BP.
			////the FVector ofc needs to be the property the actor we wanna levitate.
			//FVector LevitatingLocation{ ActorToLevitate->GetActorLocation() + (LevitatingDirection * m_LevitationHeight) };
			m_MagicComponent->setLevitatingValues(ActorToLevitate);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("m_MagicComponent is null(CharacterBase::calllineTraceFromCamera)"));
}
void ACharacterBase::callDropObject()
{
	if (m_MagicComponent)
	{
		m_MagicComponent->dropObject();
	}
}
void ACharacterBase::callSetActorVisibility()
{
	if (!m_MagicComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicComponent is null(ACharacterBase::callSetActorVisibility)"));
		return;
	}
	AMagicalActor* MagicalActor{ m_MagicComponent->lineTraceFromCamera(this) };

	m_MagicComponent->setActorVisibility(MagicalActor);
}
void ACharacterBase::callPullObject()
{
	if (!m_MagicComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicalActor is null(ACharacterBase::callMoveObject)"));
		return;
	}
	AMagicalActor* MagicalActorToMove = m_MagicComponent->lineTraceFromCamera(this);
	if (MagicalActorToMove)
	{
		FVector PullDestination{ GetActorLocation() + (GetActorForwardVector() * m_DestinationOffSet) };
		FVector Direction{ (PullDestination - MagicalActorToMove->GetActorLocation()).GetSafeNormal() };
		FVector StartLocation{ MagicalActorToMove->GetActorLocation() };
		//we want the actor to move in a straight line so we set the Z to 0, if the Z value of it is less than 40.
		//while it sits on the ground it's 50 so that is why we are using the below magic numbers to see if it's between these two values. 
		//TODO:make sure to change the approach to handle this, it's just for testing rn.
		if (StartLocation.Z >= 40.f && StartLocation.Z <= 52.f)
			Direction.Z = 0.f;
		//DrawDebugLine(GetWorld(), MagicalActorToMove->GetActorLocation(), PullDestination, FColor::Red, false, 5.f, 0.f, 10.f);
		//UE_LOG(LogTemp, Warning, TEXT("Direction = %s s"), *Direction.ToString());
		m_MagicComponent->pullObject(MagicalActorToMove, StartLocation, Direction, PullDestination);
	}
}
void ACharacterBase::setPushingValues()
{
	if (!m_MagicComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicalActor is null(ACharacterBase::callMoveObject)"));
		return;
	}
	AMagicalActor* MagicalActorToMove = m_MagicComponent->lineTraceFromCamera(this);
	if (MagicalActorToMove)
	{
		//TODO: have to change the way we handle the direction and stuff later, not the best way right now.

		FVector MagicalActorLocation{ MagicalActorToMove->GetActorLocation() };
		//gives us the location of char with the direction of where the player faces.
		FVector CharLocationOffset{ getCameraComponent()->GetComponentLocation() + GetActorForwardVector()};
		FVector Direction{ (MagicalActorLocation - CharLocationOffset).GetSafeNormal() };
		FVector Destination{ MagicalActorLocation + (Direction * m_PushDistance) };
		Direction.Z = 0.f;
		//DrawDebugLine(GetWorld(), GetActorLocation(), d, FColor::Red, false, 5.f, 0.f, 10.f);

		m_MagicComponent->pullObject(MagicalActorToMove, MagicalActorLocation, Direction, Destination);
	}
}
void ACharacterBase::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void ACharacterBase::setInventoryWidget(UInventoryWidget* InventoryWidget)
{
	if (InventoryWidget)
	{
		m_InventoryWidget = InventoryWidget;
	}
}
UInventoryWidget* ACharacterBase::getInventoryWidget()
{
	return m_InventoryWidget;
}
UInventoryComponent* ACharacterBase::getInventoryComponent()
{
	return m_InventoryComponent;
}
UCameraComponent* ACharacterBase::getCameraComponent()
{
	return m_CameraComp;
}



