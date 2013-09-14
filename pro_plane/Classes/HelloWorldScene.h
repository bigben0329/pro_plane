//
//  HelloWorldScene.h
//  pro_plane
//
//  Created by momo@ben on 13-9-7.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"

#include "MapLogic.h"

class HelloWorld : public cocos2d::CCLayer ,public cocos2d::CCTextFieldDelegate{
public:
    HelloWorld();
    ~HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();

    //绘画续函数
    virtual void draw();
    
    //手指抬起事件
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //手指按下事件
    virtual void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event);
    //手指移动事件
    virtual void ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event);
    
    //飞机向右转回调函数
    void menuRotateRightCallback(CCObject* pSender);
    //飞机向左转回调函数
    void menuRotateLeftCallback(CCObject* pSender);
    //修改用户注册名
    void btnChangeNameCallback(CCObject* pSender);
    //查询在线用户列表
    void btnGetOnlineCallback(CCObject* pSender);
    //对战在线用户
    void btnVsCallback(CCObject* pSender);
    
    //当用户启动虚拟键盘时的回调函数
    virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender);
    //当用户关闭虚拟键盘时的回调函数
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender);
    //当用户进行输入时的回调函数
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender,const char *text,int nLen);
    //当用户删除文字时的回调函数
    virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender,const char *delText,int nLen);

    
public:
    
    //game logic
    std::string _cmd;
    std::string _response;
    
    //查询在线用户开关
    bool bsetOnlineLable = false;
    //发送炸弹开关
    bool bsetBoomPos = false;
    //发送炸弹结果开关
    bool bsetBoomStatPos = false;
    
private:
    
    //layer上添加node函数
    void AddChild(CCNode *pChild);
    int _zOrder;   //自动天假纵坐标 自增长
    
    //天假炸弹sprite
    void addBoom(int x, int y, std::string boom);

    //旋转飞机
    void ratotePlane();
    int _planeDer;
    
    
    //sprite mode
    cocos2d::CCSprite*    _plane_t;         //飞机sprite
    cocos2d::CCSprite*    _plane_d;         //飞机sprite
    cocos2d::CCSprite*    _plane_l;         //飞机sprite
    cocos2d::CCSprite*    _plane_r;         //飞机sprite
    cocos2d::CCSprite*    _boom;          //炸弹sprite
    cocos2d::CCLabelTTF*  _coor;          //炸弹坐标lable
    cocos2d::CCLabelTTF*  _labelOnline;   //在线列表label
    cocos2d::CCLayer*     _layerOnline;   //在线layer
    cocos2d::CCTextFieldTTF* _nameText;
    cocos2d::CCTextFieldTTF* _nameEnemy;
    cocos2d::CCMenu*       _pMenuCn;
    cocos2d::CCTexture2D*  _planecache;
    cocos2d::CCPoint _planePos;
    
    //sprite touch mode
    bool _boomTouched;         //炸弹碰触开关
    bool _planeTouched;        //飞机碰触开关
    
    //display mode
    float _scalex;             //横轴缩放比例
    float _scaley;             //纵轴缩放比例
    
    //map logic mode
    CMapLogic* _maplogic;
    float _fDeltaAngle;
    
    //text input
    
    //svr mode
    std::string _name;           //用户注册用户名
    std::string _request;        //用户请求request
    
    void setOnlineLable();
    void setBoomPos();
    void setBoomStatPos();
    
    //game logic
    void gameLogic(float dt);
    
};

#endif // __HELLO_WORLD_H__
