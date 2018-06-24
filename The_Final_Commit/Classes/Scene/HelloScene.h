
#ifndef HelloSence_h
#define HelloSence_h

#include "cocos2d.h"

USING_NS_CC;

class HelloScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuExitCallback(cocos2d::Ref* pSender);
    
    void menuStartCallback(cocos2d::Ref* pSender);
    
    void menuSettingCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HelloScene);
};

#endif // __HELLOWORLD_SCENE_H__
