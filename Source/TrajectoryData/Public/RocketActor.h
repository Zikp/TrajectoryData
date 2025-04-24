// Copyright 2017-2022. All Rights Reserved.
// 本文件负责火箭案例的模型加载和显示逻辑

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"//场景组件
#include "Components/CapsuleComponent.h"//场景组件
//#include "SceneObj.h"
#include "RocketActor.generated.h"

class ACADBaseActor;
// 火箭CAD文件读取完成时调用
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_RocketCADLoadCompleted, ACADBaseActor*, cadActor);


// 模型actor基类，负责CAD文件加载和显示功能，火箭助推器和飞船都是本类的子类
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ACADBaseActor : public AActor
{
	GENERATED_BODY()
public:
	ACADBaseActor();
	virtual ~ACADBaseActor()
	{
	}
	// 加载csv文件
	UFUNCTION(BlueprintCallable, Category = "RocketActor")
	virtual bool LoadCADFile();
	// CAD模型加载完自动调用本函数
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "RocketActor")
	void OnCADLoadFinished();
	// 获取当前位置的经纬度（角度）和高度（m）
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketActor")
	FVector GetCurLongLatAlt() const;

protected:
	virtual void BeginPlay() override;
	// 加载occ文件完成时在主线程调用此函数，执行显示功能
	virtual bool LoadCADFileCompleted(class FLoadOccPayload* payload);
	//virtual UStaticMeshComponent* CreateMeshComponent(TSharedPtr<FSceneObjBase, ESPMode::ThreadSafe> sceneObj, USceneComponent* rootOCC, const TArray<FVector>& pts, const TArray<int32>& index, float scale);
	// 计算缩放倍数
	virtual float CalcScale(const FBox& bound, float minMeshRadiusToScale) const;
	virtual float CalcScale(double* bound, float minMeshRadiusToScale) const;
	// 获取cad文件路径
	FString GetCADFilePath() const;
	virtual USceneComponent* GetCADParentComponent() const
	{
		return SceneRoot;
	}
	UPROPERTY(BlueprintAssignable, Category = "RocketActor")
	FDelegate_RocketCADLoadCompleted Delegate_RocketCADLoadCompleted;

	// actor根组件
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	USceneComponent* SceneRoot = nullptr;
	// 火箭场景管理类
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ARocketEarthDirector* RocketEarthDirector = nullptr;
	// 模型的显示材质
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* MeshMaterial = nullptr;
	// cad文件的文件名
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketActor")
	FString RocketCADFile;
	// 模型的缩放倍数
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketActor")
	float fScale = 10.f;
	// 是否已经加载完cad模型
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	bool bLoadCADFinished = false;
};

// 飞船actor
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ARocketActor : public ACADBaseActor
{
	GENERATED_BODY()
public:
	ARocketActor();
	virtual ~ARocketActor()
	{
	}

protected:
	virtual USceneComponent* GetCADParentComponent() const override
	{
		return RocketRoot;
	}
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	USceneComponent* RocketParent = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	USceneComponent* RocketRoot = nullptr;
};

// 火箭助推器actor
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ARocketBoosterActor : public ACADBaseActor
{
	GENERATED_BODY()
public:
	ARocketBoosterActor();
	virtual ~ARocketBoosterActor()
	{
	}
	// 加载csv文件
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketActor")
	virtual UCapsuleComponent* GetRootCapsuleComponent() const;

protected:
	virtual USceneComponent* GetCADParentComponent() const override
	{
		return BoosterRoot;
	}
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	USceneComponent* BoosterParent = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	USceneComponent* BoosterRoot = nullptr;
};
