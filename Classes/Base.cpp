#include "Base.h"


Base::Base()
{
	//初始化
	_emitter = NULL;
	sprite = NULL;
}

Base::~Base()
{
	CC_SAFE_RELEASE(_emitter);
}
bool Base::init(){
	if (!Node::init()){
		return false;
	}
	//初始化Base类的成员  此处无用
	return true;
}
void Base::setSprite(Sprite *sprite){
	this->sprite = sprite;
	this->removeAllChildren();
	this->addChild(this->sprite, 1);
}
Sprite * Base::getSprite(){
	return sprite;
}
Size Base::getConSize(){
	return sprite->getContentSize();
}

void Base::initBody(){

}


