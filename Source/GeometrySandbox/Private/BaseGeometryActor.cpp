// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All)

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

void ABaseGeometryActor::SetGeometryData(const FGeometryData& Data)
{
	GeometryData = Data;
}

FGeometryData ABaseGeometryActor::GetGeometryData() const
{
	return GeometryData;
}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFired, GeometryData.TimerRate, true);
}

void ABaseGeometryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogBaseGeometry, Display, TEXT("Actor: %s is destroyed"), *GetName());
}

void ABaseGeometryActor::HandleMovement()
{
	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
		{
			FVector CurrentLocation = GetActorLocation();
			const float Time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			SetActorLocation(CurrentLocation);
		}
		break;
	case EMovementType::Static:
		break;
	default:
		break;
	}
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color) const
{
	if(!BaseMesh)
	{
		return;
	}
	
	if(UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimerFired()
{
	if(++TimerCount <= GeometryData.MaxTimerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor(); 
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
		return;
	}

	GetWorldTimerManager().ClearTimer(TimerHandle);
	OnTimerFinished.Broadcast(this);
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();	
}
