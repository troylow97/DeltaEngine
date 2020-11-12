#pragma once

#include "Reflect.h"

#include "EngineConfig.h"

#include "Core/Math/DE_Math.h"
#include "ECS/ComponentMeta.h"
#include "ECS/EntityManager.h"

#include <rttr/registration>

#include "Assets/AssetKey.h"
#include "Core/Utils/Json/JsonSerialize.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{

RTTR_REGISTRATION
{
  rttr::registration::class_<EngineConfig>( "config" )
    .property( "window", &EngineConfig::win_name )
    .property( "width", &EngineConfig::width )
    .property( "height", &EngineConfig::height )
    .property( "fps", &EngineConfig::fps );

  rttr::registration::class_<Vector2>( "vector2" )
    .property( "x", &Vector2::x )
    .property( "y", &Vector2::y );

  rttr::registration::class_<Vector3>( "vector3" )
    .property( "x", &Vector3::x )
    .property( "y", &Vector3::y )
    .property( "z", &Vector3::z );

  rttr::registration::class_<Vector4>( "vector4" )
    .property( "x", &Vector4::x )
    .property( "y", &Vector4::y )
    .property( "z", &Vector4::z )
    .property( "w", &Vector4::w );

  rttr::registration::class_<Quaternion>( "quaternion" )
    .property( "x", &Quaternion::x )
    .property( "y", &Quaternion::y )
    .property( "z", &Quaternion::z )
    .property( "w", &Quaternion::w );

  rttr::registration::class_<Point>( "point" )
    .property( "x", &Point::point_x )
    .property( "y", &Point::point_y );

  rttr::registration::class_<Color>( "color" )
    .property( "r", &Color::r )
    .property( "g", &Color::g )
    .property( "b", &Color::b )
    .property( "a", &Color::a );

  rttr::registration::class_<Material>( "material" )
    .property( "key", &Material::m_ShaderKey );

  rttr::registration::class_<Sprite>( "sprite" )
    .property( "key", &Sprite::m_Key )
    .property( "index", &Sprite::m_Index );

  rttr::registration::enumeration<ColliderType>( "collider_type" )
  (
      rttr::value( "none", ColliderType::NONE ),
      rttr::value( "box", ColliderType::BOX ),
      rttr::value( "circle", ColliderType::CIRCLE ),
      rttr::value( "line", ColliderType::LINE ),
      rttr::value( "ray", ColliderType::RAY )
  );

  rttr::registration::enumeration<Alignment>( "alignment" )
  (
    rttr::value( "align_left", Alignment::AlignLeft ),
    rttr::value( "align_right", Alignment::AlignRight ),
    rttr::value( "center", Alignment::Centralize )
  );

  rttr::registration::class_<Name>( "name" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Name>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "name", &Name::name )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Parent>( "parent" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Parent>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "name", &Parent::p_id )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Transform>( "transform" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Transform>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "old_position", &Transform::old_position )( rttr::policy::prop::bind_as_ptr )
    .property( "position", &Transform::position )( rttr::policy::prop::bind_as_ptr )
    .property( "scale", &Transform::scale )( rttr::policy::prop::bind_as_ptr )
    .property( "rotation", &Transform::rotation )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<RigidBody>( "rigidbody" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<RigidBody>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "direction", &RigidBody::Direction )( rttr::metadata( "NO_SERIALIZE", true ),( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "velocity", &RigidBody::Velocity )( rttr::metadata( "NO_SERIALIZE", true ) )
    .property( "reflected_vector", &RigidBody::ReflectedVector )( rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "acceleration", &RigidBody::Acceleration )( rttr::metadata( "NO_SERIALIZE", true ),( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "accumulated_force", &RigidBody::AccumulatedForce )( rttr::metadata( "NO_SERIALIZE", true ) )
    .property( "point_end", &RigidBody::PointEnd )( rttr::metadata( "NO_SERIALIZE", true ) )
    .property( "mass", &RigidBody::Mass )( rttr::policy::prop::bind_as_ptr )
    .property( "movespeed", &RigidBody::Movespeed )( rttr::policy::prop::bind_as_ptr )
    .property( "restitution", &RigidBody::Restitution )( rttr::policy::prop::bind_as_ptr )
    .property( "friction_coeff", &RigidBody::FrictionCoeff )( rttr::policy::prop::bind_as_ptr )
    .property("inherent_acceleration", &RigidBody::InherentAcceleration)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
    .property("max_acceleration", &RigidBody::MaxAcceleration)(rttr::policy::prop::bind_as_ptr)
    .property("acceleration_pickup", &RigidBody::AccelerationPickup)(rttr::policy::prop::bind_as_ptr)
    .property( "has_gravity", &RigidBody::hasGravity )( rttr::policy::prop::bind_as_ptr )
    .property( "is_moveable", &RigidBody::isMoveable )( rttr::policy::prop::bind_as_ptr )
    .property("is_jumping", &RigidBody::isJumping)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));

  rttr::registration::class_<Collider>("collider")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Collider>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("center", &Collider::center)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("size", &Collider::size)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("inter_point", &Collider::interPoint)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("type", &Collider::type)
      .property("is_collideable", &Collider::isCollideable)(rttr::policy::prop::bind_as_ptr)
      .property("is_trigger", &Collider::isTrigger)(rttr::policy::prop::bind_as_ptr)
      .property("is_colliding_on_floor", &Collider::isCollidingOnFloor)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("collision_layer_id", &Collider::CollisionLayerID)(rttr::policy::prop::bind_as_ptr)
      .property("collision_layer_check", &Collider::CollisionLayerCheck)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<Animator>( "animator" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Animator>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "timer", &Animator::m_Timer )( rttr::policy::prop::bind_as_ptr )
    .property( "speed", &Animator::m_Speed )( rttr::policy::prop::bind_as_ptr )
    .property( "controller_key", &Animator::m_ControllerKey )( rttr::policy::prop::bind_as_ptr )
    .property( "clip_key", &Animator::m_ClipKey )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<State>( "state" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<State>()->bits ) )
    .property( "parameters", &State::parameters )( rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) );

  rttr::registration::class_<Image>( "image" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Image>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "sprite", &Image::m_Sprite )( rttr::policy::prop::bind_as_ptr )
    .property( "offset", &Image::m_Offset )( rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "tiling", &Image::m_Tiling )( rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "flip_x", &Image::m_FlipX )( rttr::policy::prop::bind_as_ptr )
    .property( "flip_y", &Image::m_FlipY )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Text>( "text" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Text>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "key", &Text::m_FontKey )( rttr::policy::prop::bind_as_ptr )
    .property( "text", &Text::m_Text )( rttr::policy::prop::bind_as_ptr )
    .property( "alignment", &Text::alignment )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Renderer2D>( "renderer2D" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Renderer2D>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "material", &Renderer2D::m_Material )( rttr::policy::prop::bind_as_ptr )
    .property( "color", &Renderer2D::m_Color )( rttr::policy::prop::bind_as_ptr )
    .property( "active", &Renderer2D::m_Active )( rttr::policy::prop::bind_as_ptr )
    .property( "shaded", &Renderer2D::m_Shaded )( rttr::policy::prop::bind_as_ptr )
    .property( "wireframe", &Renderer2D::m_Wireframe )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Input>( "input" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Input>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "prev", &Input::previousKey )
    .property( "curr", &Input::currentKey );

  rttr::registration::class_<AI>("ai")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<AI>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("state", &AI::key)(rttr::policy::prop::bind_as_ptr)
      .property("transition", &AI::transition)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<EntityType>("entity_type")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityType>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("type", &EntityType::type)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<Health>("health")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityType>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("current_health", &Health::CurrentHealth)(rttr::policy::prop::bind_as_ptr)
      .property("max_health", &Health::MaxHealth)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<Attack>("attack")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityType>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("damage", &Attack::Damage)(rttr::policy::prop::bind_as_ptr)
      .property("max_cooldown", &Attack::MaxCooldown)(rttr::policy::prop::bind_as_ptr)
      .property("cooldown_timer", &Attack::CooldownTimer)(rttr::policy::prop::bind_as_ptr)
      .property("isAttacking", &Attack::isAttacking)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<Lifespan>("lifespan")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityType>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("lifespan", &Lifespan::Lifespan)(rttr::policy::prop::bind_as_ptr);


}

}

