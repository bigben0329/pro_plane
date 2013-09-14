//
//  HelloWorldScene.cpp
//  pro_plane
//
//  Created by momo@ben on 13-9-7.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "SvrLogic.h"

using namespace cocos2d;
using namespace CocosDenshion;


#define PTM_RATIO 32

#define COOR_BEGIN_X 28
#define COOR_BEGIN_Y 15
#define COOR_MAP_WIDTH 322
#define COOR_MAP_HEIGHT 275

#define PLANE_DER_TOP   "plane_t.png"
#define PLANE_DER_DOWN  "plane_d.png"
#define PLANE_DER_RIGHT "plane_r.png"
#define PLANE_DER_LEFT  "plane_l.png"

#define DER_TOP   0
#define DER_DOWN  2
#define DER_RIGHT 1
#define DER_LEFT  3

CSvrLogic* g_svr;

enum {
    kTagParentNode = 1,
};

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );      //启动touch事件
    setAccelerometerEnabled( false );      //启动重力感应
    
    _boomTouched = false;
    _planeTouched = false;
    _name = "ben5";
    _request = "test";
    _zOrder = 0;
    _planeDer = DER_TOP;
    _planePos = CCPointZero;
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    

    //add background png
    CCSprite* pSprbackground = CCSprite::create("background.png");
    pSprbackground->setPosition( ccp(s.width/2, s.height/2) );
    pSprbackground->setScaleX(s.width/pSprbackground->getContentSize().width);
    pSprbackground->setScaleY(s.height/pSprbackground->getContentSize().height);
    AddChild(pSprbackground);
    
    //add map picture
    CCSprite* pTable = CCSprite::create("table11x13.png");
    _scalex = s.width/pTable->getContentSize().width;
    _scaley = s.height/pTable->getContentSize().height;
    pTable->setPosition( ccp(s.width/2, s.height/2) );
    pTable->setScaleX(_scalex);
    pTable->setScaleY(_scaley);
    AddChild(pTable);

    //add logo lable
    CCLabelTTF *logo = CCLabelTTF::create("正牌打飞机", "Marker Felt", 20*_scalex);
    logo->setColor(ccc3(0,0,255));
    logo->setPosition(ccp( s.width - logo->getContentSize().width/2-10*_scalex, s.height-50*_scalex));
    AddChild(logo);
    
    // add plane picture CCRect
    _planecache = CCTextureCache::sharedTextureCache()->addImage("plane_full.png");
    
    _plane_t = CCSprite::createWithTexture(_planecache, CCRectMake(0, 0, 125, 100));
    _plane_t->setScaleX(_scalex);
    _plane_t->setScaleY(_scaley);
    _plane_t->setPosition(ccp((s.width - _plane_t->getContentSize().width/2*_scalex),
                            (_plane_t->getContentSize().height/2*_scaley + 110*_scaley)));
    AddChild(_plane_t);
    _plane_t->setVisible(true);
    
    _plane_d = CCSprite::createWithTexture(_planecache, CCRectMake(125, 0, 125, 100));
    _plane_d->setScaleX(_scalex);
    _plane_d->setScaleY(_scaley);
    _plane_d->setPosition(ccp((s.width - _plane_d->getContentSize().width/2*_scalex),
                            (_plane_d->getContentSize().height/2*_scaley + 110*_scaley)));
    AddChild(_plane_d);
    _plane_d->setVisible(false);
    
    _plane_l = CCSprite::createWithTexture(_planecache, CCRectMake(0, 100, 100, 125));
    _plane_l->setScaleX(_scalex);
    _plane_l->setScaleY(_scaley);
    _plane_l->setPosition(ccp((s.width - _plane_l->getContentSize().width/2*_scalex),
                            (_plane_l->getContentSize().height/2*_scaley + 110*_scaley)));
    AddChild(_plane_l);
    _plane_l->setVisible(false);
    
    _plane_r = CCSprite::createWithTexture(_planecache, CCRectMake(150, 101, 100, 120));
    _plane_r->setScaleX(_scalex);
    _plane_r->setScaleY(_scaley);
    _plane_r->setPosition(ccp((s.width - _plane_r->getContentSize().width/2*_scalex),
                            (_plane_r->getContentSize().height/2*_scaley + 110*_scaley)));
    AddChild(_plane_r);
    _plane_r->setVisible(false);
    
    
    //add rotate mune
    CCMenuItemImage *protateRight = CCMenuItemImage::create("rotare_r.png","rotare_r.png",
                            this,menu_selector(HelloWorld::menuRotateRightCallback) );
    protateRight->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 35*_scalex, 80*_scaley) );
    CCMenu* pMenuRight = CCMenu::create(protateRight, NULL);
    pMenuRight->setPosition( CCPointZero );
    AddChild(pMenuRight);
    
    //add rotate mune
    CCMenuItemImage *protateLeft = CCMenuItemImage::create("rotare_l.png","rotare_l.png",
                             this,menu_selector(HelloWorld::menuRotateLeftCallback));
    protateLeft->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 90*_scalex, 80*_scaley) );
    CCMenu* pMenuLeft = CCMenu::create(protateLeft, NULL);
    pMenuLeft->setPosition( CCPointZero );
    AddChild(pMenuLeft);

    // add boom
    _boom = CCSprite::create("boom.png");
    _boom->setScaleX(_scalex);
    _boom->setScaleY(_scaley);
    _boom->setPosition(ccp((s.width - _boom->getContentSize().width/2*_scalex- 90*_scalex),
                            (_boom->getContentSize().height/2*_scaley + 20)));
    AddChild(_boom);

    //init coor label
    _coor = CCLabelTTF::create("炸弹坐标", "Marker Felt", 45*_scalex);
    _coor->setVisible(false);
    _coor->setColor(ccc3(0,0,7));
    AddChild(_coor);
    
    //init map logic
    _maplogic = new CMapLogic(CCRectMake(COOR_BEGIN_X,COOR_BEGIN_Y,COOR_MAP_WIDTH,COOR_MAP_HEIGHT), 13, 11);
    _maplogic->setScale(_scalex, _scaley);
    _fDeltaAngle = 0;
    
    //add text input
    _nameText = CCTextFieldTTF::textFieldWithPlaceHolder(_name.c_str(), "Thonburi",18*_scalex);
    _nameText->setPosition(ccp(s.width - _nameText->getContentSize().width/2-100,
                               s.height-15*_scaley));
    _nameText->setDelegate(this);
    _nameText->setColor(ccc3(0,0,7));
    AddChild(_nameText);
    
    //add change name button
    CCMenuItemImage *pbtn_cn = CCMenuItemImage::create("btn_cn0.png", "btn_cn.png", this,
                               menu_selector(HelloWorld::btnChangeNameCallback));
    pbtn_cn->setPosition( ccp(_nameText->getPosition().x-60*_scalex, _nameText->getPosition().y) );
    _pMenuCn = CCMenu::create(pbtn_cn, NULL);
    _pMenuCn->setPosition( CCPointZero );
    pbtn_cn->setScale(_scalex);
    pbtn_cn->setVisible(true);
    AddChild(_pMenuCn);
    
    //add get online button
    CCMenuItemImage *pbtn_ol = CCMenuItemImage::create("btn_online.png", "btn_online.png", this,
                                menu_selector(HelloWorld::btnGetOnlineCallback));
    pbtn_ol->setPosition( ccp(60*_scalex, _nameText->getPosition().y) );
    pbtn_ol->setScale(_scalex);
    CCMenu* _pMenuOnline = CCMenu::create(pbtn_ol, NULL);
    _pMenuOnline->setPosition( CCPointZero );
    AddChild(_pMenuOnline);
    
    
    //add vs button
    CCMenuItemImage *pbtn_vs = CCMenuItemImage::create("btn_vs.png", "btn_vs.png", this,
                                                       menu_selector(HelloWorld::btnVsCallback));
    pbtn_vs->setPosition(ccp( pbtn_ol->getPositionX()+100*_scalex, pbtn_ol->getPositionY()));
    pbtn_vs->setScale(_scalex);
    CCMenu* _pMenuVs = CCMenu::create(pbtn_vs, NULL);
    _pMenuVs->setPosition( CCPointZero );
    AddChild(_pMenuVs);
    
    
    //add enemy text input
    _nameEnemy = CCTextFieldTTF::textFieldWithPlaceHolder("对手", "Thonburi",18*_scalex);
    _nameEnemy->setPosition(ccp( pbtn_vs->getPositionX()+55*_scalex, pbtn_vs->getPositionY()));
    _nameEnemy->setDelegate(this);
    _nameEnemy->setColor(ccc3(0,0,7));
    AddChild(_nameEnemy);
    
    //init fire label
    CCSize size;
    _maplogic->getItemSize(size.width, size.height);
    
    //add _layerOnline
    _layerOnline = new CCLayer();
    _layerOnline->setPosition(s.width/2, s.height/2);
    _layerOnline->setVisible(false);
    AddChild(_layerOnline);
    
    //add background png
    CCSprite* pspbg = CCSprite::create("alert_background.png");
    pspbg->setPosition(CCPointZero );
    pspbg->setOpacity(100);
    pspbg->setScaleX(0.5f);
    pspbg->setScaleY(0.5f);
    _layerOnline->addChild(pspbg, 0);
    
    _labelOnline = CCLabelTTF::create("AAA", "Marker Felt", 15*_scalex);
    _labelOnline->setColor(ccc3(125,70,50));
    _labelOnline->setPosition(CCPointZero);
    _layerOnline->addChild(_labelOnline, 2);
    
    
    //init svr logic
    g_svr = new CSvrLogic();
    g_svr->regScene(this);
    g_svr->initSvr("106.187.89.124", 3490);
    //g_svr->initSvr("172.25.32.144", 3490);
    //g_svr->initSvr("127.0.0.1", 3490);
    g_svr->doSvrCmd("reg", _name);
    
    this->schedule( schedule_selector(HelloWorld::gameLogic) );
    scheduleUpdate();
    
}

