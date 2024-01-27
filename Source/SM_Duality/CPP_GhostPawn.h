// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
//#include "InputMappingContext.h"
#include <GameFramework/SpringArmComponent.h>

#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputLibrary.h"
#include "CPP_GhostPawn.generated.h"

class ACPP_Jolyne;
UCLASS()
class SM_DUALITY_API ACPP_GhostPawn : public APawn
{
	GENERATED_BODY()
#pragma region Event
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, bool, _value); // dans animation ui besoin du bool, pour faire la fonction qui active l'event
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath onDeath;

#pragma endregion

#pragma region Component
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<ACPP_Jolyne> playerRef = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<UStaticMeshComponent> staticMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<USpringArmComponent> springArmComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;
#pragma endregion

#pragma region Input

	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToSwapEntity = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputMappingContext> mappingContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToJump = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToFly = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToHeal = nullptr; // peut etre heal notre perso
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToInteract = nullptr; // actionneur de levier ou de structure destructible ou faire tomber des mur pour faire une passerelle
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToMoveRgt = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToMoveFwd = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToRotate = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToRotateUP = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	float moveSpeed = 200;
#pragma endregion

#pragma region AnimationUI
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharracter|AnimationUI") //Pour animation de mort et UI
		float health = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharracter|AnimationUI") //Pour animation de mort et UI
		float maxHealth = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharracter|AnimationUI")
	bool isDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharracter|AnimationUI")
	float rightAxisAnime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharracter|AnimationUI")
	float forwardAxisAnime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharracter|AnimationUI")
	float rotationSpeed = 30;
#pragma endregion

#pragma region Fly
private:
	// Variable pour la hauteur du saut
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump", meta = (ClampMin = "0.0")) // peut etre inutile
		float heightJump = 400;

	// Variable pour la gravité
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump")
	float graviteInitiale = 0;


#pragma endregion
public:

	ACPP_GhostPawn();

protected:

	void InitInput();
	void MoveForward(const FInputActionValue& _value);
	void MoveRight(const FInputActionValue& _value);
	void Rotate(const FInputActionValue& _value);
	void RotateUP(const FInputActionValue& _value);
	void Fly(const FInputActionValue& _value);
	void Interact(const FInputActionValue& _value);
	void Heal(const FInputActionValue& _value);
	void DebugText(FString _text);
	//void GravityOff();
	void SwapEntity(const FInputActionValue& _value);
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
