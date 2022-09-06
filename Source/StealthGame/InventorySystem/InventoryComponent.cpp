// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"
#include "StealthGame/Characters/CharacterBase.h"
#include "InventoryWidget.h"


	// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//we are using Capacity because it's by the default the maximum number of item we can have.
	m_ItemArr.Reserve(Capacity);
	// ...
}
// Called when the game starts.
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = Cast<ACharacterBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!PlayerChar) UE_LOG(LogTemp, Warning, TEXT("PlayerChar is null(InventoryComponent)"));
	ArrangeItems();
}
// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UInventoryComponent::RemoveItem(UItemBase* ItemToRemove)
{
	if (GEngine)	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Remove Called Successfully"));
	if (ItemToRemove)
	{
		/*if theIsEmpty is true then it means the item now,
		serves as a slot for items rather than one and we no longer need cleanups.*/
		if (ItemToRemove->GetIsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Is Empty!!!"))
			return true;
		}
		//if we have more than 1 item then subtract from the number of it.
		if (ItemToRemove->m_NumberOfItem > 1)
		{
			--ItemToRemove->m_NumberOfItem;
			UE_LOG(LogTemp,Warning,TEXT("Item amount reduced"))
			return true;
		}
		//if we no longer have any of the item left, clear the properties of the item and make it into a slot inventory.
		else
		{
			if (EmptySlotClass)
			{
				UItemBase* EmptySlot = NewObject<UItemBase>(this, EmptySlotClass);
				//make sure the emptyslot object is created.
				if (!EmptySlot) return false;
				//UE_LOG(LogTemp, Warning, TEXT("Empty Slot created Successfully"))
				// 
				//find the index of ItemRemove and then replace it with EmptySlot.
					int32 j = m_ItemArr.Find(ItemToRemove);
					if (j == INDEX_NONE)
					{
						UE_LOG(LogTemp, Error, TEXT(" the item was not found!"))
							return false;
					}
					m_ItemArr[j] = EmptySlot;
				UInventoryWidget* InventoryWidget{ PlayerChar->getInventoryWidget() };
				if (!InventoryWidget)
				{
					GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("InventoryWidget is null(InventoryComponent)"));
					return false;
				}
				/*since the inventory is on the screen while we are removing an item(using it or throwing it out)
				we need to update the inventory widget*/
				InventoryWidget->UpdateInventory.ExecuteIfBound();
				return true;
			}
			
			
			
		}
	}
	UE_LOG(LogTemp,Error,TEXT("ItemToRemove is null!"))
	//if the passed item is null then return false.
	return false;
}

bool UInventoryComponent::AddItem(UItemBase* ItemToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("AddItem Called"))

		//make sure The Item we wanna add to array is not null and the Inventory is not full either.
		if (ItemToAdd && m_ItemArr.Num() < Capacity)
		{
			if (GEngine)	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, TEXT("Item Added Successfully"));

			for (auto Item : m_ItemArr)
			{
				//if there is an item with the same name then it means the item already exists.
				if (ItemToAdd->GetName() == Item->GetName())
				{
					UE_LOG(LogTemp, Warning, TEXT("Added to the number of item"))
						//make sure the item we're tryna add is stackable.
						if (!Item->m_bStackable) return false;
					Item->AddTo_NumberOfItems();
					return true;
				}
			}
			//if the item doesn't already exist in the array we will pass its values to the first EmptySlot found in the inventory.
			for (int32 Index{ 0 };Index<m_ItemArr.Num();++Index)
			{
				if (m_ItemArr[Index]->GetIsEmpty())
				{
					UE_LOG(LogTemp, Warning, TEXT("Checking GetIsEmpty and it's true"))
					m_ItemArr[Index] = ItemToAdd;	
					UInventoryWidget* InventoryWidget{ PlayerChar->getInventoryWidget() };
					//if the Inventory Widget is on the screen, then update it right away.
					if (InventoryWidget)
					{
						InventoryWidget->UpdateInventory.ExecuteIfBound();

					}
					return true;
				}

			}
			/*if the inventory doesn't have that Item already,
			and we also don't have EmptySlot in the inventory, then return false */
			return false;
			/*	m_ItemArr.Add(ItemToAdd);
				ItemToAdd->SetOwningInventory(this);
				UInventoryWidget* InventoryWidget{ PlayerChar->GetInventoryWidget() };
				if (InventoryWidget)
				{
					InventoryWidget->UpdateInventory.ExecuteIfBound();
					return true;
				}
			*/


		}
	return false;

}

void UInventoryComponent::ArrangeItems()
{
	for (int32 Index{ 0 }; Index < m_ItemArr.Num(); ++Index)
	{
		m_ItemArr[Index]->SetOwningInventory(this);
		for (int32 NextIndex{ Index + 1 }; NextIndex < m_ItemArr.Num(); ++NextIndex)
		{
			m_ItemArr[NextIndex]->SetOwningInventory(this);
			// check to see if any items are similar or not.
			FString FirstItemName{ m_ItemArr[Index]->m_DisplayName };
			FString SecondItemName{ m_ItemArr[NextIndex]->m_DisplayName };

			if ((FirstItemName == SecondItemName))
			{
				/*check and see if the item is stackable, we only need to check one of them
				because if their names are the same it means they are the from same Item class & checking one suffices*/
				if (!m_ItemArr[Index]->m_bStackable) continue;
				m_ItemArr.RemoveAtSwap(NextIndex);
				m_ItemArr[Index]->AddTo_NumberOfItems();
				--NextIndex;
			}

		}
	}
}