void HelloWorld::gameLogic(float dt)
{
    //在线列表
    if( bsetOnlineLable )
    {
        bsetOnlineLable = false;
        setOnlineLable();
    }
    
    //炸弹打击
    if( bsetBoomPos )
    {
        bsetBoomPos = false;
        setBoomPos();
    }
    
    //炸弹结果
    if( bsetBoomStatPos )
    {
        bsetBoomStatPos = false;
        setBoomStatPos();
    }
    
    
}


HelloWorld::~HelloWorld()
{
    if( _maplogic )
    {
        delete _maplogic;
        _maplogic = NULL;
    }
    
    if( g_svr )
    {
        delete g_svr;
        g_svr = NULL;
    }
    
    //delete m_debugDraw;
}

void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    //world->DrawDebugData();

    kmGLPopMatrix();
}


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}


//-------------------------------------------------------------------------------
void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
    //获取触点集合
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    
    //获得针对屏幕的坐标
    CCPoint location = pTouch->getLocationInView();
    location.y = (location.y * (-1)) + CCDirector::sharedDirector()->getWinSize().height;

    //计算飞机碰触矩形
    CCRect* _planeTextureRect;
    
    switch (_planeDer) {
        case DER_TOP:
            _planeTextureRect = new CCRect(_plane_t->getPositionX() - _plane_t->getContentSize().width/2*_scalex,_plane_t->getPositionY() - _plane_t->getContentSize().height/2*_scaley,_plane_t->getContentSize().width*_scalex, _plane_t->getContentSize().height*_scaley);
            break;
            
        case DER_DOWN:
            _planeTextureRect = new CCRect(_plane_d->getPositionX() - _plane_d->getContentSize().width/2*_scalex,_plane_d->getPositionY() - _plane_d->getContentSize().height/2*_scaley,_plane_d->getContentSize().width*_scalex, _plane_d->getContentSize().height*_scaley);
            break;
            
        case DER_RIGHT:
            _planeTextureRect = new CCRect(_plane_r->getPositionX() - _plane_r->getContentSize().width/2*_scalex,_plane_r->getPositionY() - _plane_r->getContentSize().height/2*_scaley,_plane_r->getContentSize().width*_scalex, _plane_r->getContentSize().height*_scaley);
            break;
            
        case DER_LEFT:
            _planeTextureRect = new CCRect(_plane_l->getPositionX() - _plane_l->getContentSize().width/2*_scalex,_plane_l->getPositionY() - _plane_l->getContentSize().height/2*_scaley,_plane_l->getContentSize().width*_scalex, _plane_l->getContentSize().height*_scaley);
            break;
            
        default:
            break;
    }
    
    
    //计算炸弹碰触矩形
    CCRect* _boomTextureRect = new CCRect(_boom->getPositionX() - _boom->getContentSize().width/2*_scalex,
                                           _boom->getPositionY() - _boom->getContentSize().height/2*_scaley,
                                           _boom->getContentSize().width*_scalex, _boom->getContentSize().height*_scaley);
    
    for (CCSetIterator iter = pTouches->begin(); iter != pTouches->end(); ++iter)
    {
        if (_planeTextureRect->containsPoint(location))
        {
            CCLog("Touched Plane Sprite");
            _planeTouched = true;
        }
        
        if (_boomTextureRect->containsPoint(location))
        {
            CCLog("Touched Boom Sprite");
            _boomTouched = true;
        }
        
    }
}

