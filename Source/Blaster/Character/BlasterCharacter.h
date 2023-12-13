// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "BlasterCharacter.generated.h"


UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bAiming);

	void PlayElimMontage();

	// This can be a unrealiable RPC since it's just cosmetic, it doesn't affect much gameplay
	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastHit();
	/*
	*  Lecture 100, the reason why we are removing this is because getting a replicated variable is way better than sending an rpc. We also want to play hit once
	*  Damage has been taken
	*/

	virtual void OnRep_ReplicatedMovement() override;

	void Elim();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	virtual void Destroyed() override;

	// 132
	// A boolean to tell the game that the game is restarting
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* FirstPersonCamera;

	UPROPERTY()
		class ABlasterPlayerController* BlasterPlayerController;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// We want to add this in protected in case we want a Child class to take these as well
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();

	// Lesson 51
	void CrouchedButtonPressed();
	// Lession 52
	void AimButtonPressed();
	void AimButtonReleased();
	// 59
	void AimOffset(float DeltaTime);

	// This helps calculate the pitch for our sim proxies
	void CalculateAO_Pitch();

	// 92
	//This Handle turns for simulated Proxies
	void SimProxiesTurn();

	virtual void Jump() override;
	void FireButtonPressed();
	void FireButtonReleased();

	void ReloadButtonPressed();

	void PlayHitReactMontage();

	// This is the function where we can bind to a built in delegate to receive damage
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	// This function helps us know if our HUD is valid, and this will set our widget
	void UpdateHUDHealth();

	// Pull for any relevant classes and initializes our HUD
	void PullInit();

	// Simplifying the function of turning in place
	void RotateInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere)
	class UCombatComponents* Combat;
private:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// We want to have replicated overlapping weapon
	//UPROPERTY(Replicated)
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// These functions are called for whatever variables we want to replicate
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);  // In order to designate this function whenever OverlappingWeapon gets replicated,
														// we need to add "ReplicatedUsing = OnRep_OverlappingWeapon"
														// When calling this function we can only have the input parameters of the variable we are replicating

	UFUNCTION(Server, Reliable)
	void ServerEquippedButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* ElimMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshHold = 200.0f;

	bool bRotateRootBone;

	float TurnThreshHold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/*
	* Player Health
	*/
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.0f;

	UFUNCTION()
	void OnRep_Health();

	

	bool bElim = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly) 
	float ElimDelay = 3.0f;

	void ElimTimerFinished();

	/*
	* Disolve Effect
	*/

	//UPROPERTY(VisibleAnywhere)
	class UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	// We would like to use this to update the material on our character
	UFUNCTION()
	void UpdateDissolveMaterial(float DisolveValue);

	// This will start our timeline, is also going to bind our updatedissolve callback
	void StartDissolve();

	// We would use this to store our dynamic instance we use in runtime / Dynamic Instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// This would be the actual material that will be created / Material Instance set on Blueprint, used with the dynamic instance
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/*
	* ElimBot
	*/
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

public:
	// Public Setter to have this on Weapon.h
	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

	bool IsAiming();

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	AWeapon* GetEquippedWeapon();

	FORCEINLINE ETurningInPlace GetTurnInPlace() const { return TurningInPlace; };

	FVector GetHitTarget() const;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	FORCEINLINE bool IsElimmed() const { return bElim; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE UCombatComponents* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
};
