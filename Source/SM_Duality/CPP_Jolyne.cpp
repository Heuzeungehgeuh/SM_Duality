
#include "CPP_Jolyne.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputLibrary.h"
#include "EnhancedInputComponent.h"
#include "GhostGameModeBase.h"
#include "CPP_GhostEntity.h"
#include "CPP_GhostPawn.h"

#include "GameFramework/WorldSettings.h"

// Sets default values
ACPP_Jolyne::ACPP_Jolyne()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	cameraComponent = CreateDefaultSubobject<UCameraComponent>("camera");
	//cameraComponent->AddLocalRotation. // tentative de mettre la camera dans le bonne angle -25 en pitch ou Y
	//spawnComponent = CreateDefaultSubobject<>("Spawn"); peut être un spawn de notre pet si il meurt
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->TargetArmLength = 500.0f;
	springArmComponent->TargetOffset.Z = 200;
	cameraComponent->SetupAttachment(springArmComponent);
	//graviteInitiale = GetCharacterMovement()->GravityScale; // peut etre pas besoin en laissant toujours une gravité

}

void ACPP_Jolyne::BeginPlay()
{
	Super::BeginPlay();
	InitInput();
	health = maxHealth;
	onDeath.AddDynamic(this, &ACPP_Jolyne::SetIsDead);
}
void ACPP_Jolyne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!shieldReady) 
	{
		currentTime = IncreaseTime(currentTime, maxTime);
	}
	if (!jumpReady)
	{
		currentTime0 = IncreaseTime0(currentTime0, maxTime0);
	}
	/*if (!swapController) 
	{
		InitInput();
		swapController = true;
		UInputMappingContext* inputMappingContext = UInputMappingContext::GetContext("InputMappigContext");
		if (inputMappingContext)
		{
			InputMappingContext->ChangeContext("ContextB");
		}
	}*/
	//fonction temporaire debug UI
	health;
	if(health <= 0)onDeath.Broadcast(true);
}


void ACPP_Jolyne::DebugText(FString _text)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *_text);
}

void ACPP_Jolyne::SetIsDead(bool _value)
{
	isDead = _value;
}

float ACPP_Jolyne::IncreaseTime(float& _current, float& _maxTime)
{
	float _newTime = _current + GetWorld()->DeltaTimeSeconds;
	if (_newTime >= _maxTime)
	{
		shieldReady = true;
		DebugText("shieldIsReady");
		_newTime = 0;										
	}
	return _newTime;
}
float ACPP_Jolyne::IncreaseTime0(float& _current, float& _maxTime)
{
	float _newTime = _current + GetWorld()->DeltaTimeSeconds;
	if (_newTime >= _maxTime)
	{
		jumpReady = true;
		DebugText("jumpReady");
		_newTime = 0;
	}
	return _newTime;
}
void ACPP_Jolyne::ApplyGravity()
{
	// modifie la gravité fonction appelé pendant le saut ou au moment de toucher le sol
	if (jumpReady)
	{
		
		// Simuler une gravité constante pendant le saut
		FVector gravity = FVector(0.0f, 0.0f, -1.0f) * graviteInitiale;
		jumpReady = false;
		//GetCharacterMovement()->AddForce(gravity);// peut etre pas besoin en laissant toujours une gravité
		
	}
}
//void ACPP_Jolyne::SetBoolSwap() 
//{
//
//}
		
void ACPP_Jolyne::SwapEntity(const FInputActionValue& _value)
{
	DebugText("Swap");
	FVector _add = FVector{ 150,0,0 };
	entity = GetWorld()->SpawnActor<ACPP_GhostPawn>(entityToSpawn, GetActorLocation() + _add, GetActorRotation());
	APlayerController* _playerController = GetWorld()->GetFirstPlayerController();
	if (_playerController)
	{
		_playerController->Possess(entity);
	}
	//swapController = false;
}

#pragma region overlap et perte de vie TODO
void ACPP_Jolyne::ManageOverlap(AActor* _overlapped, AActor* _overlap)
{
	if (isDead)return;
	UE_LOG(LogTemp, Warning, TEXT("%s|%s"), *_overlap->GetName(), *_overlapped->GetName());
	if (!_overlapped || !_overlap)return;
	//ACPP_Projectile* _projectile = Cast<ACPP_Projectile>(_overlap);
	/*if (!_projectile)return;
	TakeDamage(_projectile->GetDamage());*///TakeHunterDamage(damage)
	if (health <= 0)
		onDeath.Broadcast(true);
}
void ACPP_Jolyne::TakeDamage(const float& _value)
{
	health -= _value;
	health = health < 0 ? 0 : health; // if(health < 0) health = 0 else health = health;
}
#pragma endregion


