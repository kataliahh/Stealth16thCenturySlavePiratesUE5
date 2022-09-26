// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class ACharacterBase : public ACharacter
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
	//
	class UInventoryWidget* getInventoryWidget();
	//

	class UInventoryComponent* getInventoryComponent();
	//
	class UCameraComponent* getCameraComponent();

	FORCEINLINE class UMagicComponent* getMagicComponent() { return m_MagicComponent; }

	
	


protected:
	
	
	//Components
	// 
	// 3rd person SpringArm
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* m_SpringArmComp;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* m_CameraComp;

	UPROPERTY(VisibleAnywhere)
		class UInventoryComponent* m_InventoryComponent{ nullptr };

	UPROPERTY(VisibleAnywhere)
		class UMagicComponent* m_MagicComponent{ nullptr };
	//end of components.

	UPROPERTY()
		class UInventoryWidget* m_InventoryWidget{ nullptr };

	//Bindings
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// THE FOLLOWING FUNCTIONS ARE USED TO CALL MAGIC-COMPONENT FUNCTION.
	//
	void startLiftingProcess();

	void callDropObject();

	void callSetActorVisibility();

	void callcallSetMovementValues();

	void setPushingValues();
	//END OF MAGIC-COMPONENT-FUNCTION CALLERS.

	//end of binidings.


};
