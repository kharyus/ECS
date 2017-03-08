#ifndef ECSMANAGER_H
#define ECSMANAGER_H

#include "Entity.h"
#include "System.h"
#include "Component.h"
#include "Optional.h"
#include <vector>

namespace ecs
{

/** The ECSManager class coordinates the relationship between Systems and Entities,
  * including their creation. */
class ECSManager
{
public:
    ECSManager();
    virtual ~ECSManager();

    /** Updates all systems */
    void updateSystems();

    /** Creates a new BaseEntity. The user should get the reference returned to add components to it. */
    Entity& newEntity();

    /** Creates a new Entity with Components to be added already specified. */
    template <typename... Comps>
    Entity& newEntity();

    /** Deletes an entity. */
    void deleteEntity(Entity& entity);

    /** Creates a new system. */
    //template <template<typename, typename...> typename T, typename... Deps>
    template <typename T>
    System& newSystem();

    template <typename T, typename... Params>
    System& newSystemWithParameters(Params... params);

    /** Retrieves the system. */
    template <typename T>
    optionalPtr<T> getSystem();

    /** Deletes an entity. */
    void deleteSystem(System& entity);

    /** Retrieves the number of systems populated. */
    unsigned getSystemsCount();

protected:
    // Friends for message passing.
    friend Entity;

    ////////////
    // Events //
    ////////////

    /** Entity calls this to notify that a component has been added.
      * This function then calls the one from each system. */
    void componentAdded(std::type_index componentType, Entity* entity);

    /** Entity calls this to notify that a component has been removed.
      * This function then call the one from each system. */
    void componentRemoved(std::type_index componentType, Entity* entity);

private:
    // List of Entities
    std::vector<Entity*> entities;
    // List of Systems
    std::vector<System*> systems;
};





/////////////////////////////
// Template Implementation //
/////////////////////////////

template <typename... Comps>
Entity& ECSManager::newEntity()
{
    Entity* newEntity = new Entity();
    newEntity->setECSManagerReference(this);
    newEntity->addComponent<Comps...>();
    entities.push_back(newEntity);

    return *newEntity;
}

// TODO: Verify if already has system, then negate the addition.
//template <template<typename, typename...> typename T, typename... Deps>
template <typename T>
System& ECSManager::newSystem()
{
    //T<Deps...>* newSys = new T<Deps...>();
    T* newSys = new T();
    newSys->filterEntities(entities);
    systems.push_back(newSys);

    return *newSys;
}

template <typename T, typename... Params>
System& ECSManager::newSystemWithParameters(Params... params)
{
    //T<Deps...>* newSys = new T<Deps...>();
    T* newSys = new T(params...);
    newSys->filterEntities(entities);
    systems.push_back(newSys);

    return *newSys;
}

template <typename T>
optionalPtr<T> ECSManager::getSystem()
{
    for (auto& system : systems)
        if (std::type_index(typeid(*system)) == std::type_index(typeid(T)))
            return dynamic_cast<T*>(system);

    return nullptr;
}

} // namespace ecs

#endif // ECSMANAGER_H
