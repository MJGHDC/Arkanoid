#pragma once

#include "2d\CCSprite.h"
//#include "cocos2d.h"
USING_NS_CC;

class Bogie : public Sprite
{
public:
	Bogie() = default;
	~Bogie() = default;

	Bogie(int32_t tagNumber);

	static Bogie* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

private:
	int32_t mTagNumber;
};

