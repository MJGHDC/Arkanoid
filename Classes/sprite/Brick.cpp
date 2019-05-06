#include "Brick.h"
#include "sprite/properties/SpriteStatus.h"

Brick* Brick::create(SpriteFrame* pSpriteFrame, int32_t tagNumber, eItem item)
{
	Brick* pSprite = new Brick(tagNumber, item);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Brick::Brick(int32_t tagNumber, eItem item)
	: mTagNumber(tagNumber)
	, mItemUse(item)
	, mStatus(eBrickStatus::construct)
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

Sprite* Brick::GetItem() const
{
	if (mItemUse == eItem::none)
	{
		return nullptr;
	}

	Vec2 vec = Vec2(200, 300);
	auto* pItem = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("item"));
	pItem->setTag(ITEM_TAG);
	//pItem->setTag(static_cast<int32_t>(mItemUse));
	pItem->setAnchorPoint(this->getAnchorPoint());
	pItem->setPosition(this->getPosition() + Vec2(-10, 0));
	pItem->setScale(0.4f);

	if (mItemUse == eItem::powerBall)
	{
		pItem->setName("power");
	}
	else if (mItemUse == eItem::multiBall)
	{
		pItem->setName("multi");
	}

	PhysicsBody* pPhysicsBody = nullptr;

	pPhysicsBody = PhysicsBody::createBox(pItem->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));

	pPhysicsBody->setTag(pItem->getTag());
	//pPhysicsBody->setCategoryBitmask(0x02);
	//pPhysicsBody->setCollisionBitmask(0x01);
	pPhysicsBody->setCategoryBitmask(0x01);
	pPhysicsBody->setCollisionBitmask(0x02);
	pPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
	pItem->setPhysicsBody(pPhysicsBody);

	return pItem;
}
