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
	static Bead* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Bead() = default;
	Bead(int32_t tagNumber);

	~Bead() = default;

	void onEnter() override;
	void onExit() override;

	bool onContactSeparate(PhysicsContact& contact);

	void Processing(Vector<Brick*> bricks);

private:
	int32_t mTagNumber;
	int32_t mDestroyedBrickCount;
	bool mbBrickCheckList[128];
	eBeadStatus mStatus;

	EventListenerPhysicsContact* mpContactListener;
};