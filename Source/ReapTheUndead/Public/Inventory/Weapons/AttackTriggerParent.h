// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackTriggerParent.generated.h"

UCLASS()
class REAPTHEUNDEAD_API AAttackTriggerParent : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttackTriggerParent();

protected:
	virtual void BeginPlay() override;

};
