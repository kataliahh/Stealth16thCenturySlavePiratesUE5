// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 *
 */
UCLASS()
class STEALTHGAME_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	bool SetItem(class UItemBase* InItem );
	FORCEINLINE class UItemBase* GetItem() { return Item; }
	//FORCEINLINE class UImage* GetImage() { return Image; }
	FORCEINLINE class UButton* GetButton() { return Item_Button; }
	FORCEINLINE class USizeBox* GetSizeBox() { return SizeBox; };
	
protected:
	//
	//class AInventorySystemCharacter* PlayerChar{ nullptr };
	// 
	//need to create a widget of the bp version of ItemWidget,so we need this.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UItemWidget> ItemWidget_Class{ nullptr };
	//
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	//
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	//
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	//
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	//the real Item Stored in Inventory-Comp represented visually by this Widget.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UItemBase* Item;

	//UPROPERTY(meta = (BindWidget))
		//class UImage* Image;
	
	UPROPERTY(meta = (BindWidget))
		class UButton* Item_Button;
	//
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox;
};
