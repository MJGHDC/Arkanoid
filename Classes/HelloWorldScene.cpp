/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::scene = nullptr;

Scene* HelloWorld::createScene()
{
	//auto scene = Scene::createWithPhysics();
	scene = Scene::createWithPhysics();

	// set gravity
	//scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f)); // 중력 가속도
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -900.0f));

	// optional: set debug draw
	// scene->getPhysicsWorld()->setDebugDrawMask(0xffff);
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = HelloWorld::create();
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	mWinSize = Director::getInstance()->getWinSize();
	mTexture = Director::getInstance()->getTextureCache()->addImage("blocks.png");
	spriteFrameCache = SpriteFrameCache::getInstance();

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	const Vec2 centre = visibleOrigin + visibleSize / 2;

	auto* boundary = Node::create();
	auto* boundaryBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	boundary->setPhysicsBody(boundaryBody);
	boundary->setPosition(centre);
	this->addChild(boundary);


	Rect rect1 = Rect(0, 0, 128, 32);
	auto* pSprite1 = SpriteFrame::create("hd/bogie.png", rect1);
	spriteFrameCache->addSpriteFrame(pSprite1, "bogie");

	Rect rect2 = Rect(0, 0, 64, 64);
	auto* pSprite2 = SpriteFrame::create("hd/ball.png", rect2);
	spriteFrameCache->addSpriteFrame(pSprite1, "bead");

	Rect rect3 = Rect(0, 0, 64, 24);
	auto* pSprite3 = SpriteFrame::create("hd/box.png", rect3);
	spriteFrameCache->addSpriteFrame(pSprite1, "brick");

	PhysicsBody* pTempPhysicsBody = nullptr;

	float scale = 1.0f;
	pBogie = Bogie::create(pSprite1, 0);
	pBogie->setPosition(Vec2(500, 100));
	pTempPhysicsBody = SpriteSetPhysicsBody(pBogie, scale, rect1, box, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	pTempPhysicsBody->setDynamic(false);
	this->addChild(pBogie);


	scale = 0.5f;
	pBead = Bead::create(pSprite2, 1);
	pBead->setPosition(Vec2(500, 132));
	SpriteSetPhysicsBody(pBead, scale, rect2, circle, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	this->addChild(pBead);

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
	for (size_t i = 0; i < widthCount; ++i)
	{
		for (size_t j = 0; j < lengthCount; ++j)
		{
			if (brickPlacement[i][j] == 1)
			{
				auto* pBrick = Brick::create(pSprite3, 2);
				pBrick->setAnchorPoint(Vec2(1, 1));
				pBrick->setPosition(brickPosition - Size(rect3.getMaxX() * scale * j, rect3.getMaxY() * scale * i));
				pTempPhysicsBody = SpriteSetPhysicsBody(pBrick, scale, rect3, box, PhysicsMaterial(0.1f, 1.0f, 0.0f));
				pTempPhysicsBody->setDynamic(false);
				this->addChild(pBrick);
			}			
		}
	}
	

	schedule(schedule_selector(HelloWorld::tick));

	log("width : %f, height : %f", mWinSize.width, mWinSize.height);
	//log("%d, %d, %d", pBogie->getTag(), pBead->getTag(), pBrick->getTag());

	return true;
}

PhysicsBody* HelloWorld::SpriteSetPhysicsBody(Sprite* pSprite, const float scale, const Rect& rect, const ePhysicsBodyType type, const PhysicsMaterial& material, const Vec2& offset)
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

void HelloWorld::onEnter()
{
	Scene::onEnter();
}

void HelloWorld::onExit()
{
	Scene::onExit();
}

void HelloWorld::tick(float deltaTime)
{
}
