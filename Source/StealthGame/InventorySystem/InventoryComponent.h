// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STEALTHGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
		friend class UItemWidget;
public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
		bool RemoveItem(UItemBase* ItemToRemove);
	//
	UFUNCTION(BlueprintCallable)
		bool AddItem(UItemBase* ItemToAdd);
	//
	FORCEINLINE const TArray<UItemBase*>& GetItems_Arr() { return m_ItemArr; }
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UItemBase> EmptySlotClass{ nullptr };
	void ArrangeItems();
	UPROPERTY()
		class ACharacterBase* PlayerChar{ nullptr };
	UPROPERTY(EditDefaultsOnly, instanced)
		TArray<UItemBase*> m_ItemArr;

	//number of items we can carry.
	int32 Capacity{ 40 };

	// Called when the game starts.
	virtual void BeginPlay() override;

public:
	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
