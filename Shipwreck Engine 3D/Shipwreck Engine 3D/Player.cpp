#include "Player.h"

#include "StartEngine.h"
using namespace EngineInstance;

using namespace GameVariables;


//Container player;

bool isForce = false;

//float speedMultiplier = 500000.0f;
//float speedMultiplier = 30.1f;
float speedMultiplier = 1.4f;

const float sensitivity = 0.1f;


Transform camTransform;


void Player::Start(Container* container) {
	//camera.transform.parent = &player.transform;

	player.name = "Player";

	if (isForce) {
		player.rigidbody.NewRB(true, false);
		
		
		Model newModel = Model({
				TexturedVertex(Vec3(-1.0f, -1.0f, -1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(1.0f, -1.0f, -1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(1.0f, 1.0f, -1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(-1.0f, 1.0f, -1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(-1.0f, -1.0f, 1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(1.0f, -1.0f, 1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(), Color(), Vec2()),
				TexturedVertex(Vec3(-1.0f, 1.0f, 1.0f), Vec3(), Color(), Vec2())
			},
			{
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4,
				4, 5, 1, 1, 0, 4,
				7, 6, 2, 2, 3, 7,
				7, 4, 0, 0, 3, 7,
				6, 5, 1, 1, 2, 6
			});

		//player.rigidbody.UpdateMesh(&newModel);
	}

	startEng.containers.push_back(&player);
}

void Player::Update(float deltaTime, Container* container) {


	//FPS camera

	if (!input.mouse->mouseShown) {
		float pitchDelta = input.mouse->mouseDelta.y * sensitivity;
		float yawDelta = input.mouse->mouseDelta.x * sensitivity;

		glm::vec3 currentEulerAngles = glm::vec3(camTransform.rotation.x, camTransform.rotation.y, camTransform.rotation.z);

		currentEulerAngles.x += pitchDelta;
		currentEulerAngles.x = std::max(std::min(currentEulerAngles.x, 90.0f), -90.0f);

		currentEulerAngles.y += yawDelta;

		camTransform.rotation = Vec3(currentEulerAngles.x, currentEulerAngles.y, currentEulerAngles.z);

		camTransform.quaternionRotation = glm::quat(glm::radians(currentEulerAngles));

		camera->transform.rotation = camTransform.rotation;
		camera->transform.quaternionRotation = camTransform.quaternionRotation;

		player.transform.rotation.y = currentEulerAngles.y;


	}

	camera->transform.position = player.transform.position;


	glm::vec2 forwardVec3 = glm::vec2(player.transform.direction.x, player.transform.direction.z);
	glm::vec2 rightVec3 = glm::vec2(player.transform.right.x, player.transform.right.z);

	glm::vec2 forwardDirection = glm::vec2(forwardVec3.x, forwardVec3.y);
	glm::vec2 rightDirection = glm::vec2(rightVec3.x, rightVec3.y);

	//player.transform.rotation = Vec3(0.0f, camera.transform.rotation.y, 0.0f);
	//camera.transform.position = player.transform.position;

	if (isForce) {

		Vec3 frameVel = Vec3(0.0f, 0.0f, 0.0f);

		if (input.GetKeyState('W')) {

			//player.rigidbody.AddForce(Vec3(forwardDirection.x, 0.0f, forwardDirection.y));
			frameVel += Vec3(forwardDirection.x, 0.0f, forwardDirection.y);
		}

		if (input.GetKeyState('S')) {

			//player.rigidbody.AddForce(Vec3(-forwardDirection.x, 0.0f, -forwardDirection.y));
			frameVel -= Vec3(forwardDirection.x, 0.0f, forwardDirection.y);
		}


		if (input.GetKeyState('A')) {

			//player.rigidbody.AddForce(Vec3(-rightDirection.x, 0.0f, -rightDirection.y));
			frameVel -= Vec3(rightDirection.x, 0.0f, rightDirection.y);
		}

		if (input.GetKeyState('D')) {

			//player.rigidbody.AddForce(Vec3(rightDirection.x, 0.0f, rightDirection.y));
			frameVel += Vec3(rightDirection.x, 0.0f, rightDirection.y);
		}


		if (input.GetKeyState(VK_SPACE)) {

			//player.transform.position += Vec3(0.0f, speedMultiplier, 0.0f);
			frameVel += Vec3(0.0f, 1.0f, 0.0f);
		}

		if (input.GetKeyState(VK_SHIFT)) {

			//player.transform.position -= Vec3(0.0f, speedMultiplier, 0.0f);
			frameVel -= Vec3(0.0f, 1.0f, 0.0f);
		}

		player.rigidbody.SetVelocity(frameVel * speedMultiplier);
	}
	else {

		if (input.GetKeyState('W')) {

			player.transform.position += Vec3(forwardDirection.x, 0.0f, forwardDirection.y) * speedMultiplier;
		}

		if (input.GetKeyState('S')) {

			player.transform.position -= Vec3(forwardDirection.x, 0.0f, forwardDirection.y) * speedMultiplier;
		}


		if (input.GetKeyState('A')) {

			player.transform.position += Vec3(rightDirection.x, 0.0f, rightDirection.y) * speedMultiplier;
		}

		if (input.GetKeyState('D')) {

			player.transform.position -= Vec3(rightDirection.x, 0.0f, rightDirection.y) * speedMultiplier;
		}


		if (input.GetKeyState(VK_SPACE)) {

			player.transform.position += Vec3(0.0f, speedMultiplier, 0.0f);
		}

		if (input.GetKeyState(VK_SHIFT)) {

			player.transform.position -= Vec3(0.0f, speedMultiplier, 0.0f);
		}
	}
}