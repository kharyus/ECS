#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream> // Maybe make a logger header instead of this
#include "Entity.h"
#include <vector>
#include <set>
#include <typeindex>

namespace ecs
{

class ECSManager;

/** System */
class System
{
public:
    virtual void update() = 0;

protected:
    std::vector<Entity*> relevantEntities;
    std::set<std::type_index> componentDependencies;

    // Protected Constructors to be instantiated by ECSManager
    System();
    virtual ~System();

    /** Gets only the relevant entities to the system.
      * Note: No need to run it more than once after being created. */
    void filterEntities(std::vector<Entity*>& allEntities);

    /** Returns the entities. */
    std::vector<Entity*>& getEntities();

    // TODO: Add overload that accepts multiple dependencies
    /** Sets another component dependency for this system. */
    template<typename T>
    void newComponentDependency();

    /** Sets multiple component dependencies for this system. */
    template<typename T, typename T2, typename... Ts>
    void newComponentDependency();

    ////////////
    // Events //
    ////////////

    // Friends for message passing.
    friend class ECSManager;

    /** A component has been added in an entity.
      * Check if the entity is now relevant to this system and add it to the list. */
    void componentAdded(std::type_index componentType, Entity* entity);

    /** A component has been removed from an entity.
      * Check if the entity is still relevant to this system and remove it if it's not. */
    void componentRemoved(std::type_index componentType, Entity* entity);

private:

};





/////////////////////////////
// Template Implementation //
/////////////////////////////

template<typename T>
void System::newComponentDependency()
{
    try
    {
        // Check if the type provided is a base of Component otherwise throw an exception.
        if (!std::is_base_of<Component, T>::value)
            throw std::string("newComponentDependency<") + typeid(T).name() + std::string(">(): Type provided not a child of Component.");

        // Add dependency if there was no error
        componentDependencies.emplace(typeid(T));
    }
    catch (std::string s)
    {
        std::cerr << s << std::endl;
    }
}

template<typename T, typename T2, typename... Ts>
void System::newComponentDependency()
{
    try
    {
        // Check if the type provided is a base of Component otherwise throw an exception.
        if (!std::is_base_of<Component, T>::value)
            throw std::string("newComponentDependency<") + typeid(T).name() + std::string(">(): Type provided not a child of Component.");

        // Add dependency if there was no error
        componentDependencies.emplace(typeid(T));

        // Recursively call next one to be added
        newComponentDependency<T2, Ts...>();
    }
    catch (std::string s)
    {
        std::cerr << s << std::endl;
    }
}

} // namespace ecs

#endif // SYSTEM_H
