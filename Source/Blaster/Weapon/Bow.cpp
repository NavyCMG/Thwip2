// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Components/TimelineComponent.h"


//ABow::ABow()
//{
//	//create timeline
//	ActiveReloadTL = CreateDefaultSubobject<UTimelineComponent>(TEXT("ActiveReloadTL"));
//}
//
//void ABow::BeginPlay()
//{
//	Super::BeginPlay();
//
//	if (ActiveReloadCurve)
//	{
//		//create and bind an event/ function to the timeline start
//		FOnTimelineFloat ProgressUpdate;
//		ProgressUpdate.BindUFunction(this, FName("TimeLineStart"));
//
//		ActiveReloadTL->AddInterpFloat(ActiveReloadCurve, ProgressUpdate); //Ties Timeline float to normal float
//
//		//create and bind an event/ function to the timeline ending
//		FOnTimelineEvent ReloadEnd;
//		ReloadEnd.BindUFunction(this, FName("ActiveReloadFinished"));
//		ActiveReloadTL->SetTimelineFinishedFunc(ReloadEnd);
//	}
//}

void ABow::Fire(const FVector& HitTarget)
{
	if (ArrowNocked == true)
	{
		Super::Fire(HitTarget);
		ArrowNocked = false;
	}
}

//void ABow::ActiveReload()
//{
//	UE_LOG(LogTemp, Error, TEXT("Active Reload Function Called"));
//	if (ArrowNocked == false)
//	{
//		if (IsReloading == false)
//		{
//			UE_LOG(LogTemp, Error, TEXT("Reload Start"));
//			IsReloading = true;
//			FirstTry = true;
//			ActiveReloadTL->PlayFromStart();
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Is Reloading"));
//			if (ReloadProgress > ActiveReloadZoneBegin && ReloadProgress < ActiveReloadZoneEnd && FirstTry == true)
//			{
//				ActiveReloadTL->Stop();
//				IsReloading = false;
//				ArrowNocked = true;
//				UE_LOG(LogTemp, Error, TEXT("Successful Active Reload"));
//			}
//			else
//			{
//				FirstTry = false;
//				UE_LOG(LogTemp, Error, TEXT("Failed Active Reload"));
//			}
//		}
//	}
//}
//
//void ABow::ActiveReloadFinished()
//{
//	IsReloading = false;
//	ArrowNocked = true;
//	UE_LOG(LogTemp, Error, TEXT("Reload Finished"));
//}
//
//void ABow::TimeLineStart(float Progress)
//{
//	ReloadProgress = Progress;
//	UE_LOG(LogTemp, Error, TEXT("Progress: %f"), Progress);
//}

