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
//#include "SvrLogic.h"

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

class HelloWorld : public cocos2d::CCLayer ,public cocos2d::CCTextFieldDelegate{
public:
    HelloWorld();
    ~HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();

    virtual void draw();
    void update(float dt);
    
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event);
    
    void menuRotateRightCallback(CCObject* pSender);
    void menuRotateLeftCallback(CCObject* pSender);
    void btnChangeNameCallback(CCObject* pSender);
    void btnGetOnlineCallback(CCObject* pSender);
    
    
    //当用户启动虚拟键盘时的回调函数
    virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender);
    //当用户关闭虚拟键盘时的回调函数
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender);
    //当用户进行输入时的回调函数
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender,const char *text,int nLen);
    //当用户删除文字时的回调函数
    virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender,const char *delText,int nLen);
    
    void setOnlineLable(const std::string content);

private:
    
    //plane mode
    cocos2d::CCSprite* _plane;
    bool _planeTouched;
    
    //boom mode
    cocos2d::CCSprite* _boom;
    cocos2d::CCLabelTTF* _coor;
    bool _boomTouched;
    
    //display mode
    float _scalex;
    float _scaley;
    
    //map logic mode
    CMapLogic* _maplogic;
    float _fDeltaAngle;
    
    //text input
    cocos2d::CCTextFieldTTF* _nameText;
    cocos2d::CCTextFieldTTF* _nameEnemy;
    cocos2d::CCMenu *_pMenuCn;
    
    //svr mode
    std::string _name;
    std::string _request;
    
    //online mode
    cocos2d::CCLabelTTF* _labelOnline;
    cocos2d::CCLayer* _layerOnline;
};

#endif // __HELLO_WORLD_H__
