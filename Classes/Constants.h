#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__



#define GROUND_HEIGH 130

#define BEGIN_HEIGH 220

#define SPRITE_BEGIN_X 200

//初始化数量
#define cylinderNum 25
//圆盘tag
#define cylinderTag 2

//menu tag
#define playwayTag 12
//球tag
#define ballTag 1
//边界tag
#define edgeNodeTag 10
//Endlayer返回主界面tag
#define menuTag 1
//Endlayer下一关tag
#define nextTag 2
//Endlayer重新开始tag
#define restartTag 3

//关卡数
#define CHECK_POINT_NUM 6

enum CylinderColor{
	normal, red, blue, green,yellow, endColor
};

//关卡
#define checkPoint1 "checkPoint1"
#define checkPoint2 "checkPoint2"
#define checkPoint3 "checkPoint3"
#define checkPoint4 "checkPoint4"
#define checkPoint5 "checkPoint5"
#define checkPoint6 "checkPoint6"

//当前选择的关卡
#define curCheckPoint "currentCheckPoint"

#define songTime1 84
#define songTime2 108
#define songTime3 104
#define songTime4 81
#define songTime5 86
#define songTime6 109

#define CHECK_POINT_1 1
#define CHECK_POINT_2 2
#define CHECK_POINT_3 3
#define CHECK_POINT_4 4
#define CHECK_POINT_5 5
#define CHECK_POINT_6 6

//重力
#define gravity_1 -2500
#define gravity_2 -2500
#define gravity_3 -2500
#define gravity_4 -2550
#define gravity_5 -2200
#define gravity_6 -2300

//各个关卡跳跃速度的倍数
#define jumpImpulseTime_1 650
#define jumpImpulseTime_2 650
#define jumpImpulseTime_3 650
#define jumpImpulseTime_4 650
#define jumpImpulseTime_5 650
#define jumpImpulseTime_6 650

#endif