void HelloWorld::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
    //获取触点集合
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    
    //获得针对屏幕的坐标
    CCPoint location = pTouch->getLocationInView();
    location.y = (location.y * (-1)) + CCDirector::sharedDirector()->getWinSize().height;
    CCPoint realPosition;
    
    for (CCSetIterator iter = pTouches->begin(); iter != pTouches->end(); ++iter)
    {
        if (_planeTouched == true)
        {
//            _maplogic->getMapPosition(_plane, location, realPosition);
//            _plane->setPosition(realPosition);
            
            switch (_planeDer) {
                case DER_TOP:
                    _maplogic->getMapPosition(_plane_t, location, realPosition);
                    _plane_t->setPosition(realPosition);
                    _planePos = _plane_t->getPosition();
                    break;
                    
                case DER_DOWN:
                    _maplogic->getMapPosition(_plane_d, location, realPosition);
                    _plane_d->setPosition(realPosition);
                    _planePos = _plane_d->getPosition();
                    break;
                    
                case DER_RIGHT:
                    _maplogic->getMapPosition(_plane_r, location, realPosition);
                    _plane_r->setPosition(realPosition);
                    _planePos = _plane_r->getPosition();
                    break;
                    
                case DER_LEFT:
                    _maplogic->getMapPosition(_plane_l, location, realPosition);
                    _plane_l->setPosition(realPosition);
                    _planePos = _plane_l->getPosition();
                    break;
                    
                default:
                    break;
            }
        }
        
        
        if (_boomTouched == true)
        {
            _maplogic->getMapPosition(_boom, location, realPosition);
            float itemWidth = 0, itemHeigth = 0;
            _maplogic->getItemSize(itemWidth, itemHeigth);
            _boom->setPosition(realPosition);
            realPosition.x += (2*itemWidth*_scalex);
            realPosition.y += (2*itemHeigth*_scaley);
            _coor->setVisible(true);
            
            int coorX = 0, coorY = 0;
            _maplogic->getCoordinate(coorX, coorY);
            char szCoor[256] = {0};
            snprintf(szCoor, sizeof(szCoor), "(%d,%d)", coorX, coorY);
            CCLOG("szCoor %s", szCoor);
            _coor->setString(szCoor);
            _coor->setPosition(realPosition);
        }

    }
}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
    if (_planeTouched == true)
    {
        _planeTouched = false;
        _maplogic->updateMap(_planeDer);
        _maplogic->displayMap();
    }
    
    if (_boomTouched == true)
    {
        _boomTouched = false;
        _coor->setVisible(false);
        
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        _boom->setPosition(ccp((s.width - _boom->getContentSize().width/2*_scalex- 90*_scalex),
                               (_boom->getContentSize().height/2*_scaley + 20)));
        
        int coorX = 0, coorY = 0;
        _maplogic->getCoordinate(coorX, coorY);
        
        char pos[64] = {0};
        snprintf(pos, sizeof(pos), "{\"x\":%d,\"y\":%d}", coorX, coorY);
        g_svr->doSvrCmd("hit", pos);
    }
    
}


