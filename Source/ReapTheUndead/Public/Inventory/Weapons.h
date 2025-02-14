// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "Weapons.generated.h"

class AAttackTriggerParent;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKameaAttack); 
UCLASS()
class REAPTHEUNDEAD_API AWeapons : public AItem
{
	GENERATED_BODY()
public:
	AWeapons();
	virtual void UseItem() override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property", meta=(AllowPrivateAccess="true"))
	AAttackTriggerParent* AttackTriggerParent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FKameaAttack OnPlayAttackKamea;
};
