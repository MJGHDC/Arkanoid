#include "Bead.h"

Bead* Bead::create(SpriteFrame * pSpriteFrame)
{
	Bead* pSprite = new Bead(BEAD_TAG);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
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

Bead::Bead(int32_t tagNumber)
	: mTagNumber(tagNumber)
{
	this->setTag(tagNumber);
}

void Bead::onEnter()
{
	Sprite::onEnter();

	//onContactSeparate
	mpContactListener = EventListenerPhysicsContact::create();
	mpContactListener->onContactSeparate = CC_CALLBACK_1(Bead::onContactSeparate, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpContactListener, this);
}

void Bead::onExit()
{
	_eventDispatcher->removeEventListener(mpContactListener);

	Sprite::onExit();
}

bool Bead::onContactSeparate(PhysicsContact & contact)
{
	auto* bodyA = contact.getShapeA()->getBody();
	auto* nodeA = bodyA->getNode();
	auto* nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB && nodeA->getTag() == BEAD_TAG)
	{
		if (nodeB->getTag() == -1)
		{
			if (nodeA->getPositionY() < 80)
			{
				log("The End");
			}
		}
		else
		{
			if (nodeB->getTag() > BEAD_TAG)
			{
				nodeB->removeFromParentAndCleanup(true);
			}

		}
	}

	return true;
}
