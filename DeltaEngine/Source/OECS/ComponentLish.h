#pragma once

namespace DeltaEngine
{

  class IComponentList
  {
  public:
    virtual ~IComponentList() = default;
    virtual void destroy_entity(Entity entity) = 0;
  };

  template <typename T>
  class ComponentList : public IComponentList
  {
    std::array<T, MAX_ENTITIES> _components{};
    std::unordered_map<Entity, Index> _entity_index_map{};
    std::unordered_map<Index, Entity> _index_entity_map{};
    Size _components_count{};

  public:
    void insert(Entity entity, T component)
    {
      assert(_entity_index_map.find(entity) == _entity_index_map.end() && "Component added to same entity more than once.");

      // Put new entry at end
      Index index = _components_count;
      _entity_index_map[entity] = index;
      _index_entity_map[index] = entity;
      _components[index] = component;
      _components_count++;
    }

    void remove(Entity entity)
    {
      assert(_entity_index_map.find(entity) != _entity_index_map.end() && "Removing non-existent component.");

      // Copy element at end into deleted element's place to maintain density
      Index removed_entity = _entity_index_map[entity];
      Index last_element = _components_count - 1;
      _components[removed_entity] = _components[last_element];

      // Update map to point to moved spot
      Entity entityOfLastElement = _index_entity_map[last_element];
      _entity_index_map[entityOfLastElement] = removed_entity;
      _index_entity_map[removed_entity] = entityOfLastElement;

      _entity_index_map.erase(entity);
      _index_entity_map.erase(last_element);

      _components_count++;
    }

    T &get(Entity entity)
    {
      assert(_entity_index_map.find(entity) != _entity_index_map.end() && "Retrieving non-existent component.");

      return _components[_entity_index_map[entity]];
    }

    void destroy_entity(Entity entity) override
    {
      if (_entity_index_map.find(entity) != _entity_index_map.end())
        remove(entity);
    }
  };

} // namespace DeltaEngine