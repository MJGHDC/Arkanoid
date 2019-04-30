#include "Bead.h"

Bead * Bead::create(SpriteFrame * pSpriteFrame)
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

	auto* contactListener = EventListenerPhysicsContact::create();
	//contactListener->onContactSeparate
	contactListener->onContactSeparate = [=](PhysicsContact& contact) {

		auto nodeA = contact.getShapeA()->getBody()->getNode();
		auto nodeB = contact.getShapeB()->getBody()->getNode();

		if (nodeA && nodeB)
		{
			log("nodeA Tag : %d", nodeA->getTag());
			log("nodeB Tag : %d", nodeB->getTag());
			if (nodeA->getTag() == BEAD_TAG)
			{
				nodeB->removeFromParentAndCleanup(true);
			}
			else if (nodeB->getTag() == BEAD_TAG)
			{
				nodeA->removeFromParentAndCleanup(true);
			}
		}

		return true;
	}; 

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	mListener = contactListener;
}

void Bead::onExit()
{
	_eventDispatcher->removeEventListener(mListener);
	Sprite::onExit();
}
