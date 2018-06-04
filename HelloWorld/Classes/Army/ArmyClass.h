#ifndef ArmyClass_h
#define ArmyClass_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;


class BuildingsClass : public cocos2d::Sprite
{
public:
    void setLifeValue(int LifeValue)
    {
        _LifeValue = LifeValue;
    }
    
    int getLifeValue()
    {
        return _LifeValue;
    }
    
    void setHP(LoadingBar* HP)
    {
        _HP = HP;
    }
    
    LoadingBar* getHP()
    {
        return _HP;
    }
    
    void setHPInterval(float HPInterval)
    {
        _HPInterval = HPInterval;
    }
    
    float getHPInterval()
    {
        return _HPInterval;
    }
    
    bool isBuildingDestoried()
    {
        return (_LifeValue <= 0);
    }
    
    static BuildingsClass* createWithSpriteFileName(const std::string& FileName)
    {
        BuildingsClass* Building = new BuildingsClass();
        if (Building && Building->initWithFile(FileName))
        {
            Building->autorelease();
            return Building;
        }
        CC_SAFE_DELETE(Building);
        return nullptr;
    }
private:
    int _LifeValue;
    LoadingBar* _HP;
    float _HPInterval;
};



#endif /* ArmyClass_h */
