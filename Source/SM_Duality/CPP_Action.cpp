// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Action.h"
#include "CPP_GhostPawn.h"

// Sets default values
ACPP_Action::ACPP_Action()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_Action::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this,&ACPP_Action::ManageOverlap);
}

 void ACPP_Action::ManageOverlap(AActor* _overlapped, AActor* _overlap)
{
	if(!_overlap || !_overlapped)return;
	
	if(!_overlap->IsA<ACPP_GhostPawn>())return;
	
 	UE_LOG(LogTemp, Warning, TEXT("TEST ENTER"))
	
	for (int i = 0; i < allTarget.Num(); i++)
	{
		if(allTarget[i] && allTarget[i]->IsValidLowLevel())
		{
			allTarget[i]->Destroy();
		}
	}
}

// Called every frame
void ACPP_Action::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

