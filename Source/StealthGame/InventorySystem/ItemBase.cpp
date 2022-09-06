// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Engine/Texture2D.h"
UItemBase::UItemBase()
{
	m_NumberOfItem += 1;
}



UTexture2D* UItemBase::GetThumbnail() const
{
	return m_Thumbnail;
}

//void UItemBase::ClearAllProperties()
//{
//	m_Thumbnail = nullptr;
//	m_DisplayName.Empty();
//	m_ItemDescription = FText::FromString("");
//	m_NumberOfItem = 0;
//	m_bIsEmpty = true;
//	m_bStackable = false; //empty slots are not stackable.
//}
void UItemBase::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp,Warning,TEXT("An Item base destroyed(ItemBase)"))
}

//void UItemBase::CopyFrom(const UItemBase* Item)
//{
//	UE_LOG(LogTemp, Warning, TEXT("CopyFrom Called"))
//	m_DisplayName     =	Item->m_DisplayName;
//	m_ItemDescription = Item->m_ItemDescription;
//	m_bIsEmpty		  =	Item->m_bIsEmpty;
//	m_bStackable      =	Item->m_bStackable;
//	m_OwningInventory = Item->m_OwningInventory;
//	m_Thumbnail		  = Item->m_Thumbnail;
//	m_NumberOfItem    =	Item->m_NumberOfItem;
//	
//}
