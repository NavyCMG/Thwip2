// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Weapon.generated.h"

// We want to create a enums to tell the state of the weapons
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	// Enum Weapon State
	EWS_Initial UMETA(DisplayName = "Inital State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);
	void Dropped();
	/*
	Textures for the weapon crosshairs
	*/

	//reload function
	UFUNCTION()
		void ActiveReload(); //when using enhanced input this took in an FInputAction value
	UFUNCTION()
		void ActiveReloadFinished();
	UFUNCTION()
		void TimeLineStart(float Progress);

	UPROPERTY()
		class ABlasterPlayerController* BlasterPlayerController;
	UPROPERTY()
		class ABlasterHUD* BlasterHUD;

	void SetActiveReloadHUDVisibility();
	void SetActiveReloadHUDProgress();
	void SetArrowNockedHUDVisibility();

	UPROPERTY()
		APawn* OwnerInstigator;

	UPROPERTY()
		AController* OwnerController;

	//Timeline component, not sure if UTimeLine or FTimeline is better?
	UTimelineComponent* ActiveReloadTL;
	//cureve float to check timeline progress
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
		UCurveFloat* ActiveReloadCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
		float ReloadProgress;
	//Reload params
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reload)
		bool IsReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reload)
		bool ArrowNocked = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reload)
		bool FirstTry = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
		float ActiveReloadZoneBegin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
		float ActiveReloadZoneEnd;
	// Use float curve in editor to set the timeline's length



	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairsBottom;

	/*
	* Zoomed FOV While Aiming
	*/

	// The more the value the further the zoom
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 60.0f;

	// The less the zoom interp speed the slower the zoom transition
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.0f;

	/*
	* Automatic Fire
	*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay = 0.15f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bAutomatic = true;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	// This is how we play an animation from code
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();

	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

public:	
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
};
