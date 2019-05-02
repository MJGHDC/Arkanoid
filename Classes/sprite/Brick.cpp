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

	Vec2 vec = Vec2(200, 300);
	auto* pBrick = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bead"));
	pBrick->setTag(1);
	pBrick->setPosition(vec);
	//PhysicsLayer* a = static_cast<PhysicsLayer*>(nodeB->getParent());
	log("parent Tag %d", this->getTag());

	PhysicsBody* pPhysicsBody = nullptr;

	pPhysicsBody = PhysicsBody::createBox(pBrick->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));

	pPhysicsBody->setTag(pBrick->getTag());
	pBrick->addComponent(pPhysicsBody);

}

void Brick::onEnter()
{
	Sprite::onEnter();
}

void Brick::onExit()
{
	Sprite::onExit();
}
