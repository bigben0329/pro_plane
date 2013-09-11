//
//  MapLogic.h
//  pro_plane
//
//  Created by momo@ben on 13-9-7.
//
//

#ifndef __pro_plane__MapLogic__
#define __pro_plane__MapLogic__

#include <iostream>
#include "ccTypes.h"
#include "CCSprite.h"

#define MAP_MAX_ROW 99 
#define MAP_MAX_COLUM 99

enum MAP_STATUS{
    MAP_STATUS_NEW = 6,
    MAP_STATUS_HEAD = 1,
    MAP_STATUS_BODY = 2,
    MAP_STATUS_HIT = 3,
    MAP_STATUS_HURT = 4,
    MAP_STATUS_DEAD = 5
};

enum FIGHT_STATUS{
    FIGHT_STATUS_PREPARE = 0,
    FIGHT_STATUS_READY = 1,
    FIGHT_STATUS_MISS = 2,
    FIGHT_STATUS_WIN = 3,
    FIGHT_STATUS_HURT = 4,
    FIGHT_STATUS_LOST = 5
};

class CMapLogic {
public:
    ~CMapLogic();
    CMapLogic(const cocos2d::CCRect& pos, int row = 9 , int column = 9);
    
    int getMapPosition(cocos2d::CCSprite* sprite,
                       cocos2d::CCPoint& newPosition,
                       cocos2d::CCPoint& realPosition);
    
    int setScale(float scalex, float scaley);
    int getCoordinate(int& x, int& y);
    int getItemSize(float& width, float& heigth);
    
    int updateMap();
    void displayMap();
    
    FIGHT_STATUS hitMap(int x, int y);
    
private:
    
    MAP_STATUS _MapStatus[MAP_MAX_ROW][MAP_MAX_COLUM]; //地图状态二维数组
    MAP_STATUS _HitStatus[MAP_MAX_ROW][MAP_MAX_COLUM]; //进攻状态二维数组
    
    cocos2d::CCRect _rect; //地图区域
    
    int _row;     //地图的行数
    int _colum;   //地图的列数
    
    float _scalex; //地图横向缩放比例
    float _scaley; //地图纵向缩放比例

    float _itemWidth; //地图区域item宽度
    float _itemHeight; //地图区域item高度
    
    int _CoordX;  //地图item横坐标
    int _CoordY;  //地图item纵坐标

};


#endif /* defined(__pro_plane__MapLogic__) */
