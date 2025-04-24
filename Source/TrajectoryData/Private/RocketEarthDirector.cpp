// Copyright Epic Games, Inc. All Rights Reserved.

#include "RocketEarthDirector.h"
//#include "RocketEarth.h"
#include "Kismet/KismetMathLibrary.h"
//#include "GeoReferencingSystem.h"
#include "Misc/FileHelper.h"
//#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "LoaderBPFunctionLibrary.h"
//#include "JsonUtilities/Public/JsonObjectConverter.h"
//#include "RocketBaseTypes.h"
//#include "GMPHelper.h"

// 读取一行csv数据
void FRocketEarthCSVItem::LoadFromStringArray(const TArray<FString>& cols)
{
#define ReadFieldString(ind, fieldName) \
	do { \
		if (cols.IsValidIndex(ind)) \
			this->fieldName = cols[ind]; \
	} while(false)
#define ReadFieldDouble(ind, fieldName) \
	do { \
		if (cols.IsValidIndex(ind)) \
			this->fieldName = FCString::Atod(*cols[ind]); \
	} while(false)

	ReadFieldString(0, stageName);
	ReadFieldDouble(1, seconds);
	ReadFieldDouble(2, attackAngle);
	ReadFieldDouble(3, distEarthCenter);
	ReadFieldDouble(4, longitude);
	ReadFieldDouble(5, latitude);
	ReadFieldDouble(6, speed);
	ReadFieldDouble(7, pitch);
	ReadFieldDouble(8, yaw);
	ReadFieldDouble(9, roll);
}

// 获取本行的姿态
FRotator FRocketEarthCSVItem::GetRotator(AGeoReferencingSystem* geo) const
{
	//double longitudeDeg = FMath::RadiansToDegrees(longitude);
	//double latitudeDeg = FMath::RadiansToDegrees(latitude);
	//FVector east = FVector::ZeroVector;
	//FVector north = FVector::ZeroVector;
	//FVector up = FVector::ZeroVector;
	//// 这里获取到的不是pure ECEF，而是ue中的ECEF坐标，所以后面不需要再转换了
	//geo->GetENUVectorsAtGeographicLocation(FGeographicCoordinates(longitudeDeg, latitudeDeg, 0.0), east, north, up);
	//FRotator rotBase = UKismetMathLibrary::MakeRotFromXZ(north, up);
	//double p = FMath::RadiansToDegrees(pitch);
	//double y = FMath::RadiansToDegrees(yaw);
	//double r = FMath::RadiansToDegrees(roll);
	//// 	return UKismetMathLibrary::ComposeRotators(rotBase, FRotator(p, y, r));
	//return UKismetMathLibrary::ComposeRotators(FRotator(p, y, r), rotBase);
	return FRotator();
}

ARocketEarthDirector::ARocketEarthDirector()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(scene);
	RocketPathComponent = CreateDefaultSubobject<USplineComponent>(TEXT("RocketPathComponent"));
	RocketPathComponent->SetupAttachment(GetRootComponent());
}

bool ARocketEarthDirector::LoadCSVFile()
{
	RocketCSVInfo.Clear();
	FString filePath = GetCSVFilePath();
	TArray<FString> items;
	if (!FFileHelper::LoadFileToStringArray(items, *filePath))
		return false;
	FString curStageName;
	double dLastTime = -1.0;
	for (int32 i = 0; i < items.Num(); ++i)
	{
		TArray<FString> cols;
		items[i].ParseIntoArray(cols, TEXT(","), false);
		if (i == 0)
		{
			RocketCSVInfo.ColumnName = cols;
		}
		else
		{
			FRocketEarthCSVItem keyPoint;
			keyPoint.LoadFromStringArray(cols);
			if (!keyPoint.stageName.IsEmpty())
				curStageName = keyPoint.stageName;
			keyPoint.stageName = curStageName;
			if (keyPoint.seconds > dLastTime)
				RocketCSVInfo.KeyPoints.Add(MoveTemp(keyPoint));
			dLastTime = keyPoint.seconds;
		}
	}
	return true;
}

bool ARocketEarthDirector::AdjustSplineComponent()
{
	//RocketPathComponent->ClearSplinePoints();
	//AGeoReferencingSystem* geo = AGeoReferencingSystem::GetGeoReferencingSystem(this);
	//if (!IsValid(geo))
	//	return false;
	//// 根据飞行数据添加关键点
	//for (int32 i = 0; i < RocketCSVInfo.KeyPoints.Num(); ++i)
	//{
	//	const FRocketEarthCSVItem& item = RocketCSVInfo.KeyPoints[i];
	//	double longitude = FMath::RadiansToDegrees(item.longitude);
	//	double latitude = FMath::RadiansToDegrees(item.latitude);
	//	FVector pos = FVector::ZeroVector;
	//	geo->GeographicToEngine(FGeographicCoordinates(longitude, latitude, 0.0), pos);
	//	pos = pos.GetSafeNormal() * item.distEarthCenter * 100.0; // m->cm
	//	AddSplineKeyPoint(item.seconds, pos, false);
	//}
	//// 通知spline已更新飞行路径关键点
	//CompleteSplineAdd();
	//// 生成尾迹
	//GenTrailLines();
	return true;
}

int32 ARocketEarthDirector::GetRocketKeyIndexByTime(double curSeconds) const
{
	for (int32 i = 0; i < RocketCSVInfo.KeyPoints.Num(); ++i)
	{
		const FRocketEarthCSVItem& item = RocketCSVInfo.KeyPoints[i];
		if (item.seconds > curSeconds)
			return FMath::Max(0, i - 1);
	}
	return RocketCSVInfo.KeyPoints.Num() - 1;
}

