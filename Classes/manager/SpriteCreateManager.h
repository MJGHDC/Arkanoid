#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Bogie;
class Bead;
class Brick;

class SpriteCreateManager
{
	enum ePhysicsBodyType
	{
		circle,
		box,
		polygon
	};

public:
	SpriteCreateManager();
	~SpriteCreateManager() = default;

	Bogie* CreateBogie();
	Bead* CreateBead(int32_t brickTagCount);
	Vector<Brick*> CreateBrick(const int8_t brickPlacement[8][16], int8_t widthCount, int8_t lengthCount);

	PhysicsBody* SpriteSetPhysicsBody(Sprite* const pSprite, const float scale, const Rect& rect, const ePhysicsBodyType type = box, const PhysicsMaterial& material = PHYSICSBODY_MATERIAL_DEFAULT, const Vec2& offset = Vec2::ZERO) const;

private:
	Size mWinSize;
	SpriteFrameCache* mpCache;
};