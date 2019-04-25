#pragma once

#include "2d\CCSprite.h"

USING_NS_CC;

class Brick : public Sprite
{
public:
	Brick() = default;
	~Brick() = default;

	Brick(int32_t tagNumber);

	static Brick* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

private:
	int32_t mTagNumber;
};