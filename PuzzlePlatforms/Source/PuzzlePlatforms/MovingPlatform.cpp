// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		SetReplicates(true);//it can working on client update
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);//Some problem with this method X axle problem
	//GlobalTargetLocation = GlobalStartLocation + TargetLocation;
	//FVector ActorTransform = GetTransform
	//UE_LOG(LogTemp, Warning, TEXT("ActorTransform %s"), *ActorTransform.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("TargetLocation %s"), *TargetLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("GlobalTargetLocation %s"), *GlobalTargetLocation.ToString());
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(ActiveTriggers>0)
	{
		if (HasAuthority()) //only run at server
		{
			FVector Location = GetActorLocation();
			float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			float JourneyTravel = (Location - GlobalStartLocation).Size();

			if (JourneyTravel > JourneyLength)
			{
				FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}
			FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Location += Speed * DeltaTime * Direction;
			SetActorLocation(Location);
		}
	}	
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0) 
	{
		ActiveTriggers--;
	}
}

