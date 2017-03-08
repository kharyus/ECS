#include "System.h"

#include <algorithm>

namespace ecs
{

System::System()
{
    //ctor
}

System::~System()
{
    //dtor
}

void System::filterEntities(std::vector<Entity*>& allEntities)
{
    // If there's no dependencies, there's nothing that is relevant to this system.
    if (componentDependencies.size() <= 0)
        return;

    // Clear relevant entities in case it's populated.
    relevantEntities.clear();

    // Add relevant entities to the vector
    for(auto it = allEntities.begin(); it != allEntities.end(); it++)
    {
        for (std::type_index dependency : componentDependencies)
        {
            if (!(*it)->hasComponent(dependency))
                goto NextIt; // Look, Gotos! I'm a terrible programmer. :-)
        }

        // If reached here, it has all deps.
        relevantEntities.push_back(*it);

        // This goto is to skip the addition of tuples and entities
        NextIt:;
    }
}

std::vector<Entity*>& System::getEntities()
{
    return relevantEntities;
}

void System::componentAdded(std::type_index componentType, Entity* entity)
{
    // If there's no dependencies, there's nothing that is relevant to this system.
    if (componentDependencies.size() <= 0)
        return;

    // If the entity is already inside the vector, no need to add it.
    if (std::find(relevantEntities.begin(), relevantEntities.end(), entity) != relevantEntities.end())
        return;

    // See if the entity that the component was added into is now relevant to this system. If it is add it.
    for (std::type_index dependency : componentDependencies)
    {
        // Break early if it doesn't have any of them.
        if (!entity->hasComponent(dependency))
            return;
    }

    // If it reached here it has all deps. Add it.
    relevantEntities.push_back(entity);
}

void System::componentRemoved(std::type_index componentType, Entity* entity)
{
    // If there's no dependencies, there's nothing that is relevant to this system.
    if (componentDependencies.size() <= 0)
        return;

    // If the entity is not inside the vector, no need to remove it.
    auto it = std::find(relevantEntities.begin(), relevantEntities.end(), entity);
    if (it == relevantEntities.end())
        return;

    // See if the component that was removed is a dependency of this system. If it is remove the entity.
    for (std::type_index dependency : componentDependencies)
    {
        if (componentType == dependency)
        {
            relevantEntities.erase(it);
            return;
        }
    }
}

} // namespace ecs
