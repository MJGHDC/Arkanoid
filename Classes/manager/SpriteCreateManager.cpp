#include "SpriteCreateManager.h"
#include "sprite/Bead.h"
#include "sprite/Bogie.h"
#include "sprite/Brick.h"
#include "sprite/properties/SpriteStatus.h"

SpriteCreateManager::SpriteCreateManager()
	: mWinSize(Director::getInstance()->getWinSize())
	, mpCache(SpriteFrameCache::getInstance())
{
}

Bogie* SpriteCreateManager::CreateBogie()
{
	float scale = 1.f;
	auto* pBogie = Bogie::create(mpCache->getSpriteFrameByName("bogie"), BOGIE_TAG);
	pBogie->setPosition(Vec2(500, 100));
	auto* pPhysicsBody = SpriteSetPhysicsBody(pBogie, scale, pBogie->getTextureRect(), box, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	pPhysicsBody->setDynamic(false);
	pPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
	return pBogie;
}

Bead* SpriteCreateManager::CreateBead()
{
	float scale = 0.5f;
	auto* pBead = Bead::create(mpCache->getSpriteFrameByName("bead"), BEAD_TAG);
	pBead->setPosition(Vec2(500, 132));
	auto* pPhysicsBody = SpriteSetPhysicsBody(pBead, scale, pBead->getTextureRect(), circle, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	pPhysicsBody->setGravityEnable(false);
	pPhysicsBody->setCategoryBitmask(0x03);
	pPhysicsBody->setCollisionBitmask(0x03);
	pPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
	return pBead;
}

Bead* SpriteCreateManager::CreateBead(Vec2 position)
{
	auto* pBead = CreateBead();
	pBead->setPosition(position);

	return pBead;
}

Vector<Brick*> SpriteCreateManager::CreateBrick(const int8_t brickPlacement[8][16], int8_t widthCount, int8_t lengthCount)
{
	float scale = 1.0f;
	Size brickPosition = mWinSize - Size(0, 60);
	int32_t brickTagCount = 0;

	Vector<Brick*> bricks;
	bricks.reserve(widthCount * lengthCount);
	for (size_t i = 0; i < widthCount; ++i)
	{
		for (size_t j = 0; j < lengthCount; ++j)
		{
			if (brickPlacement[i][j] == 1)
			{
				eItem itemArr[3] = { eItem::none, eItem::powerBall, eItem::multiBall };
				int32_t tagNumber = BRICK_TAG + brickTagCount++;
				auto* pBrick = Brick::create(mpCache->getSpriteFrameByName("brick"), tagNumber, itemArr[random() % 3]);
				Rect rect = pBrick->getTextureRect();
				pBrick->setAnchorPoint(Vec2(1, 1));
				pBrick->setPosition(brickPosition - Size(rect.getMaxX() * scale * j, rect.getMaxY() * scale * i));
				auto* pPhysicsBody = SpriteSetPhysicsBody(pBrick, scale, pBrick->getTextureRect(), box, PhysicsMaterial(0.1f, 1.0f, 0.0f));
				pPhysicsBody->setCategoryBitmask(0x01);
				pPhysicsBody->setCollisionBitmask(0x02);
				pPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
				pPhysicsBody->setDynamic(false);
				bricks.pushBack(pBrick);
			}
		}
	}

	return bricks;
}

PhysicsBody* SpriteCreateManager::SpriteSetPhysicsBody(Sprite* const pSprite, const float scale, const Rect& rect, const ePhysicsBodyType type, const PhysicsMaterial& material, const Vec2& offset) const
{
	CCASSERT(pSprite != nullptr, "Sprite is Not NULL");
	CCASSERT(type != polygon, "an undefined type");

	PhysicsBody* pPhysicsBody = nullptr;

	if (type == box)
	{
		pPhysicsBody = PhysicsBody::createBox(Size(rect.getMaxX() * scale, rect.getMaxY() * scale), material, offset);
	}
	else if (type == circle)
	{
		pPhysicsBody = PhysicsBody::createCircle(rect.getMaxX() * scale, material, offset);
	}
	else
	{
		return nullptr;
	}

	pPhysicsBody->setTag(pSprite->getTag());
	pSprite->setScale(scale);
	pSprite->setPhysicsBody(pPhysicsBody);

	return pPhysicsBody;
}