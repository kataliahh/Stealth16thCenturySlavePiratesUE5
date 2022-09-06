// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemBase.h"
#include "InventoryWidget.h"
#include "StealthGame/Characters/CharacterBase.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "InventoryComponent.h"
bool UItemWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseClass Returned False ItemWidget::Inilialize()"))
			return false;
	}
	return true;

}

bool UItemWidget::SetItem(UItemBase* InItem)
{
	Item = InItem;
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item initialized with null(ItemWidget)"))
			return false;
	}
	return true;
	//return (Item) ? true : false;
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply DragReply{ UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,TEXT("RightMouseButton")) };
	return DragReply.NativeReply;
}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("Dragged Detected!"))
	//
	UItemWidget* Dragged_Widget{ Cast<UItemWidget>(CreateWidget(this,ItemWidget_Class)) };
	//
	if (!Dragged_Widget) return;
	//
	UTexture2D* DraggedWidget_Thumbnail{ Item->GetThumbnail() };

	//if the widget we are trying to drag has no Thumbnail then it means it is a Slot Widget and we don't want to drag that.
	
	if (!DraggedWidget_Thumbnail) return;
	FSlateBrush ImageBrush{ UWidgetBlueprintLibrary::MakeBrushFromTexture(DraggedWidget_Thumbnail, this->GetSizeBox()->WidthOverride, this->GetSizeBox()->HeightOverride) };
	
	UDragDropOperation* InventoryOperation{
		UWidgetBlueprintLibrary::CreateDragDropOperation(nullptr)
	};
	if(InventoryOperation) OutOperation = InventoryOperation;

	Dragged_Widget->GetButton()->WidgetStyle.SetNormal(ImageBrush);
	
	
	
	if (!Item) { UE_LOG(LogTemp, Warning, TEXT("Item is null(ItemWidget)")) return; }

	// need to set the item in order to show the number of it while dragging.
	Dragged_Widget->SetItem(Item); 
	InventoryOperation->DefaultDragVisual = Dragged_Widget;
	InventoryOperation->Payload = this;
	
}

bool UItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UInventoryComponent* InventoryComp;
	UInventoryWidget* InventoryWidget;
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop Called(ItemWidget)"))
	ACharacterBase* PlayerChar = Cast<ACharacterBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!PlayerChar) { UE_LOG(LogTemp,Error, TEXT("PlayerChar is null(ItemWidget)")) return false; }
	InventoryComp = PlayerChar->getInventoryComponent();
	InventoryWidget = PlayerChar->getInventoryWidget();
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("OwningInventoryWidget is null(NativeOnDrop::ItemWidget)"))
		return false;
	}
	UItemWidget* WidgetPayload{ Cast<UItemWidget>(InOperation->Payload) };
	int32 First{InventoryComp->GetItems_Arr().Find(WidgetPayload->GetItem()) };
	int32 Second{ InventoryComp->GetItems_Arr().Find(this->Item) };
	if (First == Second)
	{
		UE_LOG(LogTemp,Warning,TEXT("Item was Dropped On itself :)(UItemWidget)"))
		return true;
	}

	InventoryComp->m_ItemArr.Swap(First, Second);
	InventoryWidget->UpdateInventory.ExecuteIfBound();
	return true;


}

void UItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	UE_LOG(LogTemp,Warning,TEXT("OnDragCancelledCalled!"))
}
