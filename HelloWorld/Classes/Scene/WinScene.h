#ifndef WinScene_h
#define WinScene_h

#include "cocos2d.h"

USING_NS_CC;

class WinScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuExitCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(WinScene);
};



#endif /* WinScene_h */
