#include "Player.h"

#include "StartEngine.h"
using namespace EngineInstance;

using namespace GameVariables;


//Container player;

bool isForce = false;

//float speedMultiplier = 500000.0f;
float speedMultiplier = 110.1f;

const float sensitivity = 0.1f;


Transform camTransform;


void Player::Start(Container* container) {
	camera.transform.parent = &player.transform;

	player.name = "Player";

	if (isForce) {
		player.rigidbody.isDynamic = true;
		player.rigidbody.NewRB();
		
		
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

		player.rigidbody.UpdateMesh(&newModel);
	}

	startEng.containers.push_back(&player);
}

void Player::Update(float deltaTime, Container* container) {


	//FPS camera

	if (!input.mouse->mouseShown) {
		camTransform.rotation.x += input.mouse->mouseDelta.y * sensitivity;
		camTransform.rotation.y += input.mouse->mouseDelta.x * sensitivity;

		camTransform.rotation.x = std::max(std::min(camTransform.rotation.x, 89.0f), -89.0f);


		camTransform.rotation = startEng.NormalizeAngles(camTransform.rotation);

		Vec3 radianRot = Vec3(glm::radians(camTransform.rotation.x), glm::radians(camTransform.rotation.y), glm::radians(camTransform.rotation.z));
		camera.transform.rotation = Vec3(radianRot.x, 0.0f/*radianRot.y*/, radianRot.z);

		player.transform.rotation.y = radianRot.y;
	}



	glm::vec2 forwardVec3 = glm::radians(glm::vec2(player.transform.direction.x, player.transform.direction.z));
	glm::vec2 rightVec3 = glm::radians(glm::vec2(player.transform.right.x, player.transform.right.z));

	glm::vec2 forwardDirection = glm::normalize(glm::vec2(forwardVec3.x, forwardVec3.y)) * speedMultiplier;
	glm::vec2 rightDirection = glm::normalize(glm::vec2(rightVec3.x, rightVec3.y)) * speedMultiplier;

	//player.transform.rotation = Vec3(0.0f, camera.transform.rotation.y, 0.0f);
	//camera.transform.position = player.transform.position;

	if (isForce) {

		if (input.GetKeyState('W')) {

			player.rigidbody.AddForce(Vec3(forwardDirection.x, 0.0f, forwardDirection.y));
		}

		if (input.GetKeyState('S')) {

			player.rigidbody.AddForce(Vec3(-forwardDirection.x, 0.0f, -forwardDirection.y));
		}


		if (input.GetKeyState('A')) {

			player.rigidbody.AddForce(Vec3(-rightDirection.x, 0.0f, -rightDirection.y));
		}

		if (input.GetKeyState('D')) {

			player.rigidbody.AddForce(Vec3(rightDirection.x, 0.0f, rightDirection.y));
		}


		if (input.GetKeyState(VK_SPACE)) {

			player.transform.position += Vec3(0.0f, speedMultiplier, 0.0f);
		}

		if (input.GetKeyState(VK_SHIFT)) {

			player.transform.position -= Vec3(0.0f, speedMultiplier, 0.0f);
		}
	}
	else {

		if (input.GetKeyState('W')) {

			player.transform.position += Vec3(forwardDirection.x, 0.0f, forwardDirection.y);
		}

		if (input.GetKeyState('S')) {

			player.transform.position -= Vec3(forwardDirection.x, 0.0f, forwardDirection.y);
		}


		if (input.GetKeyState('A')) {

			player.transform.position += Vec3(rightDirection.x, 0.0f, rightDirection.y);
		}

		if (input.GetKeyState('D')) {

			player.transform.position -= Vec3(rightDirection.x, 0.0f, rightDirection.y);
		}


		if (input.GetKeyState(VK_SPACE)) {

			player.transform.position += Vec3(0.0f, speedMultiplier, 0.0f);
		}

		if (input.GetKeyState(VK_SHIFT)) {

			player.transform.position -= Vec3(0.0f, speedMultiplier, 0.0f);
		}
	}
}