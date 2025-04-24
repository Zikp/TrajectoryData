// Copyright Epic Games, Inc. All Rights Reserved.
// ����ļ������������Ĺ���
#pragma once

#include "GameFramework/Actor.h"
#include "RocketActor.h"
#include "Components/SplineComponent.h"
//#include "DrawCustomLineComponent.h"
#include "RocketEarthDirector.generated.h"

class UProceduralMeshComponent;
class UStaticMeshComponent;

// ���������������Ϣ
USTRUCT(BlueprintType)
struct TRAJECTORYDATA_API FRocketEarthSettings
{
	GENERATED_BODY()
public:
	// ����Ľ׶�����csv�ļ��ﵽ������׶����ͽ����������ķ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	FString stageDropName = TEXT("��һ��");
	// �Ƿ���ʾԤ������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	bool bShowFuturePath = true;
	// �Ƿ���ʾβ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	bool bShowTrail = true;
	// �Ƿ���ʾ����ϵ�������ģ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	bool bShowAxis = false;
	// �����ٶ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	float playRate = 1.f;
};

// ����������ݵ�csv�ļ�ÿһ�е�����
USTRUCT(BlueprintType)
struct TRAJECTORYDATA_API FRocketEarthCSVItem
{
	GENERATED_BODY()
public:
	// �׶���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	FString stageName;
	// ʱ��s
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double seconds = 0.0;
	// ����rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double attackAngle = 0.0;
	// ���ľ�m
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double distEarthCenter = 0.0;
	// ����rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double longitude = 0.0;
	// γ��rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double latitude = 0.0;
	// �ٶ�m/s
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double speed = 0.0;
	// ���rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double pitch = 0.0;
	// ƫ����rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double yaw = 0.0;
	// ����rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double roll = 0.0;
	void LoadFromStringArray(const TArray<FString>& cols);
	FRotator GetRotator(class AGeoReferencingSystem* geo) const;
};

// ����csv�ļ�������
USTRUCT(BlueprintType)
struct TRAJECTORYDATA_API FRocketEarthCSVInfo
{
	GENERATED_BODY()
public:
	// ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVInfo")
	TArray<FString> ColumnName;
	// ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVInfo")
	TArray<FRocketEarthCSVItem> KeyPoints;
	void Clear()
	{
		ColumnName.Reset();
		KeyPoints.Reset();
	}
};

// ��������Ĺ�����
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ARocketEarthDirector : public AActor
{
	GENERATED_BODY()
public:
	ARocketEarthDirector();
	virtual ~ARocketEarthDirector()
	{
	}
	// ����csv�ļ�
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	bool LoadCSVFile();
	// ����csv�ļ�
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	bool AdjustSplineComponent();
	// ��ȡ��ǰ�ؼ������
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketEarth")
	int32 GetRocketKeyIndexByTime(double curSeconds) const;
	// ��ȡ��ǰ������λ��
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketEarth")
	FRotator GetRocketRotationByTime(double curSeconds) const;
	// Ϊ��������ӹؼ���
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	void AddSplineKeyPoint(double time, const FVector& pos, bool bUpdateSpline);
	// ˢ����������
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	void CompleteSplineAdd();
	// ��ȡ�����߲�ֵλ��
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	FVector GetSplinePosByTime(double time, ESplineCoordinateSpace::Type CoordinateSpace);
	// ����Ƿ�����ʱ��
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	bool CheckNewTimeEntry(double time);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void GenTrailLines();
	// ��ȡcsv�����ļ���·��
	FString GetCSVFilePath() const;
	// ��ȡ������������ļ���·��
	FString GetSettingsFilePath() const;

protected:
	// ��ǰ��ʱ�����
	int32 CurTimeIndex = -1;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketEarth")
		//UDrawCustomLineComponent* TrailLineComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketEarth")
		UMaterialInterface* LineMaterial = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketEarth")
		USplineComponent* RocketPathComponent = nullptr;
	// csv�ļ������·���������Content�ļ��У�
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketEarth")
		FString CSVRelativePath = TEXT("../data/RocketEarth/data20240710.csv");
	// β�����ӵĿ��
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketEarth")
		double TrailLineRadius = 500.0;
	// ��������csv�ļ������������
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "RocketEarth")
		FRocketEarthCSVInfo RocketCSVInfo;
	// �����������������
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "RocketEarth")
		FRocketEarthSettings RocketEarthSettings;
};
