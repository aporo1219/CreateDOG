// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MyGameModeBase.h"
#include "Controller/MainPlayerController.h"
#include "Character/Dog.h"
#include "Character/DogPlayer.h"

AMyGameModeBase::AMyGameModeBase()
{
	//�v���C���[�R���g���[���[�̎w��
	//PlayerControllerClass = AMainPlayerController::StaticClass();

    //�ŏ���Pawn�̐ݒ�
	DefaultPawnClass = ADog::StaticClass();
}
