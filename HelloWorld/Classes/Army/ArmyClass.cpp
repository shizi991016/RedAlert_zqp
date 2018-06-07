#include "ArmyClass.h"

class ElectricPowerPlantClass : public cocos2d::Sprite
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
    
private:
    int _LifeValue;
    LoadingBar* _HP;
    float _HPInterval;
public:
    static ElectricPowerPlantClass* createWithSpriteFileName(const std::string& FileName)
    {
        ElectricPowerPlantClass* sprite = new ElectricPowerPlantClass();
        if (sprite && sprite->initWithFile(FileName))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
};

class RefineryClass : BuildingsClass
{
public:
    bool IsPositionHaveMiningYard(int IsPositionHaveMiningYard[3201][3201]);
    bool IsHaveRefinery();    
};
