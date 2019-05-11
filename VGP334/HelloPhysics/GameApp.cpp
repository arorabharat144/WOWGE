#include "GameApp.h"

#include <Graphics/Inc/Gui.h>

using namespace WOWGE;

namespace
{
	float speed{ 50.0f };
	float rotationSpeed{ 10.0f };
}

GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	const uint32_t w{ 1920 };
	const uint32_t h{ 1080 };
	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	mCamera.Initialize(0.785f, static_cast<float>(w), static_cast<float>(h), 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 20.0f, -150.0f });
	mTimer.Initialize();

	Graphics::SimpleDraw::Initialize();

	mPlane = new Maths::Plane(0.0f, 1.0f, 0.0f, 0.0f);
}

void GameApp::OnTerminate()
{
	Graphics::SimpleDraw::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	Gui::Terminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	mTimer.Update();
	float deltaTime = mTimer.GetElapsedTime();
	float totalTime = mTimer.GetTotalTime();

	auto inputSystem = Input::InputSystem::Get();

	inputSystem->Update();

	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	if (inputSystem->IsMouseDown(Input::MouseButton::RBUTTON))
	{
		auto boost = inputSystem->IsKeyDown(Input::KeyCode::LSHIFT) ? 10.0f : 1.0f;
		if (inputSystem->IsKeyDown(Input::KeyCode::W))
		{
			mCamera.Walk(speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::S))
		{
			mCamera.Walk(-speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::A))
		{
			mCamera.Strafe(-speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::D))
		{
			mCamera.Strafe(speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::Q))
		{
			mCamera.Rise(-speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::E))
		{
			mCamera.Rise(speed * boost * deltaTime);
		}

		mCamera.Pitch(rotationSpeed * deltaTime * inputSystem->GetMouseMoveY()); //MOUSE MOVEMENT
		mCamera.Yaw(rotationSpeed * deltaTime * inputSystem->GetMouseMoveX()); //MOUSE MOVEMENT
	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::ONE))
	{
		mPhysicsWorld.ClearAll();

		mPlane = new Maths::Plane(0.0f, 1.0f, 0.0f, 0.0f);

		mPhysicsWorld.AddPlane(mPlane);

		for (uint32_t i{ 0 }; i < 100; ++i)
		{
			auto particle{ new Physics::Particle() };
			particle->SetPosition({ 0.0f, 100.0f, 0.0f });
			particle->SetVelocity(
			{ std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine)
			});
			particle->radius = 0.1f;
			mPhysicsWorld.AddParticle(particle);
		}
	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::TWO))
	{
		mPhysicsWorld.ClearAll();

		mPlane = new Maths::Plane(0.0f, 1.0f, 0.0f, 0.0f);
		mPhysicsWorld.AddPlane(mPlane);

		//Physics::PhysicsWorld::Settings settings;
		//settings.iterations = 100;
		//mPhysicsWorld.Setup(settings);

		for (uint32_t i{ 0 }; i < 100; ++i)
		{
			auto particle1{ new Physics::Particle{} };
			auto particle2{ new Physics::Particle{} };

			particle1->SetPosition({ 0.0f, 10.0f, 0.0f });
			particle1->SetVelocity(
			{ std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine)
			});
			particle1->radius = 0.1f;

			mPhysicsWorld.AddParticle(particle1);

			particle2->SetPosition({ 0.0f, 12.0f, 0.0f });
			particle2->SetVelocity(
			{ std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine)
			});
			particle2->radius = 0.1f;

			mPhysicsWorld.AddParticle(particle2);

			auto spring{ new Physics::Spring{particle1, particle2} };

			mPhysicsWorld.AddConstraints(spring);
		}
	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::THREE))
	{
		mPhysicsWorld.ClearAll();

		mPlane = new Maths::Plane(0.0f, 1.0f, 0.0f, 0.0f);
		mPhysicsWorld.AddPlane(mPlane);

		const uint32_t rows{ 10 };
		const uint32_t columns{ 10 };



		const uint32_t particleCount{ 1000 };
		std::vector<Physics::Particle*> particles;
		particles.resize(particleCount);

		for (uint32_t i{ 0 }; i < particleCount; ++i)
		{
			particles[i] = new Physics::Particle();

			particles[i]->SetPosition({ 0.0f, 50.0f, 0.0f });
			particles[i]->radius = 0.1f;

			particles[i]->SetVelocity(
			{ std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
			std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine)
			});

			mPhysicsWorld.AddParticle(particles[i]);
		}

		auto fixedConstraint{ new Physics::FixedConstraint(particles[0]) };
		mPhysicsWorld.AddConstraints(fixedConstraint);


		for (uint32_t i{ 0 }; i < particleCount - 1; ++i)
		{
			auto spring{ new Physics::Spring{ particles[i], particles[i + 1] } };
			mPhysicsWorld.AddConstraints(spring);
		}


	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::FOUR))
	{
		mPhysicsWorld.ClearAll();

		mPlane = new Maths::Plane(0.0f, 1.0f, 0.0f, 0.0f);
		mPhysicsWorld.AddPlane(mPlane);

		const uint32_t rows{ 10 };
		const uint32_t columns{ 10 };

		for (uint32_t i{ 0 }; i < 10; ++i)
		{
			const uint32_t particleCount{ 100 };
			std::vector<Physics::Particle*> particles;
			particles.resize(particleCount);

			std::vector<Physics::Spring*> springs;
			springs.resize(particleCount - 1);

			for (uint32_t i{ 0 }; i < particleCount - 1; ++i)
			{
				particles[i] = new Physics::Particle();

				particles[i]->SetPosition({ 0.0f, 50.0f, 0.0f });
				particles[i]->radius = 0.1f;

				particles[i]->SetVelocity(
				{ std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
				std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
				std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine)
				});

				mPhysicsWorld.AddParticle(particles[i]);

				if (i != 0)
				{
					springs[i] = new Physics::Spring(particles[i - 1], particles[i], 1.0f);
					mPhysicsWorld.AddConstraints(springs[i]);
				}
			}
		}
	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::FIVE))
	{
		mPhysicsWorld.ClearAll();

		mPlane = new Maths::Plane(0.0f, 1.0f, 0.0f, 0.0f);
		mPhysicsWorld.AddPlane(mPlane);

		const uint32_t rows{ 20 };
		const uint32_t columns{ 20 };

		std::vector<Physics::FixedConstraint*> fixedConstraints;

		std::vector<Physics::Particle*> particles;

		std::vector<Physics::Spring*> springs;

		for (uint32_t i{ 0 }; i < rows; ++i)
		{
			for (uint32_t j{ 0 }; j < columns; ++j)
			{
				particles.emplace_back(new Physics::Particle());
				particles.back()->radius = 0.1f;
				particles.back()->SetPosition({ 0.0f, 50.0f, 0.0f });

				particles.back()->SetVelocity(
				{ std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
				std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine),
				std::uniform_real_distribution<float>{-0.5f, 0.5f}(engine)
				});
				mPhysicsWorld.AddParticle(particles.back());
			}
		}

		for (uint32_t i{ 0 }; i < rows - 1; ++i)
		{
			for (uint32_t j{ 0 }; j < columns; ++j)
			{
				springs.emplace_back(new Physics::Spring(particles[j + (i * rows)], particles[j + (i * rows) + 1], 1.0f));
				mPhysicsWorld.AddConstraints(springs.back());

				springs.emplace_back(new Physics::Spring(particles[j + (i * rows)], particles[j + (i * rows) + rows], 1.0f));
				mPhysicsWorld.AddConstraints(springs.back());
			}
		}

		for (uint32_t i{ 0 }; i < rows; ++i)
		{
			particles[i]->SetPosition({ static_cast<float>(i), 50.0f, 0.0f });
			fixedConstraints.emplace_back(new Physics::FixedConstraint(particles[i], {1.0f, 1.0f, 1.0f}));
			mPhysicsWorld.AddConstraints(fixedConstraints[i]);
		}

	}

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();

	mCamera.UpdateViewMatrix();

	Graphics::SimpleDraw::AddPlane(*mPlane, { 0.0f, 0.0f, 0.0f }, 100.0f, 5.0f, Maths::Vector4::Black());

	mPhysicsWorld.Update(deltaTime);
	mPhysicsWorld.DebugDraw();

	Graphics::SimpleDraw::Render(mCamera);

	graphicsSystem->EndRender();

}