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
    HttpReuest->SetURL("http://127.0.0.1:5001/twinscore/get_all_scenes"); // ���÷��ʵĵ�ַ
    HttpReuest->SetVerb(TEXT("POST")); // ѡ�� GET ���� POST ��ʽ����
    // HttpReuest->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // ���� Header
    HttpReuest->SetContentAsString("{}");// ����� POST �ϴ� Json �������
    HttpReuest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRequestComplete);; // ���������ʱ��Ļص�����
    // HttpReuest->OnRequestProgress().BindRaw(this, &FHttpRequestTest::OnRequestProgress); // ���ض���ʱ�������õ�
    HttpReuest->ProcessRequest(); // �����������
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

    // ���ҵ�ǰʱ���Ӧ�����ݵ�
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

    // ���������ݵ�֮����в�ֵ
    const FFlightDataPoint& Current = FlightData[CurrentDataIndex];
    const FFlightDataPoint& Next = FlightData[CurrentDataIndex + 1];

    float Alpha = (CurrentTime - Current.Time) / (Next.Time - Current.Time);

    // ����Actor��λ�ú���ת
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

    // ������ͷ
    for (int32 i = 1; i < FileLines.Num(); i++)
    {
        FString Line = FileLines[i];
        TArray<FString> Values;
        Line.ParseIntoArray(Values, TEXT("\t"), true);

        if (Values.Num() >= 9)
        {
            FFlightDataPoint DataPoint;
            DataPoint.Time = FCString::Atof(*Values[0]);

            // ת�����ľࡢ���ȡ�γ�ȵ�UE����ϵ
            double RadialDistance = FCString::Atof(*Values[2]);
            double Longitude = FCString::Atof(*Values[3]);
            double Latitude = FCString::Atof(*Values[4]);
            DataPoint.Position = ConvertToUECoordinates(RadialDistance, Longitude, Latitude);

            // ת����̬�ǵ�UE��ת
            float AttackAngle = FCString::Atof(*Values[1]);  // ����
            float Inclination = FCString::Atof(*Values[6]); // ���
            float YawAngle = FCString::Atof(*Values[7]);    // ƫ����
            float BankAngle = Values.Num() > 8 ? FCString::Atof(*Values[8]) : 0.0f; // ����

            DataPoint.Rotation = ConvertToUERotation(AttackAngle, Inclination, YawAngle, BankAngle);
            DataPoint.Velocity = FCString::Atof(*Values[5]); // �ٶ�

            FlightData.Add(DataPoint);
        }
    }
}

FVector ATrajectoryDataActor::ConvertToUECoordinates(double RadialDistance, double Longitude, double Latitude)
{
    // ����������ϵת��ΪUE����ϵ
    // UE����ϵ: X��ǰ, Y����, Z����
    // ��Ҫ����ʵ�ʱ�����������
    const float ScaleFactor = 0.001f; // ����ת��ΪUE��λ(ͨ��1UE��λ=1cm)

    double X = RadialDistance * FMath::Cos(Latitude) * FMath::Cos(Longitude);
    double Y = RadialDistance * FMath::Cos(Latitude) * FMath::Sin(Longitude);
    double Z = RadialDistance * FMath::Sin(Latitude);

    return FVector(X, Y, Z) * ScaleFactor;
}

FRotator ATrajectoryDataActor::ConvertToUERotation(float AttackAngle, float Inclination, float YawAngle, float BankAngle)
{
    // ����������̬��ת��ΪUE��ת��
    // ע��: ������Ҫ���ݾ�������ϵ�������ת����ϵ
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

    // ����Actorλ�õ���ʼ״̬
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


