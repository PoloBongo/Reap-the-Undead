// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SlotButtonInventory.generated.h"

/**
 * 
 */
UCLASS()
class REAPTHEUNDEAD_API USlotButtonInventory : public UButton
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reference", meta=(AllowPrivateAccess="true"))
	int ButtonId;
};
