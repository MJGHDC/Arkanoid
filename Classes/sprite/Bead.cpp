#include "Bead.h"

Bead::Bead(int32_t tagNumber)
	: mTagNumber(tagNumber)
{
	this->setTag(tagNumber);
}

Bead* Bead::create(SpriteFrame* pSpriteFrame, int32_t tagNumber)
{
	Bead* pSprite = new Bead(tagNumber);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

