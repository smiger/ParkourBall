#ifndef __ENDLAYER_SCENE_H__
#define __ENDLAYER_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#ifdef __ANDROID__
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif
USING_NS_CC;
USING_NS_CC_EXT;

class EndLayer :public cocos2d::Layer
{
public:
	EndLayer();

	EndLayer(bool isWin, int percent);

	~EndLayer();

	static cocos2d::Scene* createScene(bool isWin,int percent);

	virtual bool init();

	CREATE_FUNC(EndLayer);

	void restart(cocos2d::Ref* pSender);

	void menu(cocos2d::Ref* pSender);

	void next(cocos2d::Ref* pSender);
	 // 插屏广告使用
	static void showSpotAd();   //通过JNI调用JAVA静态函数，实现展示插屏广告
	static bool closeSpotAd();  //通过JNI调用JAVA静态函数，实现关闭插屏广告
private :
	int checkPoint;
	bool isWin;
	int percent;
	LabelTTF* labelTTFCardNumber;
	LabelTTF* label;
};
#endif

