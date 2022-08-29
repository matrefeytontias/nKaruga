#include "helpers/Joint.hpp"

#include "helpers/math.hpp"
#include "entities/BossEnemy.hpp"
#include "entities/Enemy.hpp"
#include "entities/Entity.hpp"

Joint::Joint()
{
	target = NULL;
	targetX = targetY = targetCX = targetCY = jointX = jointY = jointCX = jointCY = 0;
}

Joint::~Joint()
{

}

void Joint::activate(Entity *_target, int _targetX, int _targetY, int _jointX, int _jointY, int _jointCX, int _jointCY, unsigned short *_timg, unsigned short *_jimg)
{
	target = _target;
	timg = _timg;
	jimg = _jimg;
	targetX = _targetX;
	targetY = _targetY;
	targetCX = timg[0] / 2;
	targetCY = timg[1] / 2;
	jointX = _jointX;
	jointY = _jointY;
	jointCX = _jointCX == -1 ? jimg[0] / 2 : _jointCX;
	jointCY = _jointCY == -1 ? jimg[1] / 2 : _jointCY;
}

void Joint::activate(Entity *_target, int _targetX, int _targetY, int _targetCX, int _targetCY, int _jointX, int _jointY, int _jointCX, int _jointCY, unsigned short *_timg, unsigned short *_jimg)
{
	target = _target;
	targetX = _targetX;
	targetY = _targetY;
	targetCX = _targetCX;
	targetCY = _targetCY;
	jointX = _jointX;
	jointY = _jointY;
	timg = _timg;
	jimg = _jimg;
	jointCX = _jointCX == -1 ? jimg[0] / 2 : _jointCX;
	jointCY = _jointCY == -1 ? jimg[1] / 2 : _jointCY;
}

// Gives the X coordinate of the rotation center of the jointed image
Fixed Joint::getx()
{
	if (!target->isBoss && !target->isEnemy)
	{
		return target->getx() + itofix(targetX - timg[0] / 2 - jointX + jointCX);
	}
	else
	{
		Fixed angle = 0, x;
		if (target->isBoss)
		{
			BossEnemy *be = (BossEnemy*)target;
			x = be->getx();
			angle = be->angle;
		}
		else
		{
			Enemy *e = (Enemy*)target;
			x = e->getx();
			if (e->hasRotation)
			{
				angle = e->rotationAngle;
			}
		}
		Fixed cx = (targetX - targetCX) * fixcos(angle) + (targetY - targetCY) * fixsin(angle);
		return x + cx + itofix(-jointX + jointCX);
	}
}

// Gives the Y coordinate of the rotation center of the jointed image
Fixed Joint::gety()
{
	if (!target->isBoss && !target->isEnemy)
	{
		return target->gety() + itofix(targetY - timg[1] / 2 - jointY + jointCY);
	}
	else
	{
		Fixed angle = 0, y;
		if (target->isBoss)
		{
			BossEnemy *be = (BossEnemy*)target;
			y = be->gety();
			angle = be->angle;
		}
		else
		{
			Enemy *e = (Enemy*)target;
			y = e->gety();
			if (e->hasRotation)
				angle = e->rotationAngle;
		}
		Fixed cy = -(targetX - targetCX) * fixsin(angle) + (targetY - targetCY) * fixcos(angle);
		return y + cy + itofix(-jointY + jointCY);
	}
}
