// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blaster/Weapon//Casing.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Blaster/HUD/CharacterOverlay.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	// We are setting the AreaSphere to Ignore since we want the server to know these things only
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// This will make sure that the clients will have it to no collision but we are setting this to have collision on begin play only on the server
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	//create timeline
	ActiveReloadTL = CreateDefaultSubobject<UTimelineComponent>(TEXT("ActiveReloadTL"));

	

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// One way to see which is the server  GetLocalRole() == ENetRole::ROLE_Authority
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	if (ActiveReloadCurve)
	{
		//create and bind an event/ function to the timeline start
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("TimeLineStart"));

		ActiveReloadTL->AddInterpFloat(ActiveReloadCurve, ProgressUpdate); //Ties Timeline float to normal float

		//create and bind an event/ function to the timeline ending
		FOnTimelineEvent ReloadEnd;
		ReloadEnd.BindUFunction(this, FName("ActiveReloadFinished"));
		ActiveReloadTL->SetTimelineFinishedFunc(ReloadEnd);
	}

}

void AWeapon::ActiveReload()
{
	UE_LOG(LogTemp, Error, TEXT("Active Reload Function Called"));
	if (ArrowNocked == false)
	{
		if (IsReloading == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Reload Start"));
			IsReloading = true;
			FirstTry = true;
			ActiveReloadTL->PlayFromStart();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Is Reloading"));
			if (ReloadProgress > ActiveReloadZoneBegin && ReloadProgress < ActiveReloadZoneEnd && FirstTry == true)
			{
				ActiveReloadTL->Stop();
				IsReloading = false;
				ArrowNocked = true;
				UE_LOG(LogTemp, Error, TEXT("Successful Active Reload"));
			}
			else
			{
				FirstTry = false;
				UE_LOG(LogTemp, Error, TEXT("Failed Active Reload"));
			}
		}
	}
}

void AWeapon::ActiveReloadFinished()
{
	IsReloading = false;
	ArrowNocked = true;
	UE_LOG(LogTemp, Error, TEXT("Reload Finished"));
}

void AWeapon::TimeLineStart(float Progress)
{
	ReloadProgress = Progress;
	UE_LOG(LogTemp, Error, TEXT("Progress: %f"), Progress);
}


void AWeapon::SetActiveReloadHUDVisibility()
{
	if (BlasterPlayerController)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(BlasterPlayerController->GetHUD()) : BlasterHUD;
		if (IsReloading == true)
		{
			BlasterHUD->CharacterOverlay->ActiveReloadSlider->SetVisibility(ESlateVisibility::Visible);
			BlasterHUD->CharacterOverlay->ActiveReloadZoneImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			BlasterHUD->CharacterOverlay->ActiveReloadSlider->SetVisibility(ESlateVisibility::Hidden);
			BlasterHUD->CharacterOverlay->ActiveReloadZoneImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AWeapon::SetActiveReloadHUDProgress()
{
	if (BlasterPlayerController)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(BlasterPlayerController->GetHUD()) : BlasterHUD;
		BlasterHUD->CharacterOverlay->ActiveReloadSlider->SetValue(ReloadProgress);
	}
}

void AWeapon::SetArrowNockedHUDVisibility()
{
	if (BlasterPlayerController)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(BlasterPlayerController->GetHUD()) : BlasterHUD;
		if (ArrowNocked == true)
			BlasterHUD->CharacterOverlay->ArrowNockedImage->SetVisibility(ESlateVisibility::Visible);
		else
			BlasterHUD->CharacterOverlay->ArrowNockedImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActiveReloadHUDVisibility();
	SetActiveReloadHUDProgress();
	SetArrowNockedHUDVisibility();
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME(AWeapon, Ammo);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// We want to cast the OtherActor to be our blasterCharacter since that's the actor that we want to have the widget seen once the character is near the areasphere.
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::OnRep_Ammo()
{

}

void AWeapon::SpendRound()
{
	--Ammo;

}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//OwnerInstigator = GetInstigator();
		//OwnerController = OwnerInstigator->GetController();
		//OwnerController;
		//OwnerController = GetInstigator()->GetController();
		//BlasterPlayerController = Cast<ABlasterPlayerController>(GetInstigatorController());
		//OwnerController = Owner->GetNetOwningPlayer()->GetPlayerController(GetWorld());
		//BlasterPlayerController = Cast<ABlasterPlayerController>(OwnerController);
		//BlasterPlayerController = Cast<ABlasterPlayerController>(GetNetOwningPlayer()->GetPlayerController(GetWorld()));
		//BlasterPlayerController = Cast<ABlasterPlayerController>(OwnerController);
		//BlasterHUD = Cast<ABlasterHUD>(BlasterPlayerController->GetHUD());
		//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

		break;
	case EWeaponState::EWS_Dropped:
		if (HasAuthority())
		{
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	// If Casing is valid
	if (CasingClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);

			// Spawns an actor of this type
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ACasing>(
					CasingClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
					);
			}
		}
	}
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);

	// Detach the weapon
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);

	// Set the owner's weapon to nullptr
	SetOwner(nullptr);
}

