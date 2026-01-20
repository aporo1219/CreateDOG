// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dog.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/BallActor.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"//NavMesh移動に必要なインクルード
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/MyGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "BGM/MyBGM.h"

// Sets default values
ADog::ADog()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// ACharacterにはデフォルトでCapsuleComponentがRoot
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	RootComponent = GetCapsuleComponent();


	//HeadCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCollision"));
	//HeadCollision->SetupAttachment(GetMesh(), FName("head")); // headボーンにアタッチ
	//HeadCollision->SetCapsuleSize(12.f, 15.f);
	//HeadCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//HeadCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//// --- 前足（左） ---
	//FrontLeftLegCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrontLeftLegCollision"));
	//FrontLeftLegCollision->SetupAttachment(GetMesh(), FName("front_left_leg"));
	//FrontLeftLegCollision->SetCapsuleSize(6.f, 12.f);
	//FrontLeftLegCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//FrontLeftLegCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//// --- 前足（右） ---
	//FrontRightLegCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrontRightLegCollision"));
	//FrontRightLegCollision->SetupAttachment(GetMesh(), FName("front_right_leg"));
	//FrontRightLegCollision->SetCapsuleSize(6.f, 12.f);
	//FrontRightLegCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//FrontRightLegCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	//// --- 後足（左） ---
	//BackLeftLegCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BackLeftLegCollision"));
	//BackLeftLegCollision->SetupAttachment(GetMesh(), FName("back_left_leg"));
	//BackLeftLegCollision->SetCapsuleSize(6.f, 12.f);
	//BackLeftLegCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//BackLeftLegCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//// --- 後足（右） ---
	//BackRightLegCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BackRightLegCollision"));
	//BackRightLegCollision->SetupAttachment(GetMesh(), FName("back_right_leg"));
	//BackRightLegCollision->SetCapsuleSize(6.f, 12.f);
	//BackRightLegCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//BackRightLegCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 仮の見た目（エンジン内のSphereを使う）
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	//Mesh---------------------------------------------------
	UStaticMesh* DogMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/model/newdog.newdog"));
	if (DogMesh)
	{
		// 新しいメッシュコンポーネントを作成
		UStaticMeshComponent* VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
		VisualMesh->SetStaticMesh(DogMesh);

		// ルート（CapsuleComponent）にアタッチ
		VisualMesh->SetupAttachment(GetCapsuleComponent());
		//大きさ変更
		VisualMesh->SetRelativeScale3D(FVector(MeshScale));
		// 向き・位置調整（モデルに合わせて調整OK）
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -AngleCon));
		VisualMesh->SetRelativeRotation(FRotator(0.0f, AngleCon, 0.0f));

		// 衝突はカプセルで扱うため、StaticMesh側は無効に
		VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 追加：メッシュで物理シミュレートがONになっていないことを保証
		VisualMesh->SetSimulatePhysics(false);
	}
	/*else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh: /Game/model/newdog.newdog"));
	}*/
	
	// MaterialをStaticMeshに設定する
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetCollisionObjectType(ECC_Pawn);

	// 全部無視ではなく、必要なものを個別に設定
	Capsule->SetCollisionResponseToAllChannels(ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	Capsule->SetGenerateOverlapEvents(true);

	//スプリングアーム--------------------------------------------------------
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	// 距離（カメラが犬の後ろにどれくらい離れるか）
	SpringArm->TargetArmLength = 600.0f;  // 

	// カメラの角度を少し下に向ける
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	// プレイヤーのマウス操作でカメラが回らないようにする
	SpringArm->bUsePawnControlRotation = false;

	// カメラのラグでスムーズに追従
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;

	//カメラ--------------------------------------------------------------------
	// カメラのセットアップ
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// カメラはPawnの回転を使わない（固定後方視点）
	Camera->bUsePawnControlRotation = false;

	//入力----------------------------------------------------------------------
    //InputMappingの読み込み
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/input/InputMappingContext1"));

	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Look"));

	//発射位置
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	//高さ調整
	MuzzlePoint->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    //Attackアクションの読み込み
	AttackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Attack"));

	//Jumpアクション読み込み
	JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Jump"));

    //Clickアクション読み込み
	ClickAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Click"));

	//Switchアクション読み込み
	SwitchAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Switch"));

	//Standerdアクション読み込み
	StanderdAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/input/IA_Standerd"));

	// CharacterMovementComponent にジャンプ力を設定
	GetCharacterMovement()->JumpZVelocity = JumpForce;

	//スポットライト--------------------------------------------------------------------
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);//capsuleに追従
	SpotLight->SetRelativeLocation(SpotLightPos);
	SpotLight->SetRelativeRotation(SpotLightRot);
	SpotLight->Intensity = 5000.0f;//強度
	SpotLight->AttenuationRadius = 800.0f;
	SpotLight->InnerConeAngle = 25.0f;//内側の角度
	SpotLight->OuterConeAngle = 50.0f;//外側の角度
	SpotLight->SetMobility(EComponentMobility::Movable);
	SpotLight->SetVisibility(true);
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	//インップトマッピングの追加
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//マウスカーソルの有効化
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameAndUI());
	}

	//HPの初期化
	Health = MaxHealth;
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//地面についたらJumpSECheckをfalseにする
    if (GetCharacterMovement()->IsMovingOnGround())
	{
		JumpSECheck = false;
	}

	//到着判定
	if (CurrentMoveMarker)
	{
		float dis = FVector::Dist(GetActorLocation(), MoveTargetLoc);

		if (dis < GoalLocRange)
		{
			//マーカーを消す
			CurrentMoveMarker->Destroy();
			CurrentMoveMarker = nullptr;
		}
	}

	//再生されていlevelの取得
	StageName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ControlBallとIA_LookのTriggeredをBindする
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADog::Look);
		// ControlBallとIA_AttackのTriggeredをBindする
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADog::Attack);
		// ControlBallとIA_StanderdのTriggeredをBindする
		EnhancedInputComponent->BindAction(StanderdAction, ETriggerEvent::Triggered, this, &ADog::LockOn);
		// ControlBallとIA_JumpのTriggeredをBindする
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADog::Jump);
		//Contro;BallとIA_ClickのTriggeredをBindする
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ADog::MoveToMousePoint);
		//Contro;BallとIA_SwitchのTriggeredをBindする
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &ADog::Switch);
	}
}

