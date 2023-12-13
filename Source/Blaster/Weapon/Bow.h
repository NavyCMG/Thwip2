// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileWeapon.h"
#include "Components/TimelineComponent.h"
#include "Bow.generated.h"


class UCurveFloat; //used by timeline

/**
 * 
 */
UCLASS()
class BLASTER_API ABow : public AProjectileWeapon
{
	GENERATED_BODY()
	

public:
	//ABow(); // constructor
	//virtual void BeginPlay() override;

	virtual void Fire(const FVector& HitTarget) override;

/*
	//reload function
	UFUNCTION()
		void ActiveReload(); //when using enhanced input this took in an FInputAction value
	UFUNCTION()
		void ActiveReloadFinished();
	UFUNCTION()
		void TimeLineStart(float Progress);

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
*/


};
