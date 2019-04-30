#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define BEAD_TAG 2

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

private:
	int32_t mTagNumber;

	EventListener* mListener;
};