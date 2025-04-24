// Copyright 2017-2022. All Rights Reserved.

#include "RocketActor.h"
//#include "../DrawGameInstance.h"
//#include "../frame/SceneObj.h"
//#include "CommonTypes.h"
//#include "../frame/SourceFileReaderOcc.h"
//#include "LoaderBPFunctionLibrary.h"
//#include "../DrawBlueprintFunctionLibrary.h"
//#include "GeoReferencingSystem.h"
//#include "E:/UEBin/UE_5.3/Engine/Plugins/Runtime/GeoReferencing/Source/GeoReferencing/Public/GeoReferencingSystem.h"
#include "RocketEarthDirector.h"
//#include "RocketBaseTypes.h"
#include "Kismet/GameplayStatics.h"

ACADBaseActor::ACADBaseActor()
{
	MeshMaterial = LoadObject<UMaterialInterface>(NULL, TEXT("/Game/StarterContent/Material/AM84_025_New"));
	PrimaryActorTick.bCanEverTick = true;
}

bool ACADBaseActor::LoadCADFile()
{
	//UDrawGameInstance* gameIns = Cast<UDrawGameInstance>(GetGameInstance());
	//if (!gameIns->LoadFileCheck())
	//	return false;
	//FString fileName = GetCADFilePath();
	//return gameIns->SceneManager.LoadOccFiles(FProcessCompleted::CreateWeakLambda(this, [this](FProcessData* pd, bool bSuccess) {
	//	ProcessCompletedCheckResult(TEXT("读取occ文件时遇到严重错误。建议重启本软件并重新读取！"), bSuccess);
	//	auto payload = StaticCastSharedPtr<FLoadOccPayload>((static_cast<FGeomWorker*>(pd))->GetPayload());
	//	LoadCADFileCompleted(payload.Get());
	//	// 		FGMPHelper::NotifyObjectMessage(GMP::FSigSource::NullSigSrc, MSGKEY("Process.AnyThreadCompleted"), bSuccess);
	//	}), { fileName }, nullptr);

	return true;
}

FVector ACADBaseActor::GetCurLongLatAlt() const
{
	/*AGeoReferencingSystem* geo = AGeoReferencingSystem::GetGeoReferencingSystem(const_cast<ACADBaseActor*>(this));
	if (!IsValid(geo))
		return FVector::ZeroVector;
	FGeographicCoordinates coord;
	geo->EngineToGeographic(GetActorLocation(), coord);
	return FVector(coord.Longitude, coord.Latitude, coord.Altitude);*/

	return FVector();
}

void ACADBaseActor::BeginPlay()
{
	RocketEarthDirector = Cast<ARocketEarthDirector>(UGameplayStatics::GetActorOfClass(this, ARocketEarthDirector::StaticClass()));
	Super::BeginPlay();
}

bool ACADBaseActor::LoadCADFileCompleted(class FLoadOccPayload* payload)
{
	//UDrawGameInstance* gameins = (UDrawGameInstance*)GetGameInstance();
	//gameins->ChangeIsShowThrobber(false);
	//gameins->ChangeHasCADData(true);
	//if (payload)
	//{
	//	TSharedPtr<FSceneObjBase, ESPMode::ThreadSafe> sceneObj;
	//	FGeomParamPtr geomParam = payload->GetGeomParams<FGeomParam>();
	//	if (geomParam.IsValid())
	//		sceneObj = geomParam->SceneObj.Pin();
	//	auto readerOcc = StaticCastSharedPtr<FSourceFileReaderOcc>(sceneObj->GetSourceReader());
	//	const FRTMeshResultPtr meshes = payload->GetRegularMeshes();
	//	if (meshes.IsValid())
	//	{
	//		USceneComponent* rootOCC = NewUGFComponent<USceneComponent>(this, sceneObj->GetUniqueID());
	//		rootOCC->AttachToComponent(GetCADParentComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//		rootOCC->RegisterComponent();
	//		if (readerOcc.IsValid())
	//			readerOcc->SetOCCRoot(rootOCC);
	//		// 			FFiltersContext::GetInstance().RegisterSceneComp(rootOCC);
	//		int32 nShapeIndex = 0;
	//		for (const TSharedPtr<FRTMeshFrame, ESPMode::ThreadSafe>& frame : meshes->Frames)
	//		{
	//			if (!frame.IsValid() || frame->Shapes.Num() <= 0)
	//				continue;
	//			float scale = CalcScale(frame->MeshBound, 0.f);
	//			for (int32 i = 0; i < frame->Shapes.Num(); ++i)
	//			{
	//				++nShapeIndex;
	//				const FRTMeshShape& shape = frame->Shapes[i];
	//				UStaticMeshComponent* sm = CreateMeshComponent(sceneObj, rootOCC, shape.Vertices, shape.Triangles, scale);
	//				// 					FFiltersContext::GetInstance().RegisterSceneComp(sm);
	//				if (readerOcc.IsValid())
	//				{
	//					FString shapeName = FString::Printf(TEXT("shape_%d"), nShapeIndex);
	//					readerOcc->AddShape(FRuntimeMeshID::MakeRMIDUnique(shapeName), sm);
	//				}
	//			}
	//		}
	//	}
	//}
	//bLoadCADFinished = true;
	//OnCADLoadFinished();
	//Delegate_RocketCADLoadCompleted.Broadcast(this);
	return true;
}

