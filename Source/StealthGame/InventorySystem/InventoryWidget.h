// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

DECLARE_DELEGATE(FOnInventoryUpdated);
UCLASS()
class STEALTHGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnInventoryUpdated UpdateInventory;

	virtual bool Initialize() override;
	TArray<class UItemWidget*> GetItemWidget_Arr();
protected:
	
	UPROPERTY()
		class ACharacterBase* PlayerChar{
		nullptr
	};
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	//bool bInventoryUpdated{ false };
	UFUNCTION()
		void UpdateInventoryWidget();
	UPROPERTY()
	TArray<class UItemWidget*> ItemWidget_Arr;
	//we will use this variable to set how big the itemWidget will be in InventoryWidget.
	UPROPERTY(EditDefaultsOnly)
		float m_ItemSizeOverride{ 150.f };
private:
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WrapBox;
	//
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UItemWidget> ItemWidgetClass;
	//
	void CreateInventoryWidget();


};
