#include "OfficeWorker.h"

#include "StateTypes.h"

#include "Huddle.h"
#include "Evacuate.h"
#include "Idle.h"
#include "Walk.h"
#include "Sit.h"
#include "StandToSit.h"
#include "SitToStand.h"
#include "SitToType.h"
#include "Type.h"
#include "Filing.h"
#include "TypeToSit.h"
#include "Talk.h"

#include <AI/Inc/SeekBehaviour.h>
#include <AI/Inc/ArriveBehaviour.h>
#include <AI/Inc/WanderBehaviour.h>
#include <AI/Inc/WallAvoidanceBehaviour.h>

#include "WorldManager.h"

using namespace WOWGE;
using namespace WOWGE::AI;

namespace
{
	Maths::Vector3 Truncate(const Maths::Vector3& vector, float max)
	{
		Maths::Vector3 returnVector{ vector };
		if (Maths::MagnitudeXZ(vector) > max)
		{
			returnVector = Maths::Normalize(returnVector);

			returnVector *= max;
		}

		return returnVector;
	}
}

OfficeWorker::OfficeWorker(AIWorld& aiWorld) :
	Agent{ aiWorld },
	mSteeringModule{ *this },
	mStateMachine{ *this }, 
	mPathPlanner{ *this }
{

}

OfficeWorker::~OfficeWorker()
{
	mSteeringModule.Purge();
	mStateMachine.Purge();
}

void OfficeWorker::Initialize(const char* modelFileName, Graphics::SceneManager* sceneManager, const Maths::Vector3& position, float maxSpeed, float mass)
{
	mModelNode = sceneManager->CreateModelNodeWithBones(modelFileName);
	std::string modelName{ modelFileName };
	uint32_t found{ static_cast<uint32_t>(modelName.find_last_of(".")) };
	modelName = modelName.substr(0, found);
	mModelNode->SetName(modelName.c_str());

	mAnimationController.Initialize(mModelNode->GetModel());

	mPosition = position;
	mMaxSpeed = maxSpeed;
	mMass = mass;

	mSteeringModule.AddBehaviour<SeekBehaviour>();
	mSteeringModule.AddBehaviour<ArriveBehaviour>();
	//mSteeringModule.AddBehaviour<WallAvoidanceBehaviour>()->SetActive(true);

	mStateMachine.AddState<Huddle>();
	mStateMachine.AddState<Evacuate>();
	mStateMachine.AddState<Idle>();
	mStateMachine.AddState<Walk>();
	mStateMachine.AddState<Sit>();
	mStateMachine.AddState<StandToSit>();
	mStateMachine.AddState<SitToStand>();
	mStateMachine.AddState<SitToType>();
	mStateMachine.AddState<Type>();
	mStateMachine.AddState<Filing>();
	mStateMachine.AddState<TypeToSit>();
	mStateMachine.AddState<Talk>();
}

void OfficeWorker::SetInitialState(StateTypes statetype)
{
	mStateMachine.ChangeState(static_cast<uint32_t>(static_cast<int>(statetype)));
}

void OfficeWorker::SetOffsetMatrix(const WOWGE::Maths::Matrix44& offsetMatrix)
{
	mOffsetMatrix = offsetMatrix;
}

void OfficeWorker::CalculateTransformMatrix(const WOWGE::Maths::Matrix44& matrix)
{
	mTransformPosition = { matrix.m41, matrix.m42, matrix.m43 };
	mTransformScale = { Maths::Magnitude({ matrix.m11, matrix.m12, matrix.m13 }), Maths::Magnitude({ matrix.m21, matrix.m22, matrix.m23 }), Maths::Magnitude({ matrix.m31, matrix.m32, matrix.m33 }) };
	Maths::Matrix44 matrixWithoutScale{ Maths::Inverse(Maths::Matrix44::Scaling(mTransformScale.x, mTransformScale.y, mTransformScale.z)) * matrix };
	mTransformRotation = Maths::MatrixToQuaternion(matrixWithoutScale);
}

void OfficeWorker::AddAnimation(const char* fileName)
{
	mAnimationController.LoadAnimation(fileName);
}

void OfficeWorker::Update(float deltaTime)
{
	mStateMachine.Update(deltaTime);

	mAnimationController.Update();

	mAnimationController.Play(mCurrentAnimation, deltaTime, mIsCurrentAnimationLooping);

	//if (mSteeringModule.GetBehaviour<WallAvoidanceBehaviour>("WallAvoidance")->IsActive())
	//{
	//	CalculateWallsInRange({ {mPosition.x, mPosition.z}, mRadius });
	//}
	Maths::Vector3 steeringForce{ mSteeringModule.Calculate() };
	Maths::Vector3 acceleration{ steeringForce / mMass };

	mVelocity += acceleration * deltaTime;

	//Truncate velocity here
	mVelocity = Truncate(mVelocity, mMaxSpeed);

	mPosition.x += mVelocity.x * deltaTime;
	mPosition.z += mVelocity.z * deltaTime;

	EnforceNonPenetrationConstraint();

	if (Maths::MagnitudeXZSquare(mVelocity) > 0.0f)
	{
		Maths::Vector3 normalizedVelocity{ Maths::Normalize(mVelocity) };

		mHeading.x = normalizedVelocity.x;
		mHeading.z = normalizedVelocity.z;
	}

	CalculateTransformMatrix(mOffsetMatrix * LocalToWorld());
	mTransformScale = Maths::GetScale(mOffsetMatrix);
	mModelNode->GetModel().GetRootBone()->transform =
		Maths::Matrix44::Scaling(mTransformScale.x, mTransformScale.y, mTransformScale.z) *
		Maths::QuaternionToMatrix(mTransformRotation) *
		Maths::Matrix44::Translation(mTransformPosition);
}

Graphics::AnimationClip* OfficeWorker::GetCurrentAnimation()
{
	return mAnimationController.GetAnimationClip(mCurrentAnimation.c_str());
}

void OfficeWorker::SetCurrentAnimation(const char* animationName, bool isLooping)
{
	std::string a{ mModelNode->GetName() };
	std::string b{ animationName };
	mCurrentAnimation = a + b;

	mIsCurrentAnimationLooping = isLooping;

	mAnimationController.Reset(mCurrentAnimation);
}

Graphics::ModelNode* OfficeWorker::GetModelNode()
{
	return mModelNode;
}

uint32_t& OfficeWorker::GetOfficeWorkerIndex()
{
	return mOfficeWorkerIndex;
}

SteeringModule& OfficeWorker::GetSteeringModule()
{
	return mSteeringModule;
}

StateMachine<OfficeWorker>& OfficeWorker::GetStateMachine()
{
	return mStateMachine;
}

WOWGE::AI::PathPlanner& OfficeWorker::GetPathPlanner()
{
	return mPathPlanner;
}