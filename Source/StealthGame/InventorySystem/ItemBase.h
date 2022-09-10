// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 *
 */
class UInventoryComponent;
class UTexture2D;


UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInLineNew)
class STEALTHGAME_API UItemBase : public UObject
{
	GENERATED_BODY()
		friend class UInventoryComponent;
public:
	//ctor.
	UItemBase();
	//
		//part of the "Token of Stupidity"
	/*FORCEINLINE  TEnumAsByte<ItemType> GetItemType() const { return ItemType; }*/
	//
	FORCEINLINE void AddTo_NumberOfItems(int32 Number = 1) { m_NumberOfItem += Number; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UseItem(); //PURE_VIRTUAL(UItemBase::UseItem, );
	FORCEINLINE void SetOwningInventory(UInventoryComponent* OwningInventory) { m_OwningInventory = OwningInventory; }
	//
	UFUNCTION(BlueprintCallable)
		FORCEINLINE   UInventoryComponent* GetOwningInventory() const { return m_OwningInventory; }
	//FORCEINLINE void shit(){ UE_LOG(LogTemp, Warning, TEXT("shit")) }
	FORCEINLINE bool GetIsEmpty() const { return m_bIsEmpty; }
	//
	FORCEINLINE void SetIsEmpty(bool bIsEmpty) { m_bIsEmpty = bIsEmpty; }
	//
	FORCEINLINE FString GetName() const { return m_DisplayName; }
	//
	class UTexture2D* GetThumbnail() const;

	/*when we want to convert a slot with item to an
	empty slot we will call this(usually instead of removing the item completely).*/
	
	//void ClearAllProperties();
	/*UItemBase& operator= (const UItemBase& Item);*/
	virtual void BeginDestroy();
	
	//void CopyFrom(const UItemBase* Item);
protected:
	//used to represent the Item as empty with no properties(a slot that can filled with item).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool m_bIsEmpty{ false };
	//
	//Items are stackable by default but some of them may not be so we will use this var to controll it.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool m_bStackable{ true };

	UPROPERTY()
		UInventoryComponent* m_OwningInventory {
		nullptr
	};
	//
		//part of the "Token of Stupidity"
		/*UPROPERTY(EditDefaultsOnly)
			TEnumAsByte<ItemType> ItemType{};*/

	UPROPERTY(EditDefaultsOnly)
		FString m_DisplayName;

	UPROPERTY(EditDefaultsOnly)
		FText m_ItemDescription;

	UPROPERTY(EditDefaultsOnly)
		UTexture2D* m_Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 m_NumberOfItem {};

};
