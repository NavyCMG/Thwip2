// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterCharacter.h"
#include "BaseArcherCharacter.generated.h"


class AWeapon;
class UCombatComponents;
/**
 * 
 */
UCLASS()
class BLASTER_API ABaseArcherCharacter : public ABlasterCharacter
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeapon> Bow;

	virtual void BeginPlay() override;

};
