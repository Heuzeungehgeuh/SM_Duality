// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AILion.h"

#include "CPP_Jolyne.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_AILion::ACPP_AILion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	

}

// Called when the game starts or when spawned
void ACPP_AILion::BeginPlay()
{
	Super::BeginPlay();
	Init();
}


// Called every frame
void ACPP_AILion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	CanRotateToFit();
	if(nearPlayer)RotateToFitPlayer();
	if(!nearPlayer)RotateToFitDestination();
}

void ACPP_AILion::Init()
{
    playerRef = Cast<ACPP_Jolyne>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(!playerRef)return;
	canMove = true;
}

bool ACPP_AILion::GetCanMove()
{
	return canMove;
}

FVector ACPP_AILion::SetRandomLocationAroundPlayer()
{
	if(!playerRef || nearPlayer)return FVector(0);
	FVector _playerLoc = playerRef->GetActorLocation();
	float radius = 300;
	FBox randomBox = FBox(_playerLoc - FVector(radius), _playerLoc + FVector(radius));
	randomDestination = FMath::RandPointInBox(randomBox);
		return randomDestination;
}

void ACPP_AILion::CanRotateToFit()
{
	if(UKismetMathLibrary::Vector_Distance(playerRef->GetActorLocation(), GetActorLocation()) < 350)
	{
		canRotateToFitPlayer = true;
		canRotateToFitDestination = false;
		nearPlayer = true;
		canMove = false;
	}
	else
	{
		canRotateToFitDestination = true;
		canRotateToFitPlayer = false;
		nearPlayer = false;
		canMove = true;
	}	
}

void ACPP_AILion::RotateToFitPlayer()
{
	if (!playerRef) return;
	UE_LOG(LogTemp, Warning, TEXT("TEST rotate"));
	FRotator _targetRotation =  UKismetMathLibrary::RInterpTo(GetActorRotation(),playerRef->GetActorRotation(),GetWorld()->GetDeltaSeconds(),50);
	SetActorRotation(_targetRotation);
}

void ACPP_AILion::RotateToFitDestination()
{
	if (!playerRef || randomDestination.IsNearlyZero()) return;

	
	FRotator _targetRotation = (randomDestination - GetActorLocation()).Rotation();

	
	float _currentRoll = GetActorRotation().Roll;
	float _currentPitch = GetActorRotation().Pitch;

	
	FRotator NewRotation = FRotator(_currentPitch, _targetRotation.Yaw, _currentRoll);

	
	FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), NewRotation, GetWorld()->GetDeltaSeconds(), 200);

	
	SetActorRotation(InterpolatedRotation);
}


// Called to bind functionality to input
void ACPP_AILion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