//-------------------------------------------------------------------------------
void HelloWorld::menuRotateRightCallback(CCObject* pSender)
{
    _planeDer++;
    ratotePlane();
}


void HelloWorld::menuRotateLeftCallback(CCObject* pSender)
{
    _planeDer--;
    ratotePlane();
}


void HelloWorld::btnChangeNameCallback(CCObject* pSender)
{
    _nameText->attachWithIME();
}


void HelloWorld::btnGetOnlineCallback(CCObject* pSender)
{
    if( !_layerOnline->isVisible() )
    {
        _layerOnline->setVisible(true);
        g_svr->regScene(this);
        g_svr->doSvrCmd("onlineinfo", "ben");

    }
    else
    {
        _layerOnline->setVisible(false);
    }
}


void HelloWorld::btnVsCallback(CCObject* pSender)
{
    _nameEnemy->attachWithIME();
}


//-------------------------------------------------------------------------------
void HelloWorld::setOnlineLable()
{
    bsetOnlineLable = false;
    CCLOG("setOnlineLable begin content %s", _response.c_str());
    if( _labelOnline )
    {
        _labelOnline->setString(_response.c_str());
    }
    else
    {
        CCLOG("_labelOnline is null");
    }
    CCLOG("setOnlineLable end content %s", _labelOnline->getString());

}


