#ifndef INCLUDED_OFFICEWORKER_H
#define INCLUDED_OFFICEWORKER_H

#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/Maths.h>
#include <AI/Inc/AI.h>

#include "Destination.h"

enum class StateTypes;

class OfficeWorker : public WOWGE::AI::Agent
{
public:

	OfficeWorker(WOWGE::AI::AIWorld& aiWorld);

	~OfficeWorker() override;

	void Initialize(const char* modelFileName, WOWGE::Graphics::SceneManager* sceneManager, const WOWGE::Maths::Vector3& position, float maxSpeed, float mass);
	void SetInitialState(StateTypes statetype);
	void SetOffsetMatrix(const WOWGE::Maths::Matrix44& offsetMatrix);
	void CalculateTransformMatrix(const WOWGE::Maths::Matrix44& matrix);
	void AddAnimation(const char* fileName);
	void Update(float deltaTime);

	WOWGE::Graphics::AnimationClip* GetCurrentAnimation();
	void SetCurrentAnimation(const char* animationName, bool isLooping);

	WOWGE::Graphics::ModelNode* GetModelNode();

	DestinationType*& GetCurrentDestination() { return mCurrentDestination; }

	uint32_t& GetOfficeWorkerIndex();

	WOWGE::AI::SteeringModule& GetSteeringModule();

	WOWGE::AI::StateMachine<OfficeWorker>& GetStateMachine();

	WOWGE::AI::PathPlanner& GetPathPlanner();

	float& GetMass() { return mMass; }

	WOWGE::AI::Path& GetPath() { return mPath; }

private:
	//WOWGE::AI::PerceptionModule mPerceptionModule;
	WOWGE::AI::SteeringModule mSteeringModule;
	WOWGE::AI::StateMachine<OfficeWorker> mStateMachine;
	WOWGE::AI::PathPlanner mPathPlanner;

	WOWGE::Graphics::ModelNode* mModelNode;

	DestinationType* mCurrentDestination;

	uint32_t mOfficeWorkerIndex{ 0 };

	WOWGE::Graphics::AnimationController mAnimationController;
	std::string mCurrentAnimation;
	bool mIsCurrentAnimationLooping;

	WOWGE::Maths::Matrix44 mOffsetMatrix;
	WOWGE::Maths::Vector3 mTransformPosition;
	WOWGE::Maths::Quaternion mTransformRotation;
	WOWGE::Maths::Vector3 mTransformScale;

	WOWGE::AI::Path mPath;

	float mMass;
};

#endif // !INCLUDED_OFFICEWORKER_H