//#pragma once
//#include "Sprite.h"
//#include "MMShot.h"
//
//class MegaMan : public Sprite
//{
//private:
//	enum MegaManStatesFlag
//	{
//		StateOnGround = 1 << 0,
//		StateRunning = 1 << 1,
//		StateFacingRight = 1 << 2,
//		StateSlide = 1 << 3,
//		StateJump = 1 << 4,
//		StateShoot = 1 << 5,
//		StateHurt = 1 << 6,
//		StateInWater = 1 << 7
//	};
//
//	Animation2D StandAni;
//	Animation2D RunAni;
//	Animation2D SlideAni;
//	Animation2D JumpAni;
//	Animation2D ClimbAni;
//	Animation2D ClimbToPlateform;
//	Animation2D HurtAni;
//	Animation2D ShootAni;
//	Animation2D ShootRunAni;
//	Animation2D ShotJumpAni;
//	Animation2D ShotClimbAni;
//
//
//	int MegaManStateBitFlag = 0;
//	float OnGroundHeight = 0.0f;
//	bool JumpLimit = false;
//
//	static constexpr float JumpHeight = 1.0f;
//	static constexpr glm::vec2 SpriteSize = glm::vec2(1.0f, 1.0f);
//public:
//	MegaMan();
//	MegaMan(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos);
//	~MegaMan();
//
//	void Update(GLFWwindow* window, RendererManager& renderer, std::shared_ptr<Camera>& camera, float dt, LightBufferObject light, std::vector<std::shared_ptr<Object2D>>& SpriteList, std::shared_ptr<TextureManager>textureManager);
//	void AnimationHandler() override;
//	void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList) override;
//};