#pragma region Input 
void ACPP_Jolyne::InitInput()
{
	ULocalPlayer* _localPlayer = GetWorld()->GetFirstPlayerController()->GetLocalPlayer();
	if (!_localPlayer)return;
	UEnhancedInputLocalPlayerSubsystem* _inputSystem = _localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!_inputSystem)return;
	_inputSystem->AddMappingContext(mappingContext, 0);
}
#pragma region Input(ZQSD, AE, MOUSE)
void ACPP_Jolyne::MoveForward(const FInputActionValue& _value)
{
	if (isDead)return;
	//DebugText("Fwd");
	const FVector _fwd = GetActorForwardVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	forwardAxisAnime = _movementValue;
	AddMovementInput(_fwd, _movementValue);
}// Z,S.
void ACPP_Jolyne::MoveRight(const FInputActionValue& _value)
{
	if (isDead)return;
	//DebugText("Rgt");
	const FVector _rgt = GetActorRightVector();
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _movementValue = _value.Get<float>() * _delta * moveSpeed;
	rightAxisAnime = _movementValue;
	AddMovementInput(_rgt, _movementValue);
}// D,Q.
void ACPP_Jolyne::Rotate(const FInputActionValue& _value)
{
	if (isDead)return;
	//DebugText("Rotate");
	const float _delta = GetWorld()->DeltaTimeSeconds;
	const float _rotationValue = _value.Get<float>() * _delta * rotationSpeed;
	AddControllerYawInput(_rotationValue);
}// MouseX , E,A .
#pragma endregion
#pragma region Input(clicLeft/right, Space, Ctrl)
void ACPP_Jolyne::Jump(const FInputActionValue& _value)
{
	if (!jumpReady)return
	DebugText("Jump");
	LaunchCharacter(FVector(0.0f, 0.0f, 1.0f) * heightJump, false, true);
	jumpReady = false;
}
void ACPP_Jolyne::Shield(const FInputActionValue& _value)
{
	DebugText("shield");
	const UWorld* _world = GetWorld();
	if (!_world || !shieldReady)return;
	FVector _loc = GetActorLocation();
	DrawDebugSphere(_world, _loc, radius, 50, FColor::Blue, false, 5.0f, 0, 2);
	// TODO fonction qui detruit les projectiles avec les params :: (FVector position _loc, float radius) 
	shieldReady = false;
}
void ACPP_Jolyne::HealPet(const FInputActionValue& _value)
{
	DebugText("Heal");
}
void ACPP_Jolyne::PetOrderGoTo(const FInputActionValue& _value)
{
	DebugText("Valabas");
}
#pragma endregion
void ACPP_Jolyne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _inputCompo = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!_inputCompo)
	{
		DebugText("Failed to Cast Input Component");
		return;
	}
	_inputCompo->BindAction(inputToMoveFwd, ETriggerEvent::Triggered, this, &ACPP_Jolyne::MoveForward);
	_inputCompo->BindAction(inputToMoveFwd, ETriggerEvent::Completed, this, &ACPP_Jolyne::MoveForward);
	_inputCompo->BindAction(inputToMoveRgt, ETriggerEvent::Triggered, this, &ACPP_Jolyne::MoveRight);
	_inputCompo->BindAction(inputToMoveRgt, ETriggerEvent::Completed, this, &ACPP_Jolyne::MoveRight);
	_inputCompo->BindAction(inputToRotate, ETriggerEvent::Triggered, this, &ACPP_Jolyne::Rotate);
	_inputCompo->BindAction(inputToRotate, ETriggerEvent::Completed, this, &ACPP_Jolyne::Rotate);
	_inputCompo->BindAction(inputToSwap, ETriggerEvent::Completed, this, &ACPP_Jolyne::SwapEntity);
	_inputCompo->BindAction(inputToJump, ETriggerEvent::Completed, this, &ACPP_Jolyne::Jump);
	_inputCompo->BindAction(inputToShield, ETriggerEvent::Completed, this, &ACPP_Jolyne::Shield);
	_inputCompo->BindAction(inputToHeal, ETriggerEvent::Completed, this, &ACPP_Jolyne::HealPet);
	_inputCompo->BindAction(inputToOrder, ETriggerEvent::Completed, this, &ACPP_Jolyne::PetOrderGoTo);
		
	//le mettre a completed permet de renvoyer 0 si on appuie plus Pour ANIMATION!
}
#pragma endregion






