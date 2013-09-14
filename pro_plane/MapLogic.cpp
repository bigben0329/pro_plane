//
//  MapLogic.cpp
//  pro_plane
//
//  Created by momo@ben on 13-9-7.
//
//

#include "MapLogic.h"

CMapLogic::CMapLogic(const cocos2d::CCRect& rect, int row, int column):
    _rect(rect),
    _row(row),
    _colum(column)
{
    _scalex = 1.0;
    _scaley = 1.0;
    _CoordX = 0;
    _CoordY = 0;
    _itemWidth = 0;
    _itemHeight = 0;
    
    for(int x = 0 ; x < _row ; x++ )
    {
        for( int y = 0 ; y < _colum ; y++ )
        {
            _MapStatus[x][y] = MAP_STATUS_NEW;
        }
    }
    
    for(int x = MAP_MAX_ROW ; x < _row ; x++ )
    {
        for( int y = 0 ; y < MAP_MAX_COLUM ; y++ )
        {
            _HitStatus[x][y] = MAP_STATUS_NEW;
        }
    }
    
}

CMapLogic::~CMapLogic()
{

}


int CMapLogic::setScale(float scalex, float scaley)
{
    _scalex = scalex;
    _scaley = scaley;
    return 0;
}


int CMapLogic::getCoordinate(int& x, int& y)
{
    x = _CoordX;
    y = _CoordY;
    return 0;
}


int CMapLogic::getItemSize(float& width, float& heigth)
{
    width = _rect.size.width/_row;
    heigth = _rect.size.height/_colum;
    return 0;
}


int CMapLogic::updateMap(int der)
{
    for(int x = 0 ; x < _row ; x++ )
    {
        for( int y = 0 ; y < _colum ; y++ )
        {
            _MapStatus[x][y] = MAP_STATUS_NEW;
        }
    }
    
    CCLOG("update _CoordX:%d _CoordY:%d", _CoordX, _CoordY);
//#define DER_TOP   0
//#define DER_DOWN  2
//#define DER_RIGHT 1
//#define DER_LEFT  3
    switch (der) {
        case 0:
        {
            //head
            _MapStatus[_CoordX+2][_CoordY+3] = MAP_STATUS_HEAD;
            
            //body
            _MapStatus[_CoordX][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+3][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+4][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+3][_CoordY] = MAP_STATUS_BODY;
            
            break;
        }
            
        case 1:
        {
            //head
            _MapStatus[_CoordX+3][_CoordY+2] = MAP_STATUS_HEAD;
            
            //body
            _MapStatus[_CoordX][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX][_CoordY+3] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+3] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+4] = MAP_STATUS_BODY;
            
            break;
        }
    
        case 2:
        {
            //head
            _MapStatus[_CoordX+2][_CoordY] = MAP_STATUS_HEAD;
            
            //body
            _MapStatus[_CoordX][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+3][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+4][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+3] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+3] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+3][_CoordY+3] = MAP_STATUS_BODY;
            
            break;
        }
            
            
        case 3:
        {
            //head
            _MapStatus[_CoordX][_CoordY+2] = MAP_STATUS_HEAD;
            
            //body
            _MapStatus[_CoordX+3][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+3][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+3][_CoordY+3] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+2][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+1] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+2] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+3] = MAP_STATUS_BODY;
            _MapStatus[_CoordX+1][_CoordY+4] = MAP_STATUS_BODY;
            
            break;
        }
    
        default:
            break;
    }

    
    return 0;
}


