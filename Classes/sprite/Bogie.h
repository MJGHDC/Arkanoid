#pragma once

//#include "2d\CCSprite.h"
#include "cocos2d.h"
USING_NS_CC;

class Bogie : public Sprite
{
public:
	static Bogie* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Bogie() = default;
	Bogie(int32_t tagNumber);

	~Bogie() = default;

	void onEnter() override;
	void onExit() override;

private:
	int32_t mTagNumber;

	EventListener* mListener;
};

