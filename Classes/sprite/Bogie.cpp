#include "Bogie.h"

Bogie::Bogie(int32_t tagNumber)
	: mTagNumber(tagNumber)
{
	this->setTag(tagNumber);
}

Bogie* Bogie::create(SpriteFrame* pSpriteFrame, int32_t tagNumber)
{
	Bogie* pSprite = new Bogie(tagNumber);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

