#pragma once

#include "ComponentLish.h"

namespace DeltaEngine
{

  class ComponentManager
  {
    std::unordered_map<std::type_index, ComponentType> _types{};
    std::unordered_map<std::type_index, std::unique_ptr<IComponentList>> _components{};
    ComponentType _type_top{};

    template <typename T>
    ComponentList<T> *list()
    {
      std::type_index index = std::type_index(typeid(T));

      assert(_types.find(index) != _types.end() && "Component not registered before use.");
      return static_cast<ComponentList<T> *>(_components[index].get());
    }

  public:
    template <typename T>
    void register_component()
    {
      std::type_index index = std::type_index(typeid(T));

      assert(_types.find(index) == _types.end() && "Registering component type more than once.");

      _types.insert({index, _type_top});
      _components.insert({index, std::make_unique<ComponentList<T>>()});

      ++_type_top;
    }

    template <typename T>
    ComponentType get_component_types()
    {
      std::type_index index = std::type_index(typeid(T));

      assert(_types.find(index) != _types.end() && "Component not registered before use.");

      return _types[index];
    }

    template <typename T>
    void add_component(Entity entity, T component)
    {
      list<T>()->insert(entity, component);
    }

    template <typename T>
    void remove_component(Entity entity)
    {
      list<T>()->remove(entity);
    }

    template <typename T>
    T &get_component(Entity entity)
    {
      return list<T>()->get(entity);
    }

    void destroy_entity(Entity entity)
    {
      for (auto const &pair : _components)
      {
        auto const &component = pair.second;

        component->destroy_entity(entity);
      }
    }
  };

} // namespace DeltaEngine