// 根据当前时间获取飞船姿态
FRotator ARocketEarthDirector::GetRocketRotationByTime(double curSeconds) const
{
	/*AGeoReferencingSystem* geo = AGeoReferencingSystem::GetGeoReferencingSystem(const_cast<ARocketEarthDirector*>(this));
	if (!IsValid(geo))
		return FRotator::ZeroRotator;
	if (RocketCSVInfo.KeyPoints.Num() <= 0)
		return FRotator::ZeroRotator;
	int32 curInd = GetRocketKeyIndexByTime(curSeconds);
	int32 nextInd = (curInd + 1) % RocketCSVInfo.KeyPoints.Num();
	const auto& curPoint = RocketCSVInfo.KeyPoints[curInd];
	const auto& nextPoint = RocketCSVInfo.KeyPoints[nextInd];
	double alpha = (curSeconds - curPoint.seconds) / (nextPoint.seconds - curPoint.seconds);
	alpha = FMath::Clamp(alpha, 0.0, 1.0);
	FRotator result = UKismetMathLibrary::RLerp(curPoint.GetRotator(geo), nextPoint.GetRotator(geo), alpha, true);
	return result;*/
	return FRotator();
}

void ARocketEarthDirector::AddSplineKeyPoint(double time, const FVector& pos, bool bUpdateSpline)
{
	RocketPathComponent->AddPoint(FSplinePoint(time, pos), bUpdateSpline);
}

void ARocketEarthDirector::CompleteSplineAdd()
{
	if (RocketCSVInfo.KeyPoints.Num() > 0)
	{
		RocketPathComponent->Duration = RocketCSVInfo.KeyPoints.Last().seconds - RocketCSVInfo.KeyPoints[0].seconds;
	}
	RocketPathComponent->UpdateSpline();
}

// 根据时间获取飞船位置
FVector ARocketEarthDirector::GetSplinePosByTime(double time, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (RocketCSVInfo.KeyPoints.Num() <= 0)
		return FVector::ZeroVector;
	const auto& points = RocketPathComponent->GetSplinePointsPosition().Points;
	int32 curInd = GetRocketKeyIndexByTime(time);
	int32 nextInd = FMath::Min(curInd + 1, points.Num() - 1);
	const auto& curPoint = points[curInd];
	const auto& nextPoint = points[nextInd];
	double delta = (nextPoint.InVal - curPoint.InVal);
	double alpha = 0.0;
	if (delta > 0.0)
		alpha = (time - curPoint.InVal) / delta;
	alpha = FMath::Clamp(alpha, 0.0, 1.0);
	return UKismetMathLibrary::VLerp(curPoint.OutVal, nextPoint.OutVal, alpha);
	// 	return RocketPathComponent->GetLocationAtTime(time, CoordinateSpace, true);
}

// 检查是否当前时刻的关键点已经改变了
bool ARocketEarthDirector::CheckNewTimeEntry(double time)
{
	/*int32 nCurIndex = GetRocketKeyIndexByTime(time);
	if (nCurIndex == CurTimeIndex || !RocketCSVInfo.KeyPoints.IsValidIndex(nCurIndex))
		return false;
	CurTimeIndex = nCurIndex;
	FGMPHelper::NotifyObjectMessage(GMP::FSigSource::NullSigSrc, MSGKEY("RocketEarth.NewTimeEntry"), time, CurTimeIndex, RocketCSVInfo.KeyPoints[nCurIndex]);*/
	return true;
}

void ARocketEarthDirector::BeginPlay()
{
	/*FString configPath = GetSettingsFilePath();
	FString result;
	FFileHelper::LoadFileToString(result, *configPath);
	FJsonObjectConverter::JsonObjectStringToUStruct(result, &RocketEarthSettings);*/
	Super::BeginPlay();
}

void ARocketEarthDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	/*FString result;
	FJsonObjectConverter::UStructToJsonObjectString(RocketEarthSettings, result);
	FString configPath = GetSettingsFilePath();
	FFileHelper::SaveStringToFile(result, *configPath);*/
}

// 生成弹道线
void ARocketEarthDirector::GenTrailLines()
{
	/*if (!RocketEarthSettings.bShowFuturePath)
		return;
	if (!IsValid(TrailLineComponent))
	{
		TrailLineComponent = NewObject<UDrawCustomLineComponent>(this);
		TrailLineComponent->RegisterComponent();
		TrailLineComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		TrailLineComponent->SetMaterial(0, LineMaterial);
	}
	const auto& points = RocketPathComponent->GetSplinePointsPosition().Points;
	float duration = RocketPathComponent->Duration;
	int32 ptnum = points.Num();
	if (ptnum <= 1)
		return;
	FCustomMeshLine line;
	for (int32 i = 0; i < points.Num() - 1; ++i)
	{
		const auto& pt1 = points[i];
		const auto& pt2 = points[i + 1];
#define ADD_TRAIL_LINE_PT(pt) \
		{\
			line.LinePoint.Add(pt.OutVal); \
			line.PointColor.Add(FColor::Red); \
			line.PointUV.Add(FVector2f(pt.InVal / duration, pt.InVal / duration));\
		}
		ADD_TRAIL_LINE_PT(pt1);
		ADD_TRAIL_LINE_PT(pt2);
#undef ADD_TRAIL_LINE_PT
	}
	TArray<FCustomMeshLine> lines{ line };
	TrailLineComponent->SetCustomLines(lines);*/
}

FString ARocketEarthDirector::GetCSVFilePath() const
{
	FString filePath = FPaths::ProjectContentDir() / CSVRelativePath;
	return IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*filePath);
}

FString ARocketEarthDirector::GetSettingsFilePath() const
{
	FString filePath = FPaths::ProjectContentDir() / TEXT("RocketSettings.txt");
	return IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*filePath);
}