void HelloWorld::setBoomPos()
{
    bsetBoomPos = false;
    CCLOG("setBoomPos content |%s|", _response.c_str());
    
    int x= 0 , y = 0 ;
    sscanf(_response.c_str(),"{\"x\":%d,\"y\":%d}", &x, &y);
    CCLOG("x %d y %d _scalex %f _scaley %f", x, y, _scalex, _scaley);
    
    std::string boom = "fire_r.png";
    addBoom(x, y, boom);
    
    FIGHT_STATUS stat = _maplogic->hitMap(x, y);
    char cStat[128] = {0};
    snprintf(cStat, sizeof(cStat), "{\"x\":%d,\"y\":%d,\"stat\":%d}", x, y, stat);
    g_svr->doSvrCmd("hitstat", std::string(cStat));
    
    
    if( FIGHT_STATUS_WIN == stat )
    {
        alert("You Lost!");
    }
}

void HelloWorld::setBoomStatPos()
{
    CCLOG("setBoomPos content |%s|", _response.c_str());
    
    int x= 0, y = 0, stat = 0;
    int ret = sscanf(_response.c_str(),"{\"x\":%d,\"y\":%d,\"stat\":%d}", &x, &y, &stat);
    CCLOG("ret %d x %d y %d stat %d", ret, x, y, stat);
    
    std::string boom;
    switch (stat) {
        case FIGHT_STATUS_WIN:
        {
            boom = "fire_o.png"; 
            break;
        }
        case FIGHT_STATUS_HURT:
            boom = "fire_r.png";
            break;
        case FIGHT_STATUS_MISS:
            boom = "fire_y.png";
            break;
            
        default:
            boom = "fire_y.png";
            break;
    }
    
    addBoom(x, y, boom);
    
    if( FIGHT_STATUS_WIN == stat )
    {
        alert("You Win!");
    }
}