namespace DeltaEngine::RT_Reflect
{

rttr::type RT_Checker( size_t bits )
{
  if ( rttr::type::get_by_name( "name" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "name" );
  if ( rttr::type::get_by_name( "parent" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "parent" );
  if ( rttr::type::get_by_name( "transform" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "transform" );
  if ( rttr::type::get_by_name( "collider" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "collider" );
  if ( rttr::type::get_by_name( "rigidbody" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "rigidbody" );
  if ( rttr::type::get_by_name( "input" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "input" );
  if ( rttr::type::get_by_name( "animator" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "animator" );
  if ( rttr::type::get_by_name( "state" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "state" );
  if ( rttr::type::get_by_name( "image" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "image" );
  if ( rttr::type::get_by_name( "text" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "text" );
  if ( rttr::type::get_by_name( "renderer2D" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "renderer2D" );
  if (rttr::type::get_by_name("ai").get_metadata("bits").to_uint64() == bits)
      return rttr::type::get_by_name("ai");
  if (rttr::type::get_by_name("entity_type").get_metadata("bits").to_uint64() == bits)
      return rttr::type::get_by_name("entity_type");
  if (rttr::type::get_by_name("attack").get_metadata("bits").to_uint64() == bits)
      return rttr::type::get_by_name("attack");
  if (rttr::type::get_by_name("health").get_metadata("bits").to_uint64() == bits)
      return rttr::type::get_by_name("health");
  if (rttr::type::get_by_name("lifespan").get_metadata("bits").to_uint64() == bits)
      return rttr::type::get_by_name("lifespan");
  return rttr::type::get<int>();
}

void RT_Setter( EntityManager &em, EntityID id, size_t bits )
{
  if ( rttr::type::get_by_name( "name" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Name>( id, Name() );
  if ( rttr::type::get_by_name( "parent" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Parent>( id, Parent() );
  if ( rttr::type::get_by_name( "transform" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Transform>( id );
  if ( rttr::type::get_by_name( "collider" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Collider>( id );
  if ( rttr::type::get_by_name( "rigidbody" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<RigidBody>( id );
  if ( rttr::type::get_by_name( "input" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Input>( id );
  if ( rttr::type::get_by_name( "animator" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Animator>( id );
  if ( rttr::type::get_by_name( "state" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<State>( id );
  if ( rttr::type::get_by_name( "image" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Image>( id );
  if ( rttr::type::get_by_name( "text" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Text>( id);
  if ( rttr::type::get_by_name( "renderer2D" ).get_metadata( "bits" ).to_uint64() == bits )
    em.AddComponent<Renderer2D>( id );
  if (rttr::type::get_by_name("ai").get_metadata("bits").to_uint64() == bits)
      em.AddComponent<AI>(id);
  if (rttr::type::get_by_name("entity_type").get_metadata("bits").to_uint64() == bits)
      em.AddComponent<EntityType>(id);
  if (rttr::type::get_by_name("attack").get_metadata("bits").to_uint64() == bits)
      em.AddComponent<Attack>(id);
  if (rttr::type::get_by_name("health").get_metadata("bits").to_uint64() == bits)
      em.AddComponent<Health>(id);
  if (rttr::type::get_by_name("lifespan").get_metadata("bits").to_uint64() == bits)
      em.AddComponent<Lifespan>(id);
}

rttr::instance RT_Getter( EntityManager &em, EntityID &id, size_t bits )
{
  if ( rttr::type::get_by_name( "name" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Name>( id ) );
  if ( rttr::type::get_by_name( "parent" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Parent>( id ) );
  if ( rttr::type::get_by_name( "transform" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Transform>( id ) );
  if ( rttr::type::get_by_name( "collider" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Collider>( id ) );
  if ( rttr::type::get_by_name( "rigidbody" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<RigidBody>( id ) );
  if ( rttr::type::get_by_name( "input" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Input>( id ) );
  if ( rttr::type::get_by_name( "animator" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Animator>( id ) );
  if ( rttr::type::get_by_name( "state" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<State>( id ) );
  if ( rttr::type::get_by_name( "image" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Image>( id ) );
  if ( rttr::type::get_by_name( "text" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Text>( id ) );
  if ( rttr::type::get_by_name( "renderer2D" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance( em.GetComponent<Renderer2D>( id ) );
  if (rttr::type::get_by_name("ai").get_metadata("bits").to_uint64() == bits)
      return rttr::instance(em.GetComponent<AI>(id));
  if (rttr::type::get_by_name("entity_type").get_metadata("bits").to_uint64() == bits)
      return rttr::instance(em.GetComponent<EntityType>(id));
  if (rttr::type::get_by_name("attack").get_metadata("bits").to_uint64() == bits)
      return rttr::instance(em.GetComponent<Attack>(id));
  if (rttr::type::get_by_name("health").get_metadata("bits").to_uint64() == bits)
      return rttr::instance(em.GetComponent<Health>(id));
  if (rttr::type::get_by_name("lifespan").get_metadata("bits").to_uint64() == bits)
      return rttr::instance(em.GetComponent<Lifespan>(id));
  return rttr::instance();
}

void SerializeType( const std::string &str, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, void *ptr )
{
  if ( str == "name" )
    Serialize::WriteObject( *static_cast<Name *>( ptr ), writer );
  else if ( str == "parent" )
    Serialize::WriteObject( *static_cast<Parent *>( ptr ), writer );
  else if ( str == "transform" )
    Serialize::WriteObject( *static_cast<Transform *>( ptr ), writer );
  else if ( str == "collider" )
    Serialize::WriteObject( *static_cast<Collider *>( ptr ), writer );
  else if ( str == "rigidbody" )
    Serialize::WriteObject( *static_cast<RigidBody *>( ptr ), writer );
  else if ( str == "input" )
    Serialize::WriteObject( *static_cast<Input *>( ptr ), writer );
  else if ( str == "animator" )
    Serialize::WriteObject( *static_cast<Animator *>( ptr ), writer );
  else if ( str == "state" )
    Serialize::WriteObject( *static_cast<State *>( ptr ), writer );
  else if ( str == "image" )
    Serialize::WriteObject( *static_cast<Image *>( ptr ), writer );
  else if ( str == "text" )
    Serialize::WriteObject( *static_cast<Text *>( ptr ), writer );
  else if ( str == "renderer2D" )
    Serialize::WriteObject( *static_cast<Renderer2D *>( ptr ), writer );
  else if (str == "ai")
      Serialize::WriteObject(*static_cast<AI*>(ptr), writer);
  else if (str == "entity_type")
      Serialize::WriteObject(*static_cast<EntityType*>(ptr), writer);
  else if (str == "attack")
      Serialize::WriteObject(*static_cast<Attack*>(ptr), writer);
  else if (str == "health")
      Serialize::WriteObject(*static_cast<Health*>(ptr), writer);
  else if (str == "lifespan")
      Serialize::WriteObject(*static_cast<Lifespan*>(ptr), writer);
}

void DeserializeType( const std::string &str, EntityManager &em, EntityID id, rttr::variant var )
{
  if ( str == "name" )
    em.AddComponent<Name>( id, var.get_value<Name>() );
  else if ( str == "parent" )
    em.AddComponent<Parent>( id, var.get_value<Parent>() );
  else if ( str == "transform" )
    em.AddComponent<Transform>( id, var.get_value<Transform>() );
  else if ( str == "collider" )
    em.AddComponent<Collider>( id, var.get_value<Collider>() );
  else if ( str == "rigidbody" )
    em.AddComponent<RigidBody>( id, var.get_value<RigidBody>() );
  else if ( str == "input" )
    em.AddComponent<Input>( id, var.get_value<Input>() );
  else if ( str == "animator" )
    em.AddComponent<Animator>( id, var.get_value<Animator>() );
  else if ( str == "state" )
    em.AddComponent<State>( id );
  else if ( str == "image" )
    em.AddComponent<Image>( id, var.get_value<Image>() );
  else if ( str == "text" )
    em.AddComponent<Text>( id, var.get_value<Text>() );
  else if ( str == "renderer2D" )
    em.AddComponent<Renderer2D>( id, var.get_value<Renderer2D>() );
  else if (str == "ai")
      em.AddComponent<AI>(id, var.get_value<AI>());
  else if (str == "entity_type")
      em.AddComponent<EntityType>(id, var.get_value<EntityType>());
  else if (str == "attack")
      em.AddComponent<Attack>(id, var.get_value<Attack>());
  else if (str == "health")
      em.AddComponent<Health>(id, var.get_value<Health>());
  else if (str == "lifespan")
      em.AddComponent<Lifespan>(id, var.get_value<Lifespan>());
}
}