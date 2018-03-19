#ifndef __Cylinder__H__
#define __Cylinder__H__

#include "Base.h"
#include "cocos2d.h"
#include "Constants.h"
USING_NS_CC;

class Cylinder :public Base
{
public:
	virtual bool init();
	//ŒÔ¿Ì∏’ÃÂ
	virtual void initBody();

	CREATE_FUNC(Cylinder);

	void setCylinderColor(CylinderColor color);

	CylinderColor getCylinderColor();
private:
	CylinderColor cylinderColor;

};
#endif
