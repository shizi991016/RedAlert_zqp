
#ifndef HelloSence_h
#define HelloSence_h

#include "cocos2d.h"

USING_NS_CC;

class HelloScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuExitCallback(cocos2d::Ref* pSender);
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuSettingCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloScene);
    
};

#endif // __HELLOWORLD_SCENE_H__
