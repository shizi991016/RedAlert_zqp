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
    
    static ElectricPowerPlantClass* createWithSpriteFileName(const std::string& FileName)
    {
        ElectricPowerPlantClass* Building = new ElectricPowerPlantClass();
        if (Building && Building->initWithFile(FileName))
        {
            Building->autorelease();
            return Building;
        }
        CC_SAFE_DELETE(Building);
        return nullptr;
    }
private:
    int _LifeValue = 100;
    LoadingBar* _HP;
    float _HPInterval;
};


class RefineryClass : public cocos2d::Sprite
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
    
    static RefineryClass* createWithSpriteFileName(const std::string& FileName)
    {
        RefineryClass* Building = new RefineryClass();
        if (Building && Building->initWithFile(FileName))
        {
            Building->autorelease();
            return Building;
        }
        CC_SAFE_DELETE(Building);
        return nullptr;
    }
private:
    int _LifeValue = 100;
    LoadingBar* _HP;
    float _HPInterval;
};

class WarFactoryClass : public cocos2d::Sprite
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
    
    static WarFactoryClass* createWithSpriteFileName(const std::string& FileName)
    {
        WarFactoryClass* Building = new WarFactoryClass();
        if (Building && Building->initWithFile(FileName))
        {
            Building->autorelease();
            return Building;
        }
        CC_SAFE_DELETE(Building);
        return nullptr;
    }
private:
    int _LifeValue = 100;
    LoadingBar* _HP;
    float _HPInterval;
};


class BarracksClass : public cocos2d::Sprite
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
    
    static BarracksClass* createWithSpriteFileName(const std::string& FileName)
    {
        BarracksClass* Building = new BarracksClass();
        if (Building && Building->initWithFile(FileName))
        {
            Building->autorelease();
            return Building;
        }
        CC_SAFE_DELETE(Building);
        return nullptr;
    }
private:
    int _LifeValue = 100;
    LoadingBar* _HP;
    float _HPInterval;
};


#endif /* ArmyClass_h */