//視点操作の処理関数
void ADog::Look(const FInputActionValue& Value)
{
	// inputのValueはVector2Dに変換
	FVector2D v = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(v.X);
		AddControllerPitchInput(v.Y);

		//Dogが持っているControlの角度を取得
		FRotator controlRotate = GetControlRotation();

		//再生されているレベルの取得
		FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

		if (LevelName == "stage1" || LevelName == "stage2" || LevelName == "stage3")
		{
			// PlayerControllerの角度を設定する
			UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(FRotator(-controlRotate.Pitch, controlRotate.Yaw, 0.0f));
		}
		else
		{
			// PlayerControllerの角度を設定する
			UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(FRotator(controlRotate.Pitch, controlRotate.Yaw, 0.0f));
		}
	}
}

//攻撃操作の処理関数
void ADog::Attack(const FInputActionValue& Value)
{
	if (IsChangeAttack)
	{

		AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->Score -= ShootScore;
			
			//スコアが0以下になったら0にする
			if (GM->Score < 0)
			{
				GM->Score = 0;
				//UI通知
				GM->OnScoreChanged.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("NoGM"));
			}
		}
		//UI通知
		GM->OnScoreChanged.Broadcast();
		
		// カメラ取得
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC) return;

		// 銃口位置と向きを取得
		FVector MuzzleLoc = MuzzlePoint->GetComponentLocation();
		FRotator MuzzleRot = MuzzlePoint->GetComponentRotation();

		// 弾の進行方向（銃口のForwardベクトル）
		FVector ShootDir = MuzzlePoint->GetForwardVector();

		// 弾生成
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABallActor* Ball = GetWorld()->SpawnActor<ABallActor>(BallActorClass, MuzzleLoc, MuzzleRot, SpawnParams);


			if (Ball)
			{
				// 発射方向を渡す
				Ball->InitVelocity(ShootDir);
				Ball->SetOwner(this);
				//発射SE
				if (SoundToPlayShot)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayShot, GetActorLocation());
				}
			}
			
	}
}

