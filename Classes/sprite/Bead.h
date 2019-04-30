#pragma once

#include "2d\CCSprite.h"

USING_NS_CC;

class Bead : public Sprite
{
public:
	static Bead* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Bead() = default;
	Bead(int32_t tagNumber);

	~Bead() = default;

	void onEnter() override;
	void onExit() override;

private:
	int32_t mTagNumber;
};