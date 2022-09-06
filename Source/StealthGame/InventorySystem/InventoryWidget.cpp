// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ItemWidget.h"
#include "ItemBase.h"
#include "InventoryComponent.h"
#include"Components/Image.h"
#include"Components/WrapBox.h"
#include"Components/SizeBox.h"
#include "StealthGame/Characters/CharacterBase.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


bool UInventoryWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseClass Returned False InventoryWidget::Inilialize()"))
			return false;
	}
	PlayerChar = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CreateInventoryWidget();
	if (PlayerChar) PlayerChar->setInventoryWidget(this);

	UpdateInventory.BindUFunction(this, TEXT("UpdateInventoryWidget"));

	return true;

}
void UInventoryWidget::CreateInventoryWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateInventoryWidget Called (UInventoryWidget)"));
	if (WrapBox)
		WrapBox->ClearChildren();
	else UE_LOG(LogTemp, Warning, TEXT("ersersersers (UInventoryWidget)"));
	//

	if (PlayerChar)
	{
		UInventoryComponent* InventoryComp{ PlayerChar->getInventoryComponent() };
		if (!InventoryComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory is null(InventoryWidget::CreateInventoryWidget)"));
			return;
		}
		if (!ItemWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("ItemWidgetClass is null(InventoryWidget::CreateInventoryWidget)"));
			return;
		}
		ItemWidget_Arr.Reset(InventoryComp->GetItems_Arr().Num());
		//
		TArray<UItemBase*> m_ItemArr{ InventoryComp->GetItems_Arr() };
		//ItemWidgetIndex_InWrapBox.Reserve(m_ItemArr.Num());
		for (int32 Index{ 0 }; Index < m_ItemArr.Num(); ++Index)
		{
			if (!m_ItemArr[Index])
				UE_LOG(LogTemp, Warning, TEXT("Item is null(InventoryWidget)"));

			ItemWidget_Arr.Add(CreateWidget<UItemWidget>(this, ItemWidgetClass));
			ItemWidget_Arr[Index]->GetSizeBox()->SetWidthOverride(m_ItemSizeOverride);
			ItemWidget_Arr[Index]->GetSizeBox()->SetHeightOverride(m_ItemSizeOverride);
			ItemWidget_Arr[Index]->SetItem(m_ItemArr[Index]);

			FSlateBrush ImageBrush{
				UWidgetBlueprintLibrary::MakeBrushFromTexture
				(
				ItemWidget_Arr[Index]->GetItem()->GetThumbnail(),
				m_ItemSizeOverride,
				m_ItemSizeOverride) };
			//setting the thumbnail of the item in different situations(normal, pressed, hovered..).
			ItemWidget_Arr[Index]->GetButton()->WidgetStyle.SetNormal(ImageBrush);
			ItemWidget_Arr[Index]->GetButton()->WidgetStyle.SetHovered(ImageBrush);
			ItemWidget_Arr[Index]->GetButton()->WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(255, 255, 255, .2));
			ItemWidget_Arr[Index]->GetButton()->WidgetStyle.SetPressed(ImageBrush);

			WrapBox->AddChildToWrapBox(ItemWidget_Arr[Index]);

		}

	}
	else UE_LOG(LogTemp, Warning, TEXT("Player is null(InventoryWidget)"));
}

TArray<class UItemWidget*> UInventoryWidget::GetItemWidget_Arr()
{
	return ItemWidget_Arr;
}
bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	InDragDropEvent.GetScreenSpacePosition();
	bool bDroppedOnInventory{ WrapBox->GetCachedGeometry().IsUnderLocation(InDragDropEvent.GetScreenSpacePosition()) };
	if (bDroppedOnInventory) UE_LOG(LogTemp, Warning, TEXT("Droppeghuiuihuihuid on Inventory(InventoryWidget)"))
	else
	{
		if (PlayerChar)
		{
			UItemWidget* ItemWidget{ Cast<UItemWidget>(InOperation->Payload) };
			PlayerChar->getInventoryComponent()->RemoveItem(ItemWidget->GetItem());
			UE_LOG(LogTemp, Warning, TEXT("Dropped outside the inventory!!(InventoryWidget)"))
		}
	}
	return true;
}
void UInventoryWidget::UpdateInventoryWidget()
{
	//bInventoryUpdated = true;
	CreateInventoryWidget();

}