void ADog::LockOn(const FInputActionValue& Value)
{
	FVector Playerlocation = GetActorLocation();
	AActor* BestNearTarget = nullptr;
}



//ジャンプ操作の処理関数
void ADog::Jump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		//ジャンプ
		ACharacter::Jump();

		//ジャンプSE
		if (SoundToPlayJump && !JumpSECheck)
		{

			UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayJump, GetActorLocation());
			JumpSECheck = true;
		}
	}
	
}

//クリックしたら動かす関数
void ADog::MoveToMousePoint(const FInputActionValue& Value)
{
	if (!IsChangeAttack)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (!PC)
		{
			return;
		}

		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			MoveTargetLoc = Hit.ImpactPoint;

			//既存マーカーの削除
			if (CurrentMoveMarker)
			{
				CurrentMoveMarker->Destroy();
				CurrentMoveMarker = nullptr;
			}

			//マーカーの作成
			if (MoveGoalPointClass)
			{
				CurrentMoveMarker = GetWorld()->SpawnActor<AActor>
					(MoveGoalPointClass, MoveTargetLoc, FRotator::ZeroRotator);
			}

			//NavMesh上を移動
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), MoveTargetLoc);

		}
	}
}

//モードの切り替え関数
void ADog::Switch(const FInputActionValue& Value)
{
	if (!IsChangeAttack)
	{
		IsChangeAttack = true;
		//切り替えSE
		if (SoundToPlayChenge)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayChenge, GetActorLocation());
		}
	}
	else
	{
		IsChangeAttack = false;
		//切り替えSE
		if (SoundToPlayChenge)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayChenge, GetActorLocation());
		}
	}
}

//ゲームオーバー関数
void ADog::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowGameOver() called"));

	if (!GameOverClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GameOverClass is null!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null!"));
		return;
	}

	UUserWidget* GameOverUI = CreateWidget<UUserWidget>(World, GameOverClass);
	if (GameOverUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameOverUI created successfully!"));
		GameOverUI->AddToViewport();

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
			PC->SetPause(true);
		}
	}


	//入力の無効
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->DisableInput(PC);
	}


	//ゲームオーバーUIを表示
	TSubclassOf<UUserWidget> GameOverWidgetClass = LoadClass<UUserWidget>
		(nullptr, TEXT("/Game/UI/GameOver_UI.GameOver_UI_C"));
	if (GameOverWidgetClass)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport(10);
		}
	}

	//BGM再生
	AMyBGM* bgmActor = Cast<AMyBGM>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyBGM::StaticClass()));

	if (bgmActor)
	{
		bgmActor->PlayOverBGM();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMyBGM Not Found in level!"));
	}

	//ゲームの時間を止める
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

//点滅開始とダメージ処理関数
void ADog::StartBlinkAndTakeDamege(float damege)
{
	//無敵中は無視
	if (bIsInvincible)
		return;

	bIsInvincible = true;

	//ダメージ
	Health -= damege;

	//被弾SE
	if (SoundToPlayHitByEnemy)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayHitByEnemy, GetActorLocation());
	}

	if (Health < 0)
	{
		Health = 0;
	}

	//UIへ通知
	OnHealthChanged.Broadcast();

	//ゲームオーバー処理
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
		GameOver();
	}

	//倒れてる途中ならば無視
	GetWorldTimerManager().ClearAllTimersForObject(this);

	BlinkCount = 0;
	bIsVisible = true;

	//0.1秒ごとに点滅
	GetWorldTimerManager().SetTimer
	(
		BlinkTimerHandle,
		this,
		&ADog::Blink,
		BlinkInterval,
		true
	);
}

//点滅
void ADog::Blink()
{
	bIsVisible = !bIsVisible;
	SetActorHiddenInGame(!bIsVisible);

	BlinkCount++;

	if (BlinkCount >= MaxBlinkCount)
	{
		GetWorldTimerManager().ClearTimer(BlinkTimerHandle);
	}
}

////無敵処理関数
//void ADog::EndInvicnble()
//{
//	bIsInvincible = false;
//
//	SetActorHiddenInGame(false);
//}