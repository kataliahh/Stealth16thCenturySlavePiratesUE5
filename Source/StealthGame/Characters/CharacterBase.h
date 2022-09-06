// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class STEALTHGAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Getters and setters.
	 void setInventoryWidget(class UInventoryWidget* InventoryWidget);
	 class UInventoryWidget* getInventoryWidget();
	 //

	 class UInventoryComponent* getInventoryComponent();



protected:
	
	//Components
	// 3rd person SpringArm
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
		class UInventoryComponent* m_InventoryComponent{ nullptr };
	//end of components.

	UPROPERTY()
		class UInventoryWidget* m_InventoryWidget{ nullptr };

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