void HelloWorld::nodeDisp(CCNode* sender)
{
    sender->setVisible(false);
}


void HelloWorld::alert(const char* msg)
{
    _labelOnline->setString(msg);
    _layerOnline->setVisible(true);
}

//-------------------------------------------------------------------------------
void HelloWorld::AddChild(CCNode *pChild)
{
    this->addChild(pChild, _zOrder);
    _zOrder++;
}


void HelloWorld::addBoom(int x, int y, std::string boom)
{
    CCLOG("addBoom x %d y %d boom %s", x, y, boom.c_str());
    
    CCSize size;
    _maplogic->getItemSize(size.width, size.height);
    CCLOG("width %f height %f", size.width, size.height);
    
    CCSprite* fire = CCSprite::create(boom.c_str());
    CCPoint location(x, y), realPosition;
    fire->setScale(size.width/fire->getContentSize().width);
    _maplogic->getPosByCoor(fire, x, y, realPosition);
    fire->setPosition(realPosition);
    CCLOG("realPosition x %f realPosition y %f", realPosition.x, realPosition.y);
    AddChild(fire);
}

void HelloWorld::ratotePlane()
{
    if( _planeDer < 0 )
    {
        _planeDer+=4;
    }
    _planeDer = _planeDer%4;
    CCLOG("ratotePlane _planeDer %d", _planeDer);
    
    bool bTouched = false;
    if((CCPointZero.x != _planePos.x) || (CCPointZero.y!=_planePos.y))
    {
        bTouched = true;
    }
    
    _plane_t->setVisible(false);
    _plane_d->setVisible(false);
    _plane_r->setVisible(false);
    _plane_l->setVisible(false);
    
    CCPoint realPosition;
    std::string plane;
    switch (_planeDer) {
        case DER_TOP:
            if( bTouched )
            {
                _maplogic->getMapPosition(_plane_t, _planePos, _planePos);
                _plane_t->setPosition(_planePos);
            }
            _plane_t->setVisible(true);
            break;
        
        case DER_DOWN:
            if( bTouched )
            {
                _maplogic->getMapPosition(_plane_d, _planePos, _planePos);
                _plane_d->setPosition(_planePos);
            }
            _plane_d->setVisible(true);
            break;
        
        case DER_RIGHT:
            if( bTouched )
            {
                _maplogic->getMapPosition(_plane_r, _planePos, _planePos);
                _plane_r->setPosition(_planePos);
            }_plane_r->setVisible(true);
            break;
        
        case DER_LEFT:
            if( bTouched )
            {
                _maplogic->getMapPosition(_plane_l, _planePos, _planePos);
                _plane_l->setPosition(_planePos);
            }
            _plane_l->setVisible(true);
            break;
            
        default:
            _plane_t->setPosition(_planePos);
            _plane_t->setVisible(true);
            break;
    }
    
    if( bTouched )
    {
        _maplogic->updateMap(_planeDer);
    }
    _maplogic->displayMap();
}

//-------------------------------------------------------------------------------
bool HelloWorld::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
    CCLOG("启动输入");
    return false;
    //return true;(不启动)
}


bool HelloWorld::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    CCLOG("关闭输入");
    if( sender == _nameText )
    {
        g_svr->doSvrCmd("reg", _nameText->getString());
    }
    
    if( sender == _nameEnemy )
    {
        if((0 != strlen(_nameEnemy->getString()))
           && (0 == strcmp(_nameEnemy->getString(), "对手")) )
        {
            g_svr->doSvrCmd("vs", _nameEnemy->getString());
        }
    }
    
    return false;
    //return true;(不关闭)
}


bool  HelloWorld::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    CCLOG("输入字符:%s", text);
    return false;
    //return true;(不输入)
}


bool HelloWorld::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    CCLOG("删除字符");
    return false;
    //return true;(不删除)
}

