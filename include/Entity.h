#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <unordered_map>
#include <typeindex>
#include <iostream>
//#include <memory>
#include "Optional.h"

namespace ecs
{

class ECSManager;

/** BaseEntity -> Entity uses the same pattern from BaseSystem -> System.
  * BaseEntity is a generalization to be easily stored in vectors, meanwhile Entity does some extra things.*/
class Entity
{
public:
    /** Instantiates a component and adds it to the list of components. */
    template <typename T>
    void addComponent();

    /** Instantiates some components and adds them to the list of components. */
    template <typename T, typename T2, typename... Ts>
    void addComponent();

    /** Instantiates a component that needs to be initialized with some parameters. */
    template <typename T, typename... Params>
    Entity& addComponentWithParameters(Params... params);

    /** Retrieves a component if it's included in this entity.. */
    template <typename T>
    optionalPtr<T> getComponent();

    /** Retrieves a component's type if it's included in this entity. */
    optionalPtr<Component> getComponentByType(std::type_index& type);

    /** Returns whether or not this entity has the specified component based on the templated parameter. */
    template <typename T>
    bool hasComponent();

    /** Returns whether or not this entity has the specified component based on its type_index. */
    bool hasComponent(std::type_index& type);

protected:
    friend class ECSManager;

    /** Protected constructor to be instantiated by ECSManager.
      * The user should not make instantiate this class. */
    Entity();
    virtual ~Entity();

    void setECSManagerReference(ECSManager* _ecsManager);

private:
    /** Map of components. */
    //std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
    std::unordered_map<std::type_index, Component*> components;

    /** Reference to ECSManager. */
    optionalPtr<ECSManager> ecsManager = nullptr;

    ////////////////////
    // Event Triggers //
    ////////////////////

    /** Triggers the componentAddedEvent on ECSManager */
    void componentAdded(std::type_index componentType);

    /** Triggers the componentRemovedEvent on ECSManager */
    void componentRemoved(std::type_index componentType);
};





/////////////////////////////
// Template Implementation //
/////////////////////////////

// TODO: Verify if already has component
template <typename T>
void Entity::addComponent()
{
    /*std::unique_ptr<T> newComp(new T());
    components[std::type_index(typeid(T))] = std::move(newComp);*/
    T* newComp = new T();
    components[std::type_index(typeid(T))] = std::move(newComp);

    // Notify ECSManager so it can notify the systems.
    componentAdded(typeid(T));
}

template <typename T, typename T2, typename... Ts>
void Entity::addComponent()
{
    T* newComp = new T();
    components[std::type_index(typeid(T))] = std::move(newComp);

    // Notify ECSManager so it can notify the systems.
    componentAdded(typeid(T));

    // Call next addComponent
    addComponent<T2, Ts...>();
}

// TODO: Verify if already has component
template <typename T, typename... Params>
Entity& Entity::addComponentWithParameters(Params... params)
{
    T* newComp = new T(params...);
    //newComp->setOwnerEntity(this);
    components[std::type_index(typeid(T))] = newComp;

    // Notify ECSManager so it can notify the systems.
    componentAdded(typeid(T));

    return *this;
}

template <typename T>
optionalPtr<T> Entity::getComponent()
{
    auto it = components.find(std::type_index(typeid(T)));

    // Each component has different members, I don't see a way to avoid dynamic_cast unfortunately.
    // TODO: In the end, i will just limit it to run in specific frames instead of on the update.
    if (it != components.end())
        return static_cast<T*>(it->second);

    return nullptr;
}

template <typename T>
bool Entity::hasComponent()
{
    auto it = components.find(std::type_index(typeid(T)));

    if (it != components.end())
        return true;

    return false;
}

} // namespace ecs

#endif // ENTITY_H
