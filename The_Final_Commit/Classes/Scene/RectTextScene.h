#ifndef RectTextScene_h
#define RectTextScene_h

#include "cocos2d.h"

USING_NS_CC;

class MouseRect : public cocos2d::DrawNode
{
public:
    CREATE_FUNC(MouseRect);
    
    cocos2d::Point TouchStartPoint;
    cocos2d::Point TouchEndPoint;
};

class RectTextScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    MouseRect* MouseRect_ = nullptr;//选框
    
    CREATE_FUNC(RectTextScene);
    
    void getChildrenInRect(Point P1, Point P2);
};

#endif /* RectTextScene_h */
