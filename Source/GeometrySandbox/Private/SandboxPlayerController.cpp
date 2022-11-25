// Fill out your copyright notice in the Description page of Project Settings.


#include "SandboxPlayerController.h"

#include "SandboxPawn.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSandboxPlayerController, All, All);

void ASandboxPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent)
	{
		InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ASandboxPlayerController::ChangePawn);
	}
}

void ASandboxPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASandboxPawn::StaticClass(), Pawns);
}

void ASandboxPlayerController::ChangePawn()
{
	UE_LOG(LogSandboxPlayerController, Error, TEXT("Try to change a pawn"));
	if(Pawns.Num() <= 1)
	{
		UE_LOG(LogSandboxPlayerController, Error, TEXT("Pawns.Num() <= 1"));
		return;
	}

	ASandboxPawn* CurrentPawn = Cast<ASandboxPawn>(Pawns[CurrentPawnIndex]);

	CurrentPawnIndex = (CurrentPawnIndex + 1) % Pawns.Num();

	if(CurrentPawn && CurrentPawn!= GetPa)
	{
		UE_LOG(LogSandboxPlayerController, Error, TEXT("CurrentPawn %d"), CurrentPawnIndex);
		Possess(CurrentPawn);
	}
}
