// Copyright 2017-2022. All Rights Reserved.
// ���ļ�������������ģ�ͼ��غ���ʾ�߼�

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"//�������
#include "Components/CapsuleComponent.h"//�������
//#include "SceneObj.h"
#include "RocketActor.generated.h"

class ACADBaseActor;
// ���CAD�ļ���ȡ���ʱ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_RocketCADLoadCompleted, ACADBaseActor*, cadActor);


// ģ��actor���࣬����CAD�ļ����غ���ʾ���ܣ�����������ͷɴ����Ǳ��������
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ACADBaseActor : public AActor
{
	GENERATED_BODY()
public:
	ACADBaseActor();
	virtual ~ACADBaseActor()
	{
	}
	// ����csv�ļ�
	UFUNCTION(BlueprintCallable, Category = "RocketActor")
	virtual bool LoadCADFile();
	// CADģ�ͼ������Զ����ñ�����
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "RocketActor")
	void OnCADLoadFinished();
	// ��ȡ��ǰλ�õľ�γ�ȣ��Ƕȣ��͸߶ȣ�m��
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RocketActor")
	FVector GetCurLongLatAlt() const;

protected:
	virtual void BeginPlay() override;
	// ����occ�ļ����ʱ�����̵߳��ô˺�����ִ����ʾ����
	virtual bool LoadCADFileCompleted(class FLoadOccPayload* payload);
	//virtual UStaticMeshComponent* CreateMeshComponent(TSharedPtr<FSceneObjBase, ESPMode::ThreadSafe> sceneObj, USceneComponent* rootOCC, const TArray<FVector>& pts, const TArray<int32>& index, float scale);
	// �������ű���
	virtual float CalcScale(const FBox& bound, float minMeshRadiusToScale) const;
	virtual float CalcScale(double* bound, float minMeshRadiusToScale) const;
	// ��ȡcad�ļ�·��
	FString GetCADFilePath() const;
	virtual USceneComponent* GetCADParentComponent() const
	{
		return SceneRoot;
	}
	UPROPERTY(BlueprintAssignable, Category = "RocketActor")
	FDelegate_RocketCADLoadCompleted Delegate_RocketCADLoadCompleted;

	// actor�����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	USceneComponent* SceneRoot = nullptr;
	// �������������
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ARocketEarthDirector* RocketEarthDirector = nullptr;
	// ģ�͵���ʾ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* MeshMaterial = nullptr;
	// cad�ļ����ļ���
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketActor")
	FString RocketCADFile;
	// ģ�͵����ű���
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RocketActor")
	float fScale = 10.f;
	// �Ƿ��Ѿ�������cadģ��
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RocketActor")
	bool bLoadCADFinished = false;
};

// �ɴ�actor
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

// ���������actor
UCLASS(Blueprintable)
class TRAJECTORYDATA_API ARocketBoosterActor : public ACADBaseActor
{
	GENERATED_BODY()
public:
	ARocketBoosterActor();
	virtual ~ARocketBoosterActor()
	{
	}
	// ����csv�ļ�
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
