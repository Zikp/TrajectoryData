// Copyright Epic Games, Inc. All Rights Reserved.
// 这个文件负责火箭场景的管理
#pragma once

#include "GameFramework/Actor.h"
#include "RocketActor.h"
#include "Components/SplineComponent.h"
//#include "DrawCustomLineComponent.h"
#include "RocketEarthDirector.generated.h"

class UProceduralMeshComponent;
class UStaticMeshComponent;

// 火箭场景的配置信息
USTRUCT(BlueprintType)
struct TRAJECTORYDATA_API FRocketEarthSettings
{
	GENERATED_BODY()
public:
	// 分离的阶段名，csv文件里到了这个阶段名就进行助推器的分离
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	FString stageDropName = TEXT("第一级");
	// 是否显示预定弹道
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	bool bShowFuturePath = true;
	// 是否显示尾迹
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	bool bShowTrail = true;
	// 是否显示火箭上的坐标轴模型
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	bool bShowAxis = false;
	// 播放速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	float playRate = 1.f;
};

// 火箭飞行数据的csv文件每一行的数据
USTRUCT(BlueprintType)
struct TRAJECTORYDATA_API FRocketEarthCSVItem
{
	GENERATED_BODY()
public:
	// 阶段名
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	FString stageName;
	// 时间s
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double seconds = 0.0;
	// 攻角rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double attackAngle = 0.0;
	// 地心距m
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double distEarthCenter = 0.0;
	// 经度rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double longitude = 0.0;
	// 纬度rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double latitude = 0.0;
	// 速度m/s
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double speed = 0.0;
	// 倾角rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double pitch = 0.0;
	// 偏航角rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double yaw = 0.0;
	// 倾侧角rad
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVItem")
	double roll = 0.0;
	void LoadFromStringArray(const TArray<FString>& cols);
	FRotator GetRotator(class AGeoReferencingSystem* geo) const;
};

// 整个csv文件的数据
USTRUCT(BlueprintType)
struct TRAJECTORYDATA_API FRocketEarthCSVInfo
{
	GENERATED_BODY()
public:
	// 列名
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVInfo")
	TArray<FString> ColumnName;
	// 数据
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RocketEarthCSVInfo")
	TArray<FRocketEarthCSVItem> KeyPoints;
	void Clear()
	{
		ColumnName.Reset();
		KeyPoints.Reset();
	}
};

// 火箭场景的管理类
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ARocketEarthDirector : public AActor
{
	GENERATED_BODY()
public:
	ARocketEarthDirector();
	virtual ~ARocketEarthDirector()
	{
	}
	// 加载csv文件
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	bool LoadCSVFile();
	// 加载csv文件
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	bool AdjustSplineComponent();
	// 获取当前关键点序号
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketEarth")
	int32 GetRocketKeyIndexByTime(double curSeconds) const;
	// 获取当前飞行器位置
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketEarth")
	FRotator GetRocketRotationByTime(double curSeconds) const;
	// 为样条线添加关键点
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	void AddSplineKeyPoint(double time, const FVector& pos, bool bUpdateSpline);
	// 刷新样条曲线
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	void CompleteSplineAdd();
	// 获取样条线插值位置
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	FVector GetSplinePosByTime(double time, ESplineCoordinateSpace::Type CoordinateSpace);
	// 检查是否到了新时刻
	UFUNCTION(BlueprintCallable, Category = "RocketEarth")
	bool CheckNewTimeEntry(double time);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void GenTrailLines();
	// 获取csv数据文件的路径
	FString GetCSVFilePath() const;
	// 获取火箭场景配置文件的路径
	FString GetSettingsFilePath() const;

protected:
	// 当前的时刻序号
	int32 CurTimeIndex = -1;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketEarth")
		//UDrawCustomLineComponent* TrailLineComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketEarth")
		UMaterialInterface* LineMaterial = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketEarth")
		USplineComponent* RocketPathComponent = nullptr;
	// csv文件的相对路径（相对于Content文件夹）
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketEarth")
		FString CSVRelativePath = TEXT("../data/RocketEarth/data20240710.csv");
	// 尾迹粒子的宽度
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketEarth")
		double TrailLineRadius = 500.0;
	// 飞行数据csv文件里的内容数据
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "RocketEarth")
		FRocketEarthCSVInfo RocketCSVInfo;
	// 火箭场景的配置数据
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "RocketEarth")
		FRocketEarthSettings RocketEarthSettings;
};
