// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MyGameModeBase.h"
#include "Controller/MainPlayerController.h"
#include "Character/Dog.h"
#include "Character/DogPlayer.h"

AMyGameModeBase::AMyGameModeBase()
{
	//プレイヤーコントローラーの指定
	//PlayerControllerClass = AMainPlayerController::StaticClass();

    //最初のPawnの設定
	DefaultPawnClass = ADog::StaticClass();
}
