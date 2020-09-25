#pragma once

#include "Types.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace DeltaEngine
{

  class ECSModule
  {
    std::unique_ptr<EntityManager> _entities_manager;
    std::unique_ptr<ComponentManager> _components_manager;
    std::unique_ptr<SystemManager> _systems_manager;

  public:
    explicit ECSModule() : _entities_manager(std::make_unique<EntityManager>()),
                           _components_manager(std::make_unique<ComponentManager>()),
                           _systems_manager(std::make_unique<SystemManager>())
    {
    }

    // ENTITIES METHOD
    Entity create_entity()
    {
      return _entities_manager->create_entity();
    }

    void destroy_entity(Entity entity)
    {
      _entities_manager->destroy_entity(entity);
      _components_manager->destroy_entity(entity);
      _systems_manager->destroy_entity(entity);
    }

    void destroy_entities()
    {
      auto it = _entities_manager->list().begin();
      while (it != _entities_manager->list().end())
        destroy_entity(*it++);
    }

    // COMPONENTS METHOD
    template <typename T>
    void register_component()
    {
      _components_manager->register_component<T>();
    }

    template <typename T>
    void add_component(Entity entity, T component)
    {
      _components_manager->add_component<T>(entity, component);

      auto bitSignature = _entities_manager->get_signature(entity);
      bitSignature.set(_components_manager->get_component_types<T>(), true);
      _entities_manager->set_signature(entity, bitSignature);

      _systems_manager->update_entity(entity, bitSignature);
    }

    template <typename T>
    void remove_component(Entity entity)
    {
      _components_manager->remove_component<T>(entity);

      auto bitSignature = _entities_manager->get_signature(entity);
      bitSignature.set(_components_manager->get_component_types<T>(), false);
      _entities_manager->set_signature(entity, bitSignature);

      _systems_manager->update_entity(entity, bitSignature);
    }

    template <typename T>
    T &get_component(Entity entity)
    {
      return _components_manager->get_component<T>(entity);
    }

    template <typename T>
    ComponentType get_component_type()
    {
      return _components_manager->get_component_types<T>();
    }

    // SYSTEM METHOD
    template <typename T>
    T *register_system()
    {
      return _systems_manager->register_system<T>();
    }

    template <typename T>
    void set_system_signature(BitSignature signature)
    {
      _systems_manager->set_signature<T>(signature);
    }
  };

} // namespace DeltaEngine