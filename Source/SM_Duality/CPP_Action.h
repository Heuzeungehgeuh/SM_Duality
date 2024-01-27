// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Action.generated.h"

UCLASS()
class SM_DUALITY_API ACPP_Action : public AActor
{
	GENERATED_BODY()	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> target;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> allTarget; 
public:	
	// Sets default values for this actor's properties
	ACPP_Action();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void ManageOverlap(AActor* _overlapped, AActor* _overlap);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
