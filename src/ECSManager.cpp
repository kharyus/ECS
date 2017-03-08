#include "ECSManager.h"

namespace ecs
{

ECSManager::ECSManager()
{
    //ctor
}

ECSManager::~ECSManager()
{
    for (auto& system : systems)
        delete system;
    for (auto& entity : entities)
        delete entity;
}

void ECSManager::updateSystems()
{
    for (auto& system : systems)
        system->update();
}

Entity& ECSManager::newEntity()
{
    Entity* newEntity = new Entity();
    newEntity->setECSManagerReference(this);
    entities.push_back(newEntity);

    return *newEntity;
}

void ECSManager::deleteEntity(Entity& entity)
{
    auto it = entities.begin();
    while (it != entities.end())
    {
        if (*it == &entity)
        {
            delete *it;
            entities.erase(it);
            return;
        }

        ++it;
    }
}

void ECSManager::componentAdded(std::type_index componentType, Entity* entity)
{
    // Broadcast this message to every system.
    for (auto& system : systems)
        // Not to be confused with the function on this class with same name.
        system->componentAdded(componentType, entity);
}

void ECSManager::componentRemoved(std::type_index componentType, Entity* entity)
{
    // Broadcast this message to every system.
    for (auto& system : systems)
        // Not to be confused with the function on this class with same name.
        system->componentRemoved(componentType, entity);
}

void ECSManager::deleteSystem(System& system)
{
    auto it = systems.begin();
    while (it != systems.end())
    {
        if (*it == &system)
        {
            delete *it;
            systems.erase(it);
            return;
        }

        ++it;
    }
}

unsigned ECSManager::getSystemsCount()
{
    return systems.size();
}

} // namespace ecs
