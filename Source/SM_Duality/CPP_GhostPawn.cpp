// ACPP_GhostPawn


#include "CPP_GhostPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputLibrary.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerGameModeBase.h"
#include "CPP_Jolyne.h"
#include "GameFramework/WorldSettings.h"

// Sets default values
ACPP_GhostPawn::ACPP_GhostPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	staticMesh->SetupAttachment(RootComponent);
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	cameraComponent = CreateDefaultSubobject<UCameraComponent>("camera");
	springArmComponent->SetupAttachment(staticMesh);
	springArmComponent->TargetArmLength = 300;
	springArmComponent->TargetOffset.Z = 150;
	cameraComponent->SetupAttachment(springArmComponent);
}

// Called when the game starts or when spawned
void ACPP_GhostPawn::BeginPlay()
{
	Super::BeginPlay();
	InitInput();
	playerRef = Cast<ACPP_Jolyne>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void ACPP_GhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACPP_GhostPawn::DebugText(FString _text)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *_text);
}
//void ACPP_GhostEntity::GravityOff()
//{
//	
//	
//	UCharacterMovementComponent* _characterMovement = GetCharacterMovement();
//	if (_characterMovement)
//	{
//		// Désactivez la gravité
//		_characterMovement->GravityScale = 0.0f;
//	}
//	
//}

#pragma region Input 
void ACPP_GhostPawn::InitInput()
{
	ULocalPlayer* _localPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer();
	if (!_localPlayer)return;
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = _localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!_inputSystem)return;
	_inputSystem->AddMappingContext(mappingContext, 0);
}
#pragma region Input(ZQSD, AE, MOUSE)
void ACPP_GhostPawn::MoveForward(const FInputActionValue& _value)
{
	//if (isDead)return;
	//DebugText("Fwd");
	const FVector _fwd = GetActorForwardVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	//forwardAxisAnime = _movementValue;
	AddMovementInput(_fwd, _movementValue);
}// Z,S.
void ACPP_GhostPawn::MoveRight(const FInputActionValue& _value)
{
	//if (isDead)return;
	//DebugText("Rgt");
	const FVector _rgt = GetActorRightVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	//rightAxisAnime = _movementValue;
	AddMovementInput(_rgt, _movementValue);
}// D,Q.
void ACPP_GhostPawn::Rotate(const FInputActionValue& _value)
{
	//if (isDead)return;
	//DebugText("Rotate");
	const float _delta = GetWorld()->DeltaTimeSeconds;
	//const float _rotationValue = _value.Get<float>()* rotationSpeed;
	const float _rotationValue = _value.Get<float>() * _delta * rotationSpeed;
	AddControllerYawInput(_rotationValue);
}// MouseX , E,A .

//Vertical Rotate Cam
void ACPP_GhostPawn::RotateUP(const FInputActionValue& _value)
{
	
	if (isDead)return;
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _rotationValue = -_value.Get<float>() * _delta * rotationSpeed;
	if (springArmComponent)
	{
		
		FRotator NewRotation = springArmComponent->GetComponentRotation() + FRotator(_rotationValue, 0.0f, 0.0f);

		// Clamp vertical rotate
		const float MinPitch = -80.0f;
		const float MaxPitch = 80.0f;

		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, MinPitch, MaxPitch);

		
		springArmComponent->SetWorldRotation(NewRotation);
	}
}

#pragma endregion
#pragma region Input(clicLeft/right, Space, Ctrl)
void ACPP_GhostPawn::Fly(const FInputActionValue& _value)
{

	//DebugText("fly");
	////DebugText(_value);
	//if (isDead)return;
	//DebugText("Up/down");
	const FVector _up = GetActorUpVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	////upAxisAnime = _movementValue;
	AddMovementInput(_up, _movementValue);
	//LaunchCharacter(FVector(0.0f, 0.0f, _movementValue) * heightJump, false, true); // "Saute"

}
void ACPP_GhostPawn::SwapEntity(const FInputActionValue& _value) //f
{
	DebugText("swap");
	//fonction qui detruit ghost et qui reprend le controle de Jolyne
	APlayerController* _playerController = GetWorld()->GetFirstPlayerController();
	if (_playerController)
	{
		_playerController->Possess(playerRef);
	}
	//playerRef->SetBoolSwap();
	Destroy();
}

void ACPP_GhostPawn::Interact(const FInputActionValue& _value)
{
	DebugText("Interact");
}
void ACPP_GhostPawn::Heal(const FInputActionValue& _value)
{
	DebugText("Heal");
}

#pragma endregion
void ACPP_GhostPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _inputCompo = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!_inputCompo)
	{
		DebugText("Failed to Cast Input Component");
		return;
	}
	_inputCompo->BindAction(inputToMoveFwd, ETriggerEvent::Triggered, this, &ACPP_GhostPawn::MoveForward);
	_inputCompo->BindAction(inputToMoveFwd, ETriggerEvent::Completed, this, &ACPP_GhostPawn::MoveForward);
	_inputCompo->BindAction(inputToMoveRgt, ETriggerEvent::Triggered, this, &ACPP_GhostPawn::MoveRight);
	_inputCompo->BindAction(inputToMoveRgt, ETriggerEvent::Completed, this, &ACPP_GhostPawn::MoveRight);
	_inputCompo->BindAction(inputToRotate, ETriggerEvent::Triggered, this, &ACPP_GhostPawn::Rotate);
	_inputCompo->BindAction(inputToRotate, ETriggerEvent::Completed, this, &ACPP_GhostPawn::Rotate);
	_inputCompo->BindAction(inputToRotateUP, ETriggerEvent::Triggered, this, &ACPP_GhostPawn::RotateUP);
	_inputCompo->BindAction(inputToRotateUP, ETriggerEvent::Completed, this, &ACPP_GhostPawn::RotateUP);

	_inputCompo->BindAction(inputToInteract, ETriggerEvent::Completed, this, &ACPP_GhostPawn::Interact);
	_inputCompo->BindAction(inputToSwapEntity, ETriggerEvent::Completed, this, &ACPP_GhostPawn::SwapEntity);
	_inputCompo->BindAction(inputToHeal, ETriggerEvent::Completed, this, &ACPP_GhostPawn::Heal);
	_inputCompo->BindAction(inputToFly, ETriggerEvent::Triggered, this, &ACPP_GhostPawn::Fly);
	_inputCompo->BindAction(inputToFly, ETriggerEvent::Completed, this, &ACPP_GhostPawn::Fly);

	//le mettre a completed permet de renvoyer 0 si on appuie plus Pour ANIMATION!
}
#pragma endregion


