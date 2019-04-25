#pragma once

#include "2d\CCSprite.h"

USING_NS_CC;

class Bead : public Sprite
{
public:
	Bead() = default;
	~Bead() = default;

	Bead(int32_t tagNumber);

	static Bead* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

private:
	int32_t mTagNumber;
};