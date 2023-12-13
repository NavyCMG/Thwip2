// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseArcherCharacter.h"
#include "Blaster/BlasterComponents/CombatComponents.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Weapon/Bow.h"


void ABaseArcherCharacter::BeginPlay()
{
	Super::BeginPlay();

	//AWeapon* EquipingWeapon = Cast<AWeapon>(EquipedBow);
	/*SetOverlappingWeapon(EquipedBow);
	EquipButtonPressed();*/
	//EquipedBow = Cast<AWeapon*>(Bow);
	//Combat->EquipWeapon(EquipedBow);



	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Instigator = Cast<APawn>(this);

		//this->GetActorLocation()

		// Spawn the projectile at the muzzle
		AWeapon* spawned = World->SpawnActor<AWeapon>(Bow, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
		
		SetOverlappingWeapon(spawned);
		EquipButtonPressed();
	}
}
