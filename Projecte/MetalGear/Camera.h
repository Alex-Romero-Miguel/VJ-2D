#pragma once
#include "Enemy.h"

class Camera:
	public Enemy
{
public:

	enum CameraAnim {
		CAMERA_UP,
		CAMERA_DOWN,
		CAMERA_LEFT,
		CAMERA_RIGHT,
		CAMERA_ALERT_UP,
		CAMERA_ALERT_DOWN,
		CAMERA_ALERT_LEFT,
		CAMERA_ALERT_RIGHT
	};

	enum CameraState {
		PATROLLING,
		ALERTED
	};


	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

	glm::ivec2 getSize() const { return glm::ivec2(16, 16); }

	void setFacing(Facing dir);

	void setEnemies(std::vector<Enemy*>* enemiesList) { enemies = enemiesList; }

private: 
	
	CameraState state;

	std::vector<Enemy*>* enemies = nullptr;

	int alertTimer = 0;

	//glm::ivec2 patrolStart, patrolEnd; 
	//glm::vec2 moveDir;

	//glm::vec2 currentPatrolTarget;

	//float moveSpeed;

	//bool movable; // por defecto las cámaras no se mueven
	

};

