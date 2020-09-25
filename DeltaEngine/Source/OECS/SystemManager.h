#pragma once

#include "System.h"

namespace DeltaEngine
{

  class SystemManager
  {

    std::unordered_map<std::type_index, BitSignature> _systems_signature{};
    std::unordered_map<std::type_index, std::unique_ptr<System>> _systems{};

  public:
    template <typename T>
    T *register_system()
    {
      std::type_index index = std::type_index(typeid(T));

      assert(_systems.find(index) == _systems.end() && "System already registered.");

      _systems.insert({index, std::make_unique<T>()});
      return static_cast<T *>(_systems[index].get());
    }

    template <typename T>
    void set_signature(BitSignature signature)
    {
      std::type_index index = std::type_index(typeid(T));

      assert(_systems.find(index) != _systems.end() && "System not registered.");

      _systems_signature.insert({index, signature});
    }

    void destroy_entity(Entity entity)
    {
      for (const auto &pair : _systems)
      {
        const auto &system = pair.second;
        system->entities.erase(entity); // Remove entity from system
      }
    }

    void update_entity(Entity entity, BitSignature entitySignature)
    {
      for (const auto &pair : _systems)
      {
        const auto &type = pair.first;
        const auto &system = pair.second;
        const auto &systemSignature = _systems_signature[type];

        if ((entitySignature & systemSignature) == systemSignature)
          system->entities.insert(entity); // Insert entity into system when bit match
        else
          system->entities.erase(entity); // Remove entity from system when bit does not match
      }
    }
  };

} // namespace DeltaEngine