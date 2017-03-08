#include "Entity.h"

#include "ECSManager.h"

namespace ecs
{

Entity::Entity()
{

}

Entity::~Entity()
{
    for (auto& component : components)
    {
        componentRemoved(component.first);
        delete component.second;
    }
}

void Entity::setECSManagerReference(ECSManager* _ecsManager)
{
    ecsManager = _ecsManager;
}

void Entity::componentAdded(std::type_index componentType)
{
    if (ecsManager != nullptr)
        ecsManager->componentAdded(componentType, this);
}

void Entity::componentRemoved(std::type_index componentType)
{
    if (ecsManager != nullptr)
        ecsManager->componentRemoved(componentType, this);
}

bool Entity::hasComponent(std::type_index& type)
{
    auto it = components.find(type);

    if (it != components.end())
        return true;

    return false;
}

optionalPtr<Component> Entity::getComponentByType(std::type_index& type)
{
    auto it = components.find(type);

    if (it != components.end())
        return it->second;

    return nullptr;
}

} // namespace ecs
