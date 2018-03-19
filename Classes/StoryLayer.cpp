#include "StoryLayer.h"
#include "Constants.h"
#include "resource.h"
#include "PlayScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
StoryLayer::StoryLayer(){
}



StoryLayer::~StoryLayer(){

}

Scene * StoryLayer::createScene(){
	auto scene = Scene::create();
	StoryLayer *layer = StoryLayer::create();
	scene->addChild(layer);
	return scene;

}
bool StoryLayer::init(){
	if (!Layer::init()){
		return false;
	}

	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//背景
	auto *spriteBG = Sprite::create(STORY_BG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	//菜单
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//开始菜单
	auto * menuCheckPoint = MenuItemSprite::create(Sprite::create(BEGIN_BUTTON), Sprite::create(BEGIN_BUTTON_PRESS)
		, CC_CALLBACK_1(StoryLayer::start, this));
	menuCheckPoint->setPosition(Point(centerPos.x, menuCheckPoint->getContentSize().height));
	menu->addChild(menuCheckPoint);

	this->addChild(menu);
	return true;
}
void StoryLayer::start(cocos2d::Ref* pSender){
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto mainScene = TransitionFade::create(1, MainScene::createScene());
	Director::getInstance()->replaceScene(mainScene);
}
//Ԥ预加载音乐
void StoryLayer::preLoadMusic(){
	SimpleAudioEngine * audioEngin = SimpleAudioEngine::getInstance();
	audioEngin->preloadBackgroundMusic(MUSIC_EFFECT);
	audioEngin->preloadBackgroundMusic(MUSIC_WIN);
	audioEngin->preloadBackgroundMusic(MUSIC_LOSE);
}

