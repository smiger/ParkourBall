#include "MainScene.h"
#include "resource.h"
#include "PlayScene.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "Cocos2dx/ShareButton/UMShareButton.h"
#include "Cocos2dx/Common/CCUMTypeDef.h"

USING_NS_CC;
USING_NS_UM_SOCIAL;
using namespace CocosDenshion;

Scene * MainScene::_scene = NULL;

MainScene::MainScene(){
	ball = NULL;
	checkPoint = 1;
}
MainScene::~MainScene(){

}

Scene * MainScene::createScene(){
	//创建一个物理场景
	_scene = Scene::createWithPhysics();
	//_scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//设置物理世界的重力
	_scene->getPhysicsWorld()->setGravity(Vect(0, -1000));
	//添加层到物理世界中
	MainScene *layer = MainScene::create();
	_scene->addChild(layer);

	return _scene;

}
void shareCallback(int platform, int stCode, string& errorMsg)
{
    if ( stCode == 100 )
    {
        log("#### HelloWorld 开始分享");
    }
    else if ( stCode == 200 )
    {
        log("#### HelloWorld 分享成功");
    }
    else
    {
        log("#### HelloWorld 分享出错");
    }

    log("platform num is : %d.", platform);
}
bool MainScene::init(){
	if (!Layer::init()){
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	auto zeroPos = Director::getInstance()->getVisibleOrigin();
	auto centerPos = Point(size.width / 2, size.height / 2);

	//Ԥ预加载背景音乐
	preLoadMusic();

	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_plist_parkour, s_img_parkour);

	//背景
	auto *spriteBG = Sprite::create(MAIN_BG);
	spriteBG->setPosition(centerPos);
	this->addChild(spriteBG);

	auto *menu = Menu::create();
	menu->setPosition(zeroPos);
	//开始按钮
	auto * menuItemPLay = MenuItemSprite::create(Sprite::create(START_NORMAL), Sprite::create(START_PRESS), CC_CALLBACK_1(MainScene::onPlay, this));
	menuItemPLay->setPosition(centerPos.x,centerPos.y+30);
	menuItemPLay->setTag(0);
	menu->addChild(menuItemPLay);
	//显示关卡
	int num = CHECK_POINT_NUM + 1;
	for (int i = 1; i < num; i++){
		auto * menuCheckPoint = MenuItemSprite::create(Sprite::createWithSpriteFrameName("cylinder.png"), Sprite::createWithSpriteFrameName("cylinder_press.png")
			, CC_CALLBACK_1(MainScene::onPlay, this));
		menuCheckPoint->setTag(i);
		menuCheckPoint->setPosition(Point(size.width / num * i, size.height/5 ));
		menuCheckPoint->setScale(0.8);
		menu->addChild(menuCheckPoint);
		//数字
		String *str = String::createWithFormat("checkPoint%i.png", i);
		Sprite *sprite = Sprite::createWithSpriteFrameName(str->getCString());
		sprite->setPosition(Point(size.width / num * i, size.height/5  - sprite->getContentSize().height / 2 - menuCheckPoint->getContentSize().height / 2));
		this->addChild(sprite);

		//不可选择关卡加锁标志
		String *c_str = String::createWithFormat("checkPoint%i",i);
		if (i != 1 && !CCUserDefault::sharedUserDefault()->getBoolForKey(c_str->getCString(), false) ){
			Sprite *lockSprite = Sprite::createWithSpriteFrameName("lock.png");
			lockSprite->setPosition(Point(size.width / num * i, size.height/5 + lockSprite->getContentSize().height / 2 + menuCheckPoint->getContentSize().height / 2));
			this->addChild(lockSprite);
		}
	}
	

	//声音菜单
	auto soundOnMenuItem = MenuItemImage::create(
		SOUND_OPEN,
		SOUND_OPEN);
	auto soundOffMenuItem = MenuItemImage::create(
		SOUND_CLOSE,
		SOUND_CLOSE);
	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(MainScene::onPlay, this),
		soundOnMenuItem,
		soundOffMenuItem,
		NULL);
	soundToggleMenuItem->setAnchorPoint(ccp(0.5, 0.5));
	soundOnMenuItem->setScale(0.8);
	soundOffMenuItem->setScale(0.8);
	soundToggleMenuItem->setTag(100);
	soundToggleMenuItem->setPosition(
		Point(soundOnMenuItem->getContentSize().width / 2, soundOnMenuItem->getContentSize().height / 2));
	menu->addChild(soundToggleMenuItem);

	 // 获取一个CCUMSocialSDK实例
	    CCUMSocialSDK *sdk = CCUMSocialSDK::create("5513ab19fd98c546f70004b5");
	    // 设置微信的app id和app key
	    sdk->setWeiXinAppInfo("wxde1945ff7a43a0d9",
	            "c70ff7b551871a93f2562ce0086c1c1f");
	    sdk->setQQAppIdAndAppKey("1104461258", "OJWtuamgL6IYKmVH");
	    vector<int>* platform2 = new vector<int>();
	    platform2->push_back(WEIXIN);
	    platform2->push_back(WEIXIN_CIRCLE) ;
	    platform2->push_back(QQ);
	    platform2->push_back(QZONE) ;
	    sdk->setPlatforms(platform2) ;

	// 创建分享按钮, 参数1为按钮正常情况下的图片, 参数2为按钮选中时的图片,参数3为友盟appkey, 参数4为分享回调
	UMShareButton *shareButton = UMShareButton::create("res/share.png","res/share_press.png", "5513ab19fd98c546f70004b5", share_selector(shareCallback)) ;
	// 显示在友盟分享面板上的平台
	vector<int>* platforms = new vector<int>();
	platforms->push_back(WEIXIN);
	platforms->push_back(WEIXIN_CIRCLE) ;
	platforms->push_back(SINA);
	platforms->push_back(QZONE) ;
	platforms->push_back(QQ) ;
	platforms->push_back(RENREN) ;
	//platforms->push_back(DOUBAN) ;

	// 设置友盟分享面板上显示的平台
	shareButton->setPlatforms(platforms);
	// 设置文本分享内容
	shareButton->setShareContent("我在帮助史莱特重返色の星球，你也一起来吧！") ;
	// 设置要分享的图片, 图片支持本地图片和url图片, 但是url图片必须以http://或者https://开头
	if(saveScreenShot()){
		String  imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "screenshot.jpg";
		shareButton->setShareImage(imagePath.getCString()) ;
	}
	shareButton->setPosition(
		Point(size.width - soundOnMenuItem->getContentSize().width / 2, soundOnMenuItem->getContentSize().height / 2));
	shareButton->setScale(0.8);
	shareButton->setTag(101);
	menu->addChild(shareButton);

	this->addChild(menu);

	//声音开关
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){

		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_BACKGROUND, true);
		
		soundToggleMenuItem->setSelectedIndex(0);
	}
	else{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		soundToggleMenuItem->setSelectedIndex(1);
	}
	//初始化物理世界
	initPhysicWorld();
	//球
	ball = Ball::create();
	//设置位置
	ball->getSprite()->setPosition(size.width / (CHECK_POINT_NUM + 1) , size.height/2  + ball->getConSize().height / 2 );
	this->addChild(ball);
	//禁用一个物理世界的auto step,然后通过设定PhysicsWorld::step(time)来手动刷新PhysicsWorld
	_scene->getPhysicsWorld()->setAutoStep(false);
	scheduleUpdate();
	return true;
}
void MainScene::onPlay(cocos2d::Ref* pSender){
	//播放音效
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true)){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_EFFECT, false);
	}
	CCMenuItem* item = dynamic_cast<CCMenuItem*>(pSender);
	int tag = item->getTag();
	auto size = Director::getInstance()->getVisibleSize();
	int num = CHECK_POINT_NUM + 1;
	//开始
	if (tag == 0){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		
		CCUserDefault::sharedUserDefault()->setIntegerForKey(curCheckPoint, checkPoint);

		auto playScene = TransitionFade::create(1, PlayScene::createScene());
		Director::getInstance()->replaceScene(playScene);
	}
	//tag == 1到6 关卡选择
	else if (tag == 1){
		ball->initBody();
		ball->getSprite()->setPosition(size.width / num,  size.height/2  );
		checkPoint = 1;
	}
	else if (tag == 2){
		if (CCUserDefault::sharedUserDefault()->getBoolForKey(checkPoint2, false)){
			ball->initBody();
			ball->getSprite()->setPosition(size.width / num * 2,  size.height/2 );
			checkPoint = 2;
		}
	}
	else if (tag == 3){
		if (CCUserDefault::sharedUserDefault()->getBoolForKey(checkPoint3, false)){
			ball->initBody();
			ball->getSprite()->setPosition(size.width / num * 3,  size.height/2  );
			checkPoint = 3;
		}
	}
	else if (tag == 4){
		if (CCUserDefault::sharedUserDefault()->getBoolForKey(checkPoint4, false)){
			ball->initBody();
			ball->getSprite()->setPosition(size.width / num * 4,  size.height/2  );
			checkPoint = 4;
		}
	}
	else if (tag == 5){
		if (CCUserDefault::sharedUserDefault()->getBoolForKey(checkPoint5, false)){
			ball->initBody();
			ball->getSprite()->setPosition(size.width / num * 5,  size.height/2  );
			checkPoint = 5;
		}
	}
	else if (tag == 6){
		if (CCUserDefault::sharedUserDefault()->getBoolForKey(checkPoint6, false)){
			ball->initBody();
			ball->getSprite()->setPosition(size.width / num * 6,  size.height/2  );
			checkPoint = 6;
		}
	}
	//声音
	else if (tag == 100){
		if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			CCUserDefault::sharedUserDefault()->setBoolForKey("sound", false);
			CCUserDefault::sharedUserDefault()->flush();
		}
		else{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_BACKGROUND, true);
			CCUserDefault::sharedUserDefault()->setBoolForKey("sound", true);
			CCUserDefault::sharedUserDefault()->flush();
		}
	}
	
	

}

