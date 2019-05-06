#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define BEAD_TAG 2

enum class eBeadStatus;
class Brick;

class Bead : public Sprite
{
public:
	static Bead* create(SpriteFrame* pSpriteFrame);
	static Bead* create(SpriteFrame* pSpriteFrame, int32_t tagNumber, int32_t stageBrickCount);

	Bead() = default;
	Bead(int32_t tagNumber, int32_t stageBrickCount);

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
	int32_t mDestroyedBrickCount;
	int32_t mStageBrickCount;
	bool mbBrickCheckList[maxBrickCount];
	eBeadStatus mStatus;
	float mItemTime;

	EventListenerPhysicsContact* mpContactListener;
};