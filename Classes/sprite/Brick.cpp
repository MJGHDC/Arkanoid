#include "Brick.h"

Brick* Brick::create(SpriteFrame* pSpriteFrame, int32_t tagNumber)
{
	Brick* pSprite = new Brick(tagNumber);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Brick::Brick(int32_t tagNumber)
	: mTagNumber(tagNumber)
{
	this->setTag(tagNumber);
}

void Brick::onEnter()
{
	Sprite::onEnter();
}

void Brick::onExit()
{
	Sprite::onExit();
}
