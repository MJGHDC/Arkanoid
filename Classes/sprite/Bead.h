#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define BEAD_TAG 2

enum class eBeadStatus;
class Brick;

class Bead final : public Sprite
{
	static int32_t s_mDestroyedBrickCount;

public:
	static Bead* create(SpriteFrame* pSpriteFrame);
	static Bead* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Bead() = default;
	Bead(int32_t tagNumber);

	~Bead() = default;

	void onEnter() override;
	void onExit() override;

	bool onContactSeparate(PhysicsContact& contact);

	Sprite* Processing(Vector<Brick*> bricks);
	void SetPowerBall();
	float Item(float deltaTime);

private:
	void statusReset();

private:
	enum
	{
		maxBrickCount = 128
	};

	int32_t mTagNumber;
	bool mbBrickCheckList[maxBrickCount];
	eBeadStatus mStatus;
	float mItemTime;
	float mSpeed;

	EventListenerPhysicsContact* mpContactListener;
};