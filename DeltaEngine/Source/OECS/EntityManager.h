#pragma once

namespace DeltaEngine
{

  class EntityManager
  {

    std::queue<Entity> _entities_bin{};
    std::set<Entity> _entities{};
    std::array<BitSignature, MAX_ENTITIES> _entities_signature{};
    Size _entities_count{};

  public:
    EntityManager()
    {
      for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        _entities_bin.push(entity);
    }

    std::set<Entity> &list()
    {
      return _entities;
    }

    Entity create_entity()
    {
      assert(_entities_count < MAX_ENTITIES && "Too many entities in existence.");

      Entity id = _entities_bin.front();
      _entities_bin.pop();
      _entities.insert(id);
      _entities_count++;
      return id;
    }

    void destroy_entity(Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      _entities_signature[entity].reset();
      _entities_bin.push(entity);
      _entities_count--;
      _entities.erase(entity);
    }

    void set_signature(Entity entity, BitSignature signature)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      _entities_signature[entity] = signature;
    }

    BitSignature get_signature(Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      return _entities_signature[entity];
    }
  };

} // namespace DeltaEngine