int CMapLogic::getMapPosition(cocos2d::CCSprite* sprite,
                              cocos2d::CCPoint& newPosition,
                              cocos2d::CCPoint& realPosition)
{
    realPosition = newPosition;
    
    CCLOG("rect width:%f heigth:%f", _rect.size.width ,_rect.size.height);
    CCLOG("sprite width:%f heigth:%f",sprite->getContentSize().width,sprite->getContentSize().height);
    if(_rect.size.width < sprite->getContentSize().width
       || _rect.size.height < sprite->getContentSize().height )
    {
        CCLOG("rect is too small!");
        return 0;
    }
    
    CCLOG("newPosition x:%f y:%f scalex:%f scaley:%f", newPosition.x, newPosition.y, _scalex, _scaley);
    cocos2d::CCPoint tempPos;
    tempPos.x = newPosition.x/_scalex;
    tempPos.y = newPosition.y/_scaley;
    CCLOG("tempPos x:%f y:%f", tempPos.x, tempPos.y);
    
    //get real pos x
    if( _rect.getMinX() >= tempPos.x - sprite->getContentSize().width/2 )
    {
        _CoordX = 0;
        //CCLOG("rect minx:%f sprite min x:%f", _rect.getMinX(), tempPos.x - sprite->getContentSize().width/2);
        realPosition.x = _rect.getMinX() + sprite->getContentSize().width/2;
    }
    else if( _rect.getMaxX() <= tempPos.x + sprite->getContentSize().width/2)
    {
        _CoordX = (int)((_rect.getMaxX() - sprite->getContentSize().width)/_itemWidth)-1;
        //CCLOG("rect max:%f sprite max x:%f", _rect.getMaxX(), tempPos.x + sprite->getContentSize().width/2);
        realPosition.x = _rect.getMaxX() - sprite->getContentSize().width/2;
    }
    else
    {
        float sprMinX = tempPos.x - sprite->getContentSize().width/2;
        _itemWidth = _rect.size.width/_row;
        CCLOG("rect min:%f max:%f sprite min x:%f itemWidth:%f",
              _rect.getMinX(), _rect.getMaxX(),
              sprMinX, _itemWidth);
        
        _CoordX = (int)((sprMinX - _rect.getMinX())/_itemWidth);
        CCLOG("_CoordX:%d", _CoordX);
        realPosition.x = _rect.getMinX() + _CoordX*_itemWidth + sprite->getContentSize().width/2;
        CCLOG("real x:%f", realPosition.x);
    }
    
    //get real pos y
    if( _rect.getMinY() >= tempPos.y - sprite->getContentSize().height/2 )
    {
        _CoordY = 0;
        CCLOG("rect miny:%f sprite min y:%f", _rect.getMinY(), tempPos.y - sprite->getContentSize().height/2);
        realPosition.y = _rect.getMinY() + sprite->getContentSize().height/2;
    }
    else if( _rect.getMaxY() <= tempPos.y + sprite->getContentSize().height/2)
    {
        _CoordY = (int)((_rect.getMaxY() - sprite->getContentSize().height)/_itemHeight);
        CCLOG("rect maxy:%f sprite max y:%f", _rect.getMaxY(), tempPos.y + sprite->getContentSize().height/2);
        realPosition.y = _rect.getMaxY() - sprite->getContentSize().height/2;
    }
    else
    {
        float sprMinY = tempPos.y - sprite->getContentSize().height/2;
        _itemHeight = _rect.size.height/_colum;
        CCLOG("rect min:%f max:%f sprite min y:%f sprMinY:%f",
              _rect.getMinY(), _rect.getMaxY(),
              sprMinY, _itemHeight);
        
        _CoordY = (int)((sprMinY - _rect.getMinY())/_itemHeight);
        CCLOG("_CoordY:%d", _CoordY);
        realPosition.y = _rect.getMinY() + _CoordY*_itemHeight + sprite->getContentSize().height/2;
        CCLOG("real y:%f", realPosition.y);
    }

    //get real pos
    realPosition.x *= _scalex;
    realPosition.y *= _scaley;
    
    return 0;
}


int CMapLogic::getPosByCoor(cocos2d::CCSprite* sprite,
                              int x, int y,
                              cocos2d::CCPoint& realPosition)
{
    realPosition.x = _rect.getMinX() + x*_itemWidth + sprite->getContentSize().width/2;
    realPosition.y = _rect.getMinY() + y*_itemHeight + sprite->getContentSize().height/2;
    
    //get real pos
    realPosition.x *= _scalex;
    realPosition.y *= _scaley;
    
    return 0;
}


void CMapLogic::displayMap()
{
    char szMapDisplay[1024] = {0};
    int index = 0;
    CCLOG("_row:%d _colum:%d", _row, _colum);
    
    snprintf(szMapDisplay + index, sizeof(int), "\n");
    index++;
    for(int y = _colum-1 ; y >= 0 ; y-- )
    {
        for( int x = 0 ; x < _row ; x++ )
        {
            CCLOG("x:%d y:%d status:%d", x, y, _MapStatus[x][y]);
            snprintf(szMapDisplay + index, sizeof(int), "%d", _MapStatus[x][y]);
            index++;
        }
        snprintf(szMapDisplay + index, sizeof(int), "\n");
        index++;
    }
    CCLOG(szMapDisplay);
}


FIGHT_STATUS CMapLogic::hitMap(int x, int y)
{
    if( MAP_STATUS_HEAD == _MapStatus[x][y] )
    {
        _HitStatus[x][y] = MAP_STATUS_DEAD;
        return FIGHT_STATUS_WIN;
    }
    else if( MAP_STATUS_BODY == _MapStatus[x][y] )
    {
        _HitStatus[x][y] = MAP_STATUS_HURT;
        return FIGHT_STATUS_HURT;
    }
    
    _HitStatus[x][y] = MAP_STATUS_HIT;
    return FIGHT_STATUS_MISS;
}


