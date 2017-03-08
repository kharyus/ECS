#ifndef COMPONENT_H
#define COMPONENT_H

namespace ecs
{

class Entity;

/** Component class only serves the purpose of code auto-documentation. */
struct Component
{
public:
    Component();
    virtual ~Component();
};

} // namespace ecs

#endif // COMPONENT_H
