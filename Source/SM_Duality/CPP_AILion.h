// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPP_AILion.generated.h"

class ACPP_Jolyne;
UCLASS()
class SM_DUALITY_API ACPP_AILion : public APawn
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Blueprintable)
	TObjectPtr<ACPP_Jolyne> playerRef;
	UPROPERTY(EditAnywhere, Blueprintable)
	FVector randomDestination;
	UPROPERTY(EditAnywhere, Blueprintable)
	FRotator actualRotate;
	UPROPERTY(EditAnywhere, Blueprintable)
	bool canRotateToFitPlayer = false;
	UPROPERTY(EditAnywhere, Blueprintable)
	bool canRotateToFitDestination = false;
	UPROPERTY(EditAnywhere, Blueprintable)
	bool nearPlayer = false;
	UPROPERTY(EditAnywhere, Blueprintable)
	bool canMove = false;
	
public:
	// Sets default values for this pawn's properties
	ACPP_AILion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Init();
	UFUNCTION(BlueprintCallable)
	bool GetCanMove();
	UFUNCTION(BlueprintCallable)
	FVector SetRandomLocationAroundPlayer();
	void CanRotateToFit();
	void RotateToFitPlayer();
	void RotateToFitDestination();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
