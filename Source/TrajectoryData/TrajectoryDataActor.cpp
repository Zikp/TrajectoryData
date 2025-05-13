// Fill out your copyright notice in the Description page of Project Settings.


#include "TrajectoryDataActor.h"

// Sets default values
ATrajectoryDataActor::ATrajectoryDataActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATrajectoryDataActor::BeginPlay()
{
    Super::BeginPlay();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpReuest = FHttpModule::Get().CreateRequest();
    HttpReuest->SetURL("http://127.0.0.1:5001/twinscore/get_all_scenes"); // 设置访问的地址
    HttpReuest->SetVerb(TEXT("POST")); // 选择 GET 还是 POST 方式请求
    // HttpReuest->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // 设置 Header
    HttpReuest->SetContentAsString("{}");// 如果是 POST 上传 Json 就用这个
    HttpReuest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRequestComplete);; // 绑定请求完成时候的回调函数
    // HttpReuest->OnRequestProgress().BindRaw(this, &FHttpRequestTest::OnRequestProgress); // 下载东西时检查进度用的
    HttpReuest->ProcessRequest(); // 发送这个请求
}

void ATrajectoryDataActor::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded) 
{
    if (bSucceeded)
    {
        //UE_LOG();
    }
}

void ATrajectoryDataActor::Tick(float DeltaTime)
{
    if (!bIsSimulationRunning || FlightData.Num() == 0)
        return;

    CurrentTime += DeltaTime * SimulationSpeed;

    // 查找当前时间对应的数据点
    while (CurrentDataIndex < FlightData.Num() - 1 &&
        FlightData[CurrentDataIndex + 1].Time <= CurrentTime)
    {
        CurrentDataIndex++;
    }

    if (CurrentDataIndex >= FlightData.Num() - 1)
    {
        bIsSimulationRunning = false;
        return;
    }

    // 在两个数据点之间进行插值
    const FFlightDataPoint& Current = FlightData[CurrentDataIndex];
    const FFlightDataPoint& Next = FlightData[CurrentDataIndex + 1];

    float Alpha = (CurrentTime - Current.Time) / (Next.Time - Current.Time);

    // 更新Actor的位置和旋转
    //AActor* Owner = GetOwner();
    if (1)
    {
        FVector NewLocation = FMath::Lerp(Current.Position, Next.Position, Alpha);
        FRotator NewRotation = FMath::Lerp(Current.Rotation, Next.Rotation, Alpha);

        this->SetActorLocation(NewLocation);
        this->SetActorRotation(NewRotation);
    }
}

void ATrajectoryDataActor::LoadFlightData(const FString& FilePath)
{
    FlightData.Empty();

    TArray<FString> FileLines;
    if (!FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
        return;

    // 跳过表头
    for (int32 i = 1; i < FileLines.Num(); i++)
    {
        FString Line = FileLines[i];
        TArray<FString> Values;
        Line.ParseIntoArray(Values, TEXT("\t"), true);

        if (Values.Num() >= 9)
        {
            FFlightDataPoint DataPoint;
            DataPoint.Time = FCString::Atof(*Values[0]);

            // 转换地心距、经度、纬度到UE坐标系
            double RadialDistance = FCString::Atof(*Values[2]);
            double Longitude = FCString::Atof(*Values[3]);
            double Latitude = FCString::Atof(*Values[4]);
            DataPoint.Position = ConvertToUECoordinates(RadialDistance, Longitude, Latitude);

            // 转换姿态角到UE旋转
            float AttackAngle = FCString::Atof(*Values[1]);  // 攻角
            float Inclination = FCString::Atof(*Values[6]); // 倾角
            float YawAngle = FCString::Atof(*Values[7]);    // 偏航角
            float BankAngle = Values.Num() > 8 ? FCString::Atof(*Values[8]) : 0.0f; // 倾侧角

            DataPoint.Rotation = ConvertToUERotation(AttackAngle, Inclination, YawAngle, BankAngle);
            DataPoint.Velocity = FCString::Atof(*Values[5]); // 速度

            FlightData.Add(DataPoint);
        }
    }
}

FVector ATrajectoryDataActor::ConvertToUECoordinates(double RadialDistance, double Longitude, double Latitude)
{
    // 将地心坐标系转换为UE坐标系
    // UE坐标系: X向前, Y向右, Z向上
    // 需要根据实际比例进行缩放
    const float ScaleFactor = 0.001f; // 将米转换为UE单位(通常1UE单位=1cm)

    double X = RadialDistance * FMath::Cos(Latitude) * FMath::Cos(Longitude);
    double Y = RadialDistance * FMath::Cos(Latitude) * FMath::Sin(Longitude);
    double Z = RadialDistance * FMath::Sin(Latitude);

    return FVector(X, Y, Z) * ScaleFactor;
}

FRotator ATrajectoryDataActor::ConvertToUERotation(float AttackAngle, float Inclination, float YawAngle, float BankAngle)
{
    // 将飞行器姿态角转换为UE旋转角
    // 注意: 可能需要根据具体坐标系定义调整转换关系
    return FRotator(
        FMath::RadiansToDegrees(AttackAngle),    // Pitch
        FMath::RadiansToDegrees(YawAngle),       // Yaw
        FMath::RadiansToDegrees(BankAngle)       // Roll
    );
}

void ATrajectoryDataActor::StartSimulation()
{
    bIsSimulationRunning = true;
}

void ATrajectoryDataActor::PauseSimulation()
{
    bIsSimulationRunning = false;
}

void ATrajectoryDataActor::ResetSimulation()
{
    CurrentTime = 0.0f;
    CurrentDataIndex = 0;

    // 重置Actor位置到初始状态
    if (FlightData.Num() > 0)
    {
        //AActor* Owner = GetOwner();
        if (1)
        {
            this->SetActorLocation(FlightData[0].Position);
            this->SetActorRotation(FlightData[0].Rotation);
        }
    }
}


