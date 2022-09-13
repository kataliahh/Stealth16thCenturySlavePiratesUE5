// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StealthGame/InventorySystem/InventoryComponent.h"
#include "StealthGame/InventorySystem/InventoryWidget.h"
#include "StealthGame/MagicSystem/MagicComponent.h"
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
}
//this function is responsible to call functions of m_MagicComponent that are related to lifting.
void ACharacterBase::startLiftingProcess()
{
	if (m_MagicComponent)
	{
		AActor* ActorToLevitate{ m_MagicComponent->findObjectToLevitate(this) };
		if (ActorToLevitate)
		{
				FVector LevitatingDirection{ ActorToLevitate->GetActorUpVector() };
				FVector LevitatingLocation{ ActorToLevitate->GetActorLocation() + (LevitatingDirection * m_LiftHeight) };
				m_MagicComponent->setLevitatingValues(ActorToLevitate, LevitatingLocation);
		}

	}
	else
		UE_LOG(LogTemp, Warning, TEXT("m_MagicComponent is null(CharacterBase::callfindObjectToLevitate)"));
}
void ACharacterBase::callDropObject()
{
	if (m_MagicComponent)
	{
		m_MagicComponent->dropObject();
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



