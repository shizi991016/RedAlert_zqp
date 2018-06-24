#ifndef LoseScene_h
#define LoseScene_h

#include "cocos2d.h"

USING_NS_CC;

class LoseScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuExitCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(LoseScene);
};


#endif /* LoseScene_h */
