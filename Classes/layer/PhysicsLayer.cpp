#include "layer/PhysicsLayer.h"

PhysicsLayer::PhysicsLayer(const Scene* const scene)
	: mParentScene(scene)
{
}

PhysicsLayer* PhysicsLayer::create(const Scene* const scene)
{
	PhysicsLayer* pLayer = new PhysicsLayer(scene);
	if (pLayer && pLayer->init())
	{
		pLayer->autorelease();

		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);

	return nullptr;
}

bool PhysicsLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	mWinSize = Director::getInstance()->getWinSize();
	mpTexture = Director::getInstance()->getTextureCache()->addImage("blocks.png");
	mpSpriteFrameCache = SpriteFrameCache::getInstance();

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	const Vec2 centre = visibleOrigin + visibleSize / 2;

	PhysicsBody* pTempPhysicsBody = nullptr;

	auto* boundary = Node::create();
	auto* boundaryBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	boundaryBody->setContactTestBitmask(0xFFFFFFFF);
	boundary->setPhysicsBody(boundaryBody);
	boundary->setPosition(centre);
	this->addChild(boundary);


	Rect rect1 = Rect(0, 0, 128, 32);
	auto* pSprite1 = SpriteFrame::create("hd/bogie.png", rect1);
	mpSpriteFrameCache->addSpriteFrame(pSprite1, "bogie");

	Rect rect2 = Rect(0, 0, 64, 64);
	auto* pSprite2 = SpriteFrame::create("hd/ball.png", rect2);
	mpSpriteFrameCache->addSpriteFrame(pSprite2, "bead");

	Rect rect3 = Rect(0, 0, 64, 24);
	auto* pSprite3 = SpriteFrame::create("hd/box.png", rect3);
	mpSpriteFrameCache->addSpriteFrame(pSprite3, "brick");

	Rect rect4 = Rect(0, 0, 64, 24);
	auto* pSprite4 = SpriteFrame::create("hd/Icon.png", rect3);
	mpSpriteFrameCache->addSpriteFrame(pSprite4, "item");

	float scale = 1.0f;
	mpBogie = Bogie::create(pSprite1, BOGIE_TAG);
	mpBogie->setPosition(Vec2(500, 100));
	pTempPhysicsBody = SpriteSetPhysicsBody(mpBogie, scale, rect1, box, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	pTempPhysicsBody->setDynamic(false);
	this->addChild(mpBogie);


	scale = 0.5f;
	mpBead = Bead::create(pSprite2, BEAD_TAG);
	mpBead->setPosition(Vec2(500, 132));
	pTempPhysicsBody = SpriteSetPhysicsBody(mpBead, scale, rect2, circle, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	pTempPhysicsBody->setGravityEnable(false);
	pTempPhysicsBody->setCategoryBitmask(0x02);
	pTempPhysicsBody->setCollisionBitmask(0x01);
	pTempPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
	//pTempPhysicsBody->setVelocity(Vec2(300, 600));
	this->addChild(mpBead);

	// 위 두 친구는 joint로 묶도록 하자.

	int8_t brickPlacement[8][16] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	constexpr int8_t widthCount = sizeof(brickPlacement) / sizeof(brickPlacement[0]);
	constexpr int8_t lengthCount = sizeof(brickPlacement) / widthCount;
	static_assert(widthCount == 8, "세로 벽돌은 8개이여야 함");
	static_assert(lengthCount == 16, "가로 벽돌은 16개이여야 함");


	scale = 1.0f;
	Size brickPosition = mWinSize - Size(0, 60);
	int32_t brickTagCount = 0;
	for (size_t i = 0; i < widthCount; ++i)
	{
		for (size_t j = 0; j < lengthCount; ++j)
		{
			if (brickPlacement[i][j] == 1)
			{
				int32_t tagNumber = 4 + brickTagCount++;
				auto* pBrick = Brick::create(pSprite3, tagNumber);
				pBrick->setAnchorPoint(Vec2(1, 1));
				pBrick->setPosition(brickPosition - Size(rect3.getMaxX() * scale * j, rect3.getMaxY() * scale * i));
				pTempPhysicsBody = SpriteSetPhysicsBody(pBrick, scale, rect3, box, PhysicsMaterial(0.1f, 1.0f, 0.0f));
				pTempPhysicsBody->setCategoryBitmask(0x02);
				pTempPhysicsBody->setCollisionBitmask(0x03);
				pTempPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
				pTempPhysicsBody->setDynamic(false);
				this->addChild(pBrick);
			}
		}
	}


	schedule(schedule_selector(PhysicsLayer::tick));

	log("width : %f, height : %f", mWinSize.width, mWinSize.height);
	//log("%d, %d, %d", pBogie->getTag(), pBead->getTag(), pBrick->getTag());

	return true;
}

PhysicsBody* PhysicsLayer::SpriteSetPhysicsBody(Sprite* const pSprite, const float scale, const Rect& rect, const ePhysicsBodyType type, const PhysicsMaterial& material, const Vec2& offset) const
{
	assert(pSprite != nullptr, "Sprite is Not NULL");
	assert(type != polygon, "an undefined type");

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

void PhysicsLayer::onEnter()
{
	Layer::onEnter();

	mpTouchListener = EventListenerTouchOneByOne::create();
	mpTouchListener->onTouchBegan = CC_CALLBACK_2(PhysicsLayer::onTouchBegan, this);
	mpTouchListener->onTouchMoved = CC_CALLBACK_2(PhysicsLayer::onTouchMoved, this);
	mpTouchListener->onTouchEnded = CC_CALLBACK_2(PhysicsLayer::onTouchEnded, this);
	mpTouchListener->onTouchCancelled = CC_CALLBACK_2(PhysicsLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpTouchListener, this);

}

void PhysicsLayer::onExit()
{
	Layer::onExit();

	_eventDispatcher->removeAllEventListeners();
}

void PhysicsLayer::tick(float deltaTime)
{
}

bool PhysicsLayer::onTouchBegan(Touch* touch, Event* event)
{
	auto location = touch->getLocation();
	auto arr = mParentScene->getPhysicsWorld()->getShapes(location);

	PhysicsBody* body = nullptr;
	for (auto& obj : arr)
	{
		log("%d", obj->getBody()->getTag());
		if ((obj->getBody()->getTag()) == BEAD_TAG)
		{
			body = obj->getBody();
			break;
		}
	}

	if (body != nullptr)
	{
		Node* mouse = Node::create();
		mouse->setPhysicsBody(PhysicsBody::create(0, 0));
		mouse->getPhysicsBody()->setDynamic(false);
		mouse->setPosition(location);
		this->addChild(mouse);
		PhysicsJointPin* joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
		joint->setMaxForce(5000.0f * body->getMass());
		mParentScene->getPhysicsWorld()->addJoint(joint);
		mMouseJoint.insert(std::make_pair(touch->getID(), mouse));

		return true;
	}
	else // Bead Collision Test
	{
		int32_t collisionBitmask = mpBead->getPhysicsBody()->getCollisionBitmask();
		if (collisionBitmask == 0x01)
		{
			mpBead->getPhysicsBody()->setCollisionBitmask(collisionBitmask << 1);
		}
		else
		{
			mpBead->getPhysicsBody()->setCollisionBitmask(collisionBitmask >> 1);
		}

		//this->addChild(pBrick);
	}

	return false;
}

void PhysicsLayer::onTouchMoved(Touch* touch, Event* event)
{
	auto it = mMouseJoint.find(touch->getID());

	if (it != mMouseJoint.end())
	{
		it->second->setPosition(touch->getLocation());
	}
}

void PhysicsLayer::onTouchEnded(Touch* touch, Event* event)
{
	auto it = mMouseJoint.find(touch->getID());

	if (it != mMouseJoint.end())
	{
		this->removeChild(it->second);
		mMouseJoint.erase(it);
	}
}