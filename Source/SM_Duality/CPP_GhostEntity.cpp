//


#include "CPP_GhostEntity.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputLibrary.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerGameModeBase.h"
#include "CPP_Jolyne.h"
#include "GameFramework/WorldSettings.h"

// Sets default values
ACPP_GhostEntity::ACPP_GhostEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	cameraComponent = CreateDefaultSubobject<UCameraComponent>("camera");
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->TargetArmLength = 500.0f;
	springArmComponent->TargetOffset.Z = 200;
	cameraComponent->SetupAttachment(springArmComponent);
}

// Called when the game starts or when spawned
void ACPP_GhostEntity::BeginPlay()
{
	Super::BeginPlay();
	InitInput();
	playerRef = Cast<ACPP_Jolyne>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void ACPP_GhostEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACPP_GhostEntity::DebugText(FString _text)
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
void ACPP_GhostEntity::InitInput()
{
	ULocalPlayer* _localPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer();
	if (!_localPlayer)return;
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = _localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!_inputSystem)return;
	_inputSystem->AddMappingContext(mappingContext, 0);
}
#pragma region Input(ZQSD, AE, MOUSE)
void ACPP_GhostEntity::MoveForward(const FInputActionValue& _value)
{
	if (isDead)return;
	//DebugText("Fwd");
	const FVector _fwd = GetActorForwardVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	//forwardAxisAnime = _movementValue;
	AddMovementInput(_fwd, _movementValue);
}// Z,S.
void ACPP_GhostEntity::MoveRight(const FInputActionValue& _value)
{
	if (isDead)return;
	//DebugText("Rgt");
	const FVector _rgt = GetActorRightVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	//rightAxisAnime = _movementValue;
	AddMovementInput(_rgt, _movementValue);
}// D,Q.
void ACPP_GhostEntity::Rotate(const FInputActionValue& _value)
{
	if (isDead)return;
	//DebugText("Rotate");
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _rotationValue = _value.Get<float>() * _delta * rotationSpeed;
	AddControllerYawInput(_rotationValue);
}// MouseX , E,A .
#pragma endregion
#pragma region Input(clicLeft/right, Space, Ctrl)
void ACPP_GhostEntity::Fly(const FInputActionValue& _value)
{
	
	//DebugText("fly");
	////DebugText(_value);
	//if (isDead)return;
	//DebugText("Up/down");
	const FVector _up = GetActorUpVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>()*_delta* moveSpeed;
	////upAxisAnime = _movementValue;
	AddMovementInput(_up, _movementValue);
	//LaunchCharacter(FVector(0.0f, 0.0f, _movementValue) * heightJump, false, true); // "Saute"
	
}
void ACPP_GhostEntity::SwapEntity(const FInputActionValue& _value) //f
{
	//DebugText("swap");
	//fonction qui detruit ghost et qui reprend le controle de Jolyne
	APlayerController* _playerController = GetWorld()->GetFirstPlayerController();
	if (_playerController)
	{
		_playerController->Possess(playerRef);
	}
	Destroy();
}
	
void ACPP_GhostEntity::Interact(const FInputActionValue& _value)
{
	DebugText("Interact");
}
void ACPP_GhostEntity::Heal(const FInputActionValue& _value)
{
	DebugText("Heal");
}

#pragma endregion
void ACPP_GhostEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _inputCompo = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!_inputCompo)
	{
		DebugText("Failed to Cast Input Component");
		return;
	}
	_inputCompo->BindAction(inputToMoveFwd, ETriggerEvent::Triggered, this, &ACPP_GhostEntity::MoveForward);
	_inputCompo->BindAction(inputToMoveFwd, ETriggerEvent::Completed, this, &ACPP_GhostEntity::MoveForward);
	_inputCompo->BindAction(inputToMoveRgt, ETriggerEvent::Triggered, this, &ACPP_GhostEntity::MoveRight);
	_inputCompo->BindAction(inputToMoveRgt, ETriggerEvent::Completed, this, &ACPP_GhostEntity::MoveRight);
	_inputCompo->BindAction(inputToRotate, ETriggerEvent::Triggered, this, &ACPP_GhostEntity::Rotate);
	_inputCompo->BindAction(inputToRotate, ETriggerEvent::Completed, this, &ACPP_GhostEntity::Rotate);

	_inputCompo->BindAction(inputToInteract, ETriggerEvent::Completed, this, &ACPP_GhostEntity::Interact);
	_inputCompo->BindAction(inputToSwapEntity, ETriggerEvent::Completed, this, &ACPP_GhostEntity::SwapEntity);
	_inputCompo->BindAction(inputToHeal, ETriggerEvent::Completed, this, &ACPP_GhostEntity::Heal);
	_inputCompo->BindAction(inputToFly, ETriggerEvent::Triggered, this, &ACPP_GhostEntity::Fly);
	_inputCompo->BindAction(inputToFly, ETriggerEvent::Completed, this, &ACPP_GhostEntity::Fly);

	//le mettre a completed permet de renvoyer 0 si on appuie plus Pour ANIMATION!
}
#pragma endregion


