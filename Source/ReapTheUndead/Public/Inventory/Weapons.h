// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "Weapons.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API AWeapons : public AItem
{
	GENERATED_BODY()
public:
	AWeapons();
	virtual void UseItem() override;
	
protected:
	virtual void BeginPlay() override;
};
