#include "camera.h"

void Camera::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/camera.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(CAMERA_UP, 8);
	sprite->addKeyframe(CAMERA_UP, glm::vec2(0.0f, 0.0f));
	sprite->setAnimationSpeed(CAMERA_DOWN, 8);
	sprite->addKeyframe(CAMERA_DOWN, glm::vec2(0.25f, 0.0f));
	sprite->setAnimationSpeed(CAMERA_LEFT, 8);
	sprite->addKeyframe(CAMERA_LEFT, glm::vec2(0.5f, 0.0f));
	sprite->setAnimationSpeed(CAMERA_RIGHT, 8);
	sprite->addKeyframe(CAMERA_RIGHT, glm::vec2(0.75f, 0.0f));

	sprite->setAnimationSpeed(CAMERA_ALERT_UP, 8);
	sprite->addKeyframe(CAMERA_ALERT_UP, glm::vec2(0.5f, 0.0f));
	sprite->setAnimationSpeed(CAMERA_ALERT_DOWN, 8);
	sprite->addKeyframe(CAMERA_ALERT_DOWN, glm::vec2(0.5f, 0.0f));
	sprite->setAnimationSpeed(CAMERA_ALERT_LEFT, 8);
	sprite->addKeyframe(CAMERA_ALERT_LEFT, glm::vec2(0.5f, 0.0f));
	sprite->setAnimationSpeed(CAMERA_ALERT_RIGHT, 8);
	sprite->addKeyframe(CAMERA_ALERT_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->changeAnimation(0);

	posEnemy = glm::vec2(0, 0);
	tileMapDispl = tileMapPos;

	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x, tileMapDispl.y + posEnemy.y));

	movable = false;

	moveDir = glm::ivec2(1, 0);
	moveSpeed = 1;



}

void Camera::update(int deltaTime) {
	sprite->update(deltaTime);

	switch (state) {
	case PATROLLING:
		//patrol();
		if (canSeePlayer()) {
			state = ALERTED;
			switch (facing) {
			case FACE_UP:
				sprite->changeAnimation(CAMERA_ALERT_UP);
				break;
			case FACE_DOWN:
				sprite->changeAnimation(CAMERA_ALERT_DOWN);
				break;
			case FACE_LEFT:
				sprite->changeAnimation(CAMERA_ALERT_LEFT);
				break;
			case FACE_RIGHT:
				sprite->changeAnimation(CAMERA_ALERT_RIGHT);
				break;
			}
		}
		break;
	case ALERTED:
		// Stop moving 
		break;

	}
}

void Camera::setFacing(Facing dir) { 

	facing = dir;
	switch (facing) {
	case FACE_UP:
		sprite->changeAnimation(CAMERA_UP);
		break;
	case FACE_DOWN:
		sprite->changeAnimation(CAMERA_DOWN);
		break;
	case FACE_LEFT:
		sprite->changeAnimation(CAMERA_LEFT);
		break;
	case FACE_RIGHT:
		sprite->changeAnimation(CAMERA_RIGHT);
		break;
	}
}