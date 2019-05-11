#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Bogie;
class Bead;
class Brick;

class Stage1 final : public Layer
{
	enum ePhysicsBodyType
	{
		circle,
		box,
		polygon
	};

public:
	Stage1(const Scene* const scene);
	static Stage1* create(const Scene* const scene);

	virtual bool init() override;

	//CREATE_FUNC(PhysicsLayer);

	void onEnter() override;
	void onExit() override;
	void tick(float deltaTime);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	void A();

private:
	const Scene* const mParentScene;

	Bogie* mpBogie;
	Bead* mpBead;
	Vector<Bead*> mBeads;
	Vector<Brick*> mBricks;
	int32_t mDestroyedBrickCount;

	EventListenerTouchOneByOne* mpTouchListener;
	EventListenerKeyboard* mpKeyboardListener;

	std::unordered_map<int, Node*> mMouseJoint;
};