

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
//#include "InputMappingContext.h"
#include <GameFramework/SpringArmComponent.h>
#include "GameFramework/Character.h"
#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputLibrary.h"
#include "CPP_Jolyne.generated.h"


class ACPP_GhostEntity;
class ACPP_GhostPawn;
UCLASS()
class SM_DUALITY_API ACPP_Jolyne : public ACharacter
{
	GENERATED_BODY()

#pragma region Event
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, bool, _value); // dans animation ui besoin du bool, pour faire la fonction qui active l'event
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath onDeath;
	
#pragma endregion

#pragma region Component
public:
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	bool swapController = false;
protected:
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<USpringArmComponent> springArmComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TSubclassOf<ACPP_GhostPawn> entityToSpawn = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Component")
	TObjectPtr<ACPP_GhostPawn> entity = nullptr;
#pragma endregion

#pragma region Input
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputMappingContext> mappingContext = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToJump = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToSwap = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToShield = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToHeal = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToOrder = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToMoveRgt = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToMoveFwd = nullptr;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|InputMovement")
	TObjectPtr<UInputAction> inputToRotate = nullptr;
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
	float rotationSpeed = 80;
#pragma endregion

#pragma region TimerShield
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|TimerShield")
	float radius = 350;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|TimerShield")
	bool shieldReady = false;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|TimerShield")
	float currentTime = 0;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|TimerShield")
	float maxTime = 5; // temporaire pour debug 30 sinon
#pragma endregion

#pragma region Jump
private:
	// Variable pour la hauteur du saut
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump", meta = (ClampMin = "0.0"))
	float heightJump = 400;

	// Variable pour la gravité
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump")
	float graviteInitiale = 0;

	// Variable pour déterminer si le personnage est en train de sauter
protected:
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump")
	bool jumpReady = true;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump")
	float currentTime0 = 0;
	UPROPERTY(EditAnywhere, Category = "PlayerCharracter|Jump")
	float maxTime0 = 1.5;
#pragma endregion
public:
	
	ACPP_Jolyne();
public: // EVENT UI
	FOnDeath& OnDeath() { return onDeath; }
	void SetBoolSwap() { swapController = false; }
protected:
	
	virtual void BeginPlay() override;
	void ApplyGravity();
	void InitInput();
	void MoveForward(const FInputActionValue& _value);
	void MoveRight(const FInputActionValue& _value);
	void Rotate(const FInputActionValue& _value);
	void Jump(const FInputActionValue& _value);
	void SwapEntity(const FInputActionValue& _value);
	void Shield(const FInputActionValue& _value);
	void HealPet(const FInputActionValue& _value);
	void PetOrderGoTo(const FInputActionValue& _value);
	float IncreaseTime(float& _current, float& _maxTime);
	float IncreaseTime0(float& _current, float& _maxTime);
	UFUNCTION(BlueprintCallable)
	void DebugText(FString _text);
	UFUNCTION()
	void SetIsDead(bool _value);
	UFUNCTION()
	void ManageOverlap(AActor* _overlapped, AActor* _overlap);
	void TakeDamage(const float& _value);

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
