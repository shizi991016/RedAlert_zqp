#ifndef CountryChoiceScene_h
#define CountryChoiceScene_h

#include "cocos2d.h"

USING_NS_CC;

class CountryChoiceScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void countryOneCallBack(cocos2d::Ref* pSender);
    
    void countryTwoCallBack(cocos2d::Ref* pSender);
    
    void countryThreeCallBack(cocos2d::Ref* pSender);
    
    CREATE_FUNC(CountryChoiceScene);
    
};


#endif /* CountryChoiceScene_h */
