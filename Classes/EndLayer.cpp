#include "EndLayer.h"
#include "Constants.h"
#include "resource.h"
#include "PlayScene.h"
#include "MainScene.h"
#include "LeaveLayer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
EndLayer::EndLayer(){
	checkPoint = 1;
	this->isWin = false;
	this->percent = 0;
	labelTTFCardNumber = NULL;
	label = NULL;
}

EndLayer::EndLayer(bool isWin, int percent){
	checkPoint = 1;
	this->isWin = isWin;
	this->percent = percent;
	labelTTFCardNumber = NULL;
	init();
}

EndLayer::~EndLayer(){

}

Scene * EndLayer::createScene(bool isWin, int percent){
	auto scene = Scene::create();
	EndLayer *layer = new EndLayer( isWin,  percent);
	scene->addChild(layer);
	return scene;

}
bool EndLayer::init(){
	if (!Layer::init()){
		return false;
	}

	int ad = CCUserDefault::sharedUserDefault()->getIntegerForKey("ad", 1);
	if ( ad%3 == 0){
		CCUserDefault::sharedUserDefault()->setIntegerForKey("ad", ad + 1);
		showSpotAd();
	}else{
		CCUserDefault::sharedUserDefault()->setIntegerForKey("ad", ad + 1);
	}


	//获取可见尺寸
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//获取关卡
	checkPoint = CCUserDefault::sharedUserDefault()->getIntegerForKey(curCheckPoint, 1);

	//创建一个背景精灵
	auto *spriteBG = Sprite::create(END_BG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	//创建菜单
	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//主界面
	auto * menuCheckPoint = MenuItemSprite::create(Sprite::createWithSpriteFrameName("menuNormal.png"), Sprite::createWithSpriteFrameName("menuPress.png")
		, CC_CALLBACK_1(EndLayer::menu, this));
	menuCheckPoint->setTag(menuTag);
	menuCheckPoint->setPosition(Point(centerPos.x - menuCheckPoint->getContentSize().width / 2, size.height / 3));
	menuCheckPoint->setScale(0.7);
	menu->addChild(menuCheckPoint);

	if (isWin){
		//胜利了而且不是最后一关  显示下一关按钮
		if (checkPoint != CHECK_POINT_NUM){
			auto * menuCheckPoint = MenuItemSprite::create(Sprite::createWithSpriteFrameName("nextNormal.png"), Sprite::createWithSpriteFrameName("nextPress.png")
				, CC_CALLBACK_1(EndLayer::next, this));
			menuCheckPoint->setTag(nextTag);
			menuCheckPoint->setPosition(Point(centerPos.x + menuCheckPoint->getContentSize().width / 2, size.height / 3));
			menuCheckPoint->setScale(0.7);
			menu->addChild(menuCheckPoint);
		}
		
	}
	else{
		//失败了  显示重新开始按钮
		auto * menuCheckPoint = MenuItemSprite::create(Sprite::createWithSpriteFrameName("reStartNormal.png"), Sprite::createWithSpriteFrameName("reStartPress.png")
			, CC_CALLBACK_1(EndLayer::restart, this));
		menuCheckPoint->setTag(restartTag);
		menuCheckPoint->setPosition(Point(centerPos.x + menuCheckPoint->getContentSize().width / 2, size.height / 3));
		menuCheckPoint->setScale(0.7);
		menu->addChild(menuCheckPoint);
	}
	

	//云朵
	auto *spriteCloud = Sprite::createWithSpriteFrameName("cloud.png");
	spriteCloud->setPosition(Point(size.width / 2, size.height/2 + spriteCloud->getContentSize().height / 2));
	this->addChild(spriteCloud);

	this->addChild(menu);

	//完成百分比
	labelTTFCardNumber = LabelTTF::create("0", "HiraKakuProN-w6", 35);
	labelTTFCardNumber->setColor(Color3B::GRAY);
	labelTTFCardNumber->setPosition(Point(spriteCloud->getPosition().x, spriteCloud->getPosition().y - labelTTFCardNumber->getContentSize().height/2));
	this->addChild(labelTTFCardNumber, 2);
	labelTTFCardNumber->setString(
		StringUtils::format("%d%%", percent));
	//提示语
	label = LabelTTF::create("0", "HiraKakuProN-w6", 20);
	label->setColor(Color3B::GRAY);
	label->setPosition(Point(spriteCloud->getPosition().x, spriteCloud->getPosition().y - labelTTFCardNumber->getContentSize().height - label->getContentSize().height));
	this->addChild(label, 2);
	if (isWin){
		label->setString("下一关更精彩！");
	}
	else{
		label->setString("就差一点点了！");
	}
	return true;
}
void EndLayer::restart(cocos2d::Ref* pSender){
	//判断是否播放音效
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto playScene = TransitionFade::create(1, PlayScene::createScene());
	Director::getInstance()->replaceScene(playScene);
}

void EndLayer::menu(cocos2d::Ref* pSender){
	//判断是否播放音效
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	auto mainScene = TransitionFade::create(1, MainScene::createScene());
	Director::getInstance()->replaceScene(mainScene);
}

void EndLayer::next(cocos2d::Ref* pSender){
	//判断是否播放音效
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	//设置为下一关
	CCUserDefault::sharedUserDefault()->setIntegerForKey(curCheckPoint, checkPoint + 1);
	auto playScene = TransitionFade::create(1, PlayScene::createScene());
	Director::getInstance()->replaceScene(playScene);
}



void EndLayer::showSpotAd()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台

    // 定义JNI函数信息结构体
    JniMethodInfo showSpotAd;

    bool isHave = JniHelper::getStaticMethodInfo(showSpotAd, "org/cocos2dx/cpp/AppActivity", "showSpotAd", "()V");
    if (!isHave) {
        log("jni:showSpotAd此函数不存在");
    }else{
        log("jni:showSpotAd函数存在");
        showSpotAd.env->CallStaticVoidMethod(showSpotAd.classID, showSpotAd.methodID);
    }
    #endif
}

bool EndLayer::closeSpotAd()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo closeSpotAd;
    bool isHave = JniHelper::getStaticMethodInfo(closeSpotAd, "org/cocos2dx/cpp/AppActivity", "closeSpotAd", "()Z");
    if (!isHave) {
        log("jni:closeSpotAd此函数不存在");
        return false;
    }else{
        log("jni:closeSpotAd函数存在");
        return closeSpotAd.env->CallStaticBooleanMethod(closeSpotAd.classID, closeSpotAd.methodID);
    }
    #endif
	return false;
}


