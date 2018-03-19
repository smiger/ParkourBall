#include "EndNotLeave.h"
#include "Constants.h"
#include "resource.h"
#include "PlayScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
EndNotLeave::EndNotLeave(){
}



EndNotLeave::~EndNotLeave(){

}

Scene * EndNotLeave::createScene(){
	auto scene = Scene::create();
	EndNotLeave *layer = EndNotLeave::create();
	scene->addChild(layer);
	return scene;

}
bool EndNotLeave::init(){
	if (!Layer::init()){
		return false;
	}
	//获取可见尺寸
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//创建一个背景精灵
	auto *spriteBG = Sprite::create(END_NOTLEAVE);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	//创建菜单
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//按钮
	auto * menuCheckPoint = MenuItemSprite::create(Sprite::create(MENU_NORMAL_SMALL), Sprite::create(MENU_PRESS_SMALL)
		, CC_CALLBACK_1(EndNotLeave::menu, this));
	menuCheckPoint->setPosition(Point(menuCheckPoint->getContentSize().width/2, size.height - menuCheckPoint->getContentSize().height/2));
	
	menu->addChild(menuCheckPoint);

	this->addChild(menu);

	//粒子效果
	ParticleSystem *_emitter = ParticleRain::create();
	_emitter->setAutoRemoveOnFinish(true);
	_emitter->setPosition(size.width / 2, size.height);
	this->addChild(_emitter, 5);
	return true;
}
void EndNotLeave::menu(cocos2d::Ref* pSender){
	//判断是否播放音效
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto mainScene = TransitionFade::create(1, MainScene::createScene());
	Director::getInstance()->replaceScene(mainScene);
}