void MainScene::onEnter(){
	Layer::onEnter();
	//碰撞事件监听
	auto contactListenner = EventListenerPhysicsContact::create();
	contactListenner->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenner, this);
}
void MainScene::onEnterTransitionDidFinish(){
	Layer::onEnterTransitionDidFinish();
}
void MainScene::onExit(){
	Layer::onExit();
	//移除
	_eventDispatcher->removeEventListenersForTarget(this);

}
void MainScene::initPhysicWorld(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//创建一个静态的线状刚体
	auto body = PhysicsBody::createEdgeSegment(origin, Point(visibleSize.width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);
	//设置弹性  1-完全
	body->getShape(0)->setRestitution(1);
	//设置body是否受引力影响
	body->setGravityEnable(false);
	//类别标示掩码
	body->setCategoryBitmask(0x01);
	//碰撞掩码
	body->setCollisionBitmask(0x01);
	//接触测试掩码，表示哪些类别的物体“接触”时，会产生消息
	body->setContactTestBitmask(0x01);
	auto edgeNode = Node::create();
	edgeNode->setPosition(0, visibleSize.height/5);
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode, 2);
}

void MainScene::preLoadMusic(){
	SimpleAudioEngine * audioEngin = SimpleAudioEngine::getInstance();
	audioEngin->preloadBackgroundMusic(MUSIC_BACKGROUND);
}
bool MainScene::onContactBegin(PhysicsContact& contact){
	ball->rotateAction();
	return true;
}
void MainScene::update(float t){
	int steps = 3;
	float dt = CCDirector::getInstance()->getAnimationInterval() / (float)steps;
	PhysicsWorld* s = getScene()->getPhysicsWorld();
	for (int i = 0; i<steps; i++){
		s->step(dt);
	}
}
bool MainScene::saveScreenShot()

{
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("first", true)){
		return true;
	}

// ***************************************************************************

// 截屏

CCSize size = CCDirector::sharedDirector()->getWinSize();



log("Window Size, width = %d, height = %d.", (int)size.width, (int)size.height );

CCRenderTexture* texture = CCRenderTexture::create((int)size.width, (int)size.height, kCCTexture2DPixelFormat_RGBA8888);

texture->setPosition(ccp(size.width/2, size.height/2));

texture->begin();

CCDirector::sharedDirector()->getRunningScene()->visit();

texture->end();



string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() ;

if ( imagePath.length() == 0 )

{

return false;

}



// imagePath = imagePath.c_str() ;



char imgName[20] = "screenshot.jpg" ;

//保存为jpeg

bool result = texture->saveToFile(imgName, kCCImageFormatJPEG);

if ( result )

{

// imagePath += "screenshot.png";

imagePath += imgName ;

}
CCUserDefault::sharedUserDefault()->setBoolForKey("first", false);
return true;
}

