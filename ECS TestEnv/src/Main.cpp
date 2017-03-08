#include <iostream>
#include "ECSManager.h"

// Test components and systems

struct HealthComponent : public Component
{
    int health = 10;
    int defense = 3;
    bool isDead = false;
};

class HealthSystem : public System
{
public:
    HealthSystem()
    {
        newComponentDependency<HealthComponent>();
    }

    void update()
    {
        dealDamage();
    }

    void dealDamage()
    {
        int damageDealt = 10;
        auto hpComp = getEntities()[0]->getComponent<HealthComponent>();

        if (!hpComp->isDead)
            hpComp->health -= damageDealt - hpComp->defense;
        else
            std::cout << "Stop, he's already dead." << std::endl;

        hpComp->isDead = hpComp->health <= 0 ? true : false;
    }
};

// Read Print Loop System
class RPLSystem : public System
{
public:
    void update()
    {
        std::string s;
        std::cin >> s;
        std::cout << s << std::endl;
        std::cout << getEntities()[0]->getComponent<HealthComponent>()->health << std::endl;
    }
};

int main()
{
    ECSManager ecsManager;

    Entity& e = ecsManager.newEntity();
    e.addComponent<HealthComponent>();
    //e.addComponent<HealthComponent>();

    ecsManager.newSystem<RPLSystem>();
    ecsManager.newSystem<HealthSystem>();

    // Keep running forever
    /*while(1)
    {
        ecsManager.update();
    }*/

    std::cout << typeid(HealthComponent).hash_code() << std::endl;
    std::cout << typeid(HealthSystem).hash_code() << std::endl;
    std::cout << typeid(HealthComponent).hash_code() << std::endl;

    return 0;
}
