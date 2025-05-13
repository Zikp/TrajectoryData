// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TrajectoryDataActor.generated.h"

USTRUCT(BlueprintType)
struct FFlightDataPoint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Time;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Position;  // 转换后的UE坐标系位置

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator Rotation; // 转换后的UE坐标系旋转

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Velocity;    // 速度大小
};

UCLASS()
class TRAJECTORYDATA_API ATrajectoryDataActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrajectoryDataActor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

    UFUNCTION(BlueprintCallable, Category = "Flight Trajectory")
    void LoadFlightData(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Flight Trajectory")
    void StartSimulation();

    UFUNCTION(BlueprintCallable, Category = "Flight Trajectory")
    void PauseSimulation();

    UFUNCTION(BlueprintCallable, Category = "Flight Trajectory")
    void ResetSimulation();

protected:
    TArray<FFlightDataPoint> FlightData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Trajectory")
    float SimulationSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Trajectory")
    bool bIsSimulationRunning = false;

    float CurrentTime = 0.0f;
    int32 CurrentDataIndex = 0;

    FVector ConvertToUECoordinates(double RadialDistance, double Longitude, double Latitude);
    FRotator ConvertToUERotation(float AttackAngle, float Inclination, float YawAngle, float BankAngle);

};
