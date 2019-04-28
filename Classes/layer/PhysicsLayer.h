#pragma once

#include "cocos2d.h"
#include "sprite/Bead.h"
#include "sprite/Bogie.h"
#include "sprite/Brick.h"

USING_NS_CC;

class PhysicsLayer : public Layer
{
	enum ePhysicsBodyType
	{
		circle,
		box,
		polygon
	};

public:
	PhysicsLayer(const Scene* const scene);
	static PhysicsLayer* create(const Scene* const scene);

	virtual bool init() override;

	//CREATE_FUNC(PhysicsLayer);

	PhysicsBody* SpriteSetPhysicsBody(Sprite* const pSprite, const float scale, const Rect& rect, const ePhysicsBodyType type, const PhysicsMaterial& material = PHYSICSBODY_MATERIAL_DEFAULT, const Vec2& offset = Vec2::ZERO) const;

	void onEnter() override;
	void onExit() override;
	void tick(float deltaTime);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

private:
	const Scene* const mParentScene;

	Size mWinSize;
	Texture2D* mpTexture;
	SpriteFrameCache* mpSpriteFrameCache;

	Bogie* mpBogie;
	Bead* mpBead;
	//Brick* pBrick;

	EventListenerTouchOneByOne* mpTouchListener;
	EventListenerKeyboard* mpKeyboardListener;

	std::unordered_map<int, Node*> mMouseJoint;
};