// 根据读取到的cad数据，创建ue中的mesh组件
//UStaticMeshComponent* ACADBaseActor::CreateMeshComponent(TSharedPtr<FSceneObjBase, ESPMode::ThreadSafe> sceneObj, USceneComponent* rootOCC, const TArray<FVector>& pts, const TArray<int32>& index, float scale)
//{
//	// #if	POST_PRIMITIVE_COLLISION
//#if 1
//	bool bGenCollision = true;
//	ECollisionEnabled::Type collisionType = ECollisionEnabled::Type::QueryOnly;
//#else
//	bool bGenCollision = false;
//	ECollisionEnabled::Type collisionType = ECollisionEnabled::Type::NoCollision;
//#endif
//	TArray<FLinearColor> colors;
//	TArray<FVector> normals;
//	NSCommonDepends::CalcMeshNormals(pts, index, normals, false);
//	UStaticMesh* mesh = ULoaderBPFunctionLibrary::CreateStaticMeshFromVertices(this, pts, index, normals, colors, false, bGenCollision);
//	UStaticMeshComponent* sm = NewUGFComponent<UStaticMeshComponent>(this, sceneObj->GetUniqueID());
//	sm->SetCollisionEnabled(collisionType);
//	sm->SetStaticMesh(mesh);
//	sm->AttachToComponent(rootOCC, FAttachmentTransformRules::KeepRelativeTransform);
//	sm->RegisterComponent();
//	UMaterialInstanceDynamic* matDyn = UMaterialInstanceDynamic::Create(MeshMaterial, sm);
//	FLinearColor clr = UDrawBlueprintFunctionLibrary::RandomColor(3);
//	clr.A = 1.0;
//	matDyn->SetVectorParameterValue(FName(TEXT("MatColor")), clr);
//	sm->SetMaterial(0, matDyn);
//	sm->SetRelativeScale3D(FVector(scale, scale, scale));
//	return sm;
//}

float ACADBaseActor::CalcScale(const FBox& bound, float minMeshRadiusToScale) const
{
	return fScale;
}

float ACADBaseActor::CalcScale(double* bound, float minMeshRadiusToScale) const
{
	return fScale;
}

FString ACADBaseActor::GetCADFilePath() const
{
	FString filePath = FPaths::ProjectContentDir() / RocketCADFile;
	return IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*filePath);
}

ARocketActor::ARocketActor()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	RocketCADFile = TEXT("../data/RocketEarth/HTV2.iges");
	RocketParent = CreateDefaultSubobject<USceneComponent>(TEXT("RocketParent"));
	RocketParent->SetupAttachment(SceneRoot);
	RocketRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RocketRoot"));
	RocketRoot->SetupAttachment(RocketParent);
}

ARocketBoosterActor::ARocketBoosterActor()
{
	SceneRoot = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	RocketCADFile = TEXT("../data/RocketEarth/HTV-2.STEP");
	BoosterParent = CreateDefaultSubobject<USceneComponent>(TEXT("BoosterParent"));
	BoosterParent->SetupAttachment(SceneRoot);
	BoosterRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BoosterRoot"));
	BoosterRoot->SetupAttachment(BoosterParent);
}

UCapsuleComponent* ARocketBoosterActor::GetRootCapsuleComponent() const
{
	return Cast<UCapsuleComponent>(SceneRoot);
}
