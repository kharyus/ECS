#include <iostream>
#include "ECSManager.h"
#include <chrono>
#include "Logger.h"

// Test components and systems

using namespace ecs;

// Globals for chrono test
std::chrono::time_point<std::chrono::system_clock> time0, time1;
/*auto time0 = std::chrono::system_clock::now();
auto time1 = std::chrono::system_clock::now();*/

struct BananaComponent : public Component
{
    int banana = 123;
};

struct HealthComponent : public Component
{
    int health = 10;
    int defense = 3;
    bool isDead = false;
};

struct PositionComponent : public Component
{
    using PosUnit = int;
    PosUnit x = 0;
    PosUnit y = 0;
};

// Read Print Loop System
class RPLSystem : public System
{
public:
    RPLSystem()
    {
        newComponentDependency<BananaComponent>();
    }

    void update()
    {
        /*std::string s;
        std::cin >> s;
        std::cout << s << std::endl;*/
        time0 = std::chrono::system_clock::now();
        /*for (auto& entity : getEntities())
            std::cout << entity->getComponent<HealthComponent>()->health << std::endl;*/
    }
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
        for (auto& entity : getEntities())
        {
            auto hpComp = entity->getComponent<HealthComponent>();

            if (!hpComp->isDead) {
                // Remove health
                hpComp->health -= damageDealt - hpComp->defense;

                // Update isDead state
                hpComp->isDead = hpComp->health <= 0 ? true : false;
            }
            else
                // Kill this entity
                ;
        }
    }
};

class MovingSystem : public System
{
public:
    MovingSystem()
    {
        newComponentDependency<PositionComponent>();
    }

    void update()
    {
        for (auto& entity : getEntities())
        {
            auto posComp = entity->getComponent<PositionComponent>();

            ++posComp->x;
            ++posComp->y;
            //std::cout << "X: " << ++posComp->x << "Y: " << ++posComp->y << std::endl;
        }
    }
};

int main()
{
    ECSManager ecsManager;

    //////////////////////////////////////////////////////////////////////////
    // Benchmark Entity creation time ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    const unsigned numEntities = 1'000'000;

    auto timeCreate0 = std::chrono::system_clock::now();
    for (int i = 0; i < numEntities; i++)
    {
        ecsManager.newEntity<HealthComponent, BananaComponent>();
    }
    auto timeCreate1 = std::chrono::system_clock::now();
    std::chrono::duration<double> timeCreateDiff = timeCreate1 - timeCreate0;
    std::cout << "Time taken to create: " << timeCreateDiff.count() << std::endl;
    LOG_LINE("Time Taken to create " + std::to_string(numEntities) + " entities: " + std::to_string(timeCreateDiff.count()));

    //////////////////////////////////////////////////////////////////////////

    // System created here, filters every entity present on ecsManager.
    ecsManager.newSystem<RPLSystem>();
    ecsManager.newSystem<HealthSystem>();

    // New entity created after systems. No message sent yet.
    Entity& e2 = ecsManager.newEntity();
    // A component is added, now this entity is more than just a skeleton.
    // componentAdded() message is sent so that the systems try to get this entity if it fits their criteria.
    e2.addComponent<HealthComponent>();
    e2.addComponent<BananaComponent>();

    Entity& e3 = ecsManager.newEntity();
    ecsManager.deleteEntity(e3);

    // Moving System
    Entity& movableEntity = ecsManager.newEntity();
    movableEntity.addComponent<PositionComponent>();

    ecsManager.newSystem<MovingSystem>();

    // Keep running forever
    while(1)
    {
        ecsManager.updateSystems();

        // Benchmark Update time
        // Note: time0 is defined inside RPLSystem
        time1 = std::chrono::system_clock::now();
        std::chrono::duration<double> timeDiff = time1 - time0;
        std::cout << "Time taken: " << timeDiff.count() << std::endl;
        LOG_LINE("Time Taken on an update with " + std::to_string(ecsManager.getSystemsCount()) + " systems containing arbitrary code, and " + std::to_string(numEntities) + " entities: " + std::to_string(timeDiff.count()));
    }

    return 0;
}
