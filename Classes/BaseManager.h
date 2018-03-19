#ifndef __BaseManager__H__
#define __BaseManager__H__

#include "cocos2d.h"
#include "Cylinder.h"
#include <list>
USING_NS_CC;
using namespace std;

class BaseManager :public Node
{
public:
	BaseManager();
	~BaseManager();
	CREATE_FUNC(BaseManager);
	virtual bool init();
	virtual void update(float dt);
	list <Cylinder*> getCylinderList();
	void setMove(bool isMove);
private:
	void createCylinder();
	list <Cylinder*> cylinderList;
	//�Ƿ�Ҫ�ƶ�������β
	bool isMove;
	//�ؿ�
	int checkPoint;
};
#endif
