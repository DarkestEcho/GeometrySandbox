// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGeometryActor.generated.h"

UENUM(BlueprintType)
enum class EMovementType: uint8
{
	Sin,
	Static
};

USTRUCT()							
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category="Movement")    
	float Amplitude = 50.f;                          
                                                    
	UPROPERTY(EditAnywhere, Category="Movement")    
	float Frequency = 2.f;                           
                                                    
	UPROPERTY(EditAnywhere, Category="Movement")    
	EMovementType MoveType = EMovementType::Static;

	UPROPERTY(EditAnywhere, Category="Design")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category="Design")
	float TimerRate = 3.f;

	UPROPERTY(EditAnywhere, Category="Design")
	int32 MaxTimerCount = 5;
};

UCLASS()
class GEOMETRYSANDBOX_API ABaseGeometryActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseGeometryActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	void SetGeometryData(const FGeometryData& Data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Geometry Data")
	FGeometryData GeometryData;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int32 TimerCount = 0;
	
	FVector InitialLocation;
	FTimerHandle TimerHandle;

	void HandleMovement();
	void SetColor(const FLinearColor& Color) const;

	void OnTimerFired();
};