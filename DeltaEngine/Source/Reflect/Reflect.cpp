#pragma once

#include "Reflect.h"

#include "EngineConfig.h"

#include "Core/Math/DE_Math.h"
#include "ECS/ComponentMeta.h"
#include "ECS/EntityManager.h"

#include <rttr/registration>
#include "Core/Utils/Json/JsonSerialize.h"

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

  rttr::registration::enumeration<ColliderType>( "collider_type" )
  (
      rttr::value( "none", ColliderType::NONE ),
      rttr::value( "box", ColliderType::BOX ),
      rttr::value( "circle", ColliderType::CIRCLE ),
      rttr::value( "line", ColliderType::LINE ),
      rttr::value( "ray", ColliderType::RAY )
  );

  rttr::registration::class_<Transform>( "transform" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Transform>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "old_position", &Transform::old_position )(rttr::policy::prop::bind_as_ptr)
    .property( "position", &Transform::position )(rttr::policy::prop::bind_as_ptr)
    .property( "scale", &Transform::scale )(rttr::policy::prop::bind_as_ptr)
    .property( "rotation", &Transform::rotation )(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<RigidBody>( "rigidbody" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<RigidBody>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "direction", &RigidBody::Direction )
    .property( "velocity", &RigidBody::Velocity )
    .property( "reflected_vector", &RigidBody::ReflectedVector )
    .property( "acceleration", &RigidBody::Acceleration )
    .property("accumulated_force", &RigidBody::AccumulatedForce)
    .property("point_end", &RigidBody::PointEnd)
    .property( "mass", &RigidBody::Mass )(rttr::policy::prop::bind_as_ptr)
    .property( "movespeed", &RigidBody::Movespeed )(rttr::policy::prop::bind_as_ptr)
    .property("restitution", &RigidBody::Restitution)(rttr::policy::prop::bind_as_ptr)
    .property("friction_coeff", &RigidBody::FrictionCoeff)(rttr::policy::prop::bind_as_ptr)
    .property( "has_gravity", &RigidBody::hasGravity )(rttr::policy::prop::bind_as_ptr)
    .property( "is_moveable", &RigidBody::isMoveable )(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<Collider>( "collider" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Collider>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property("center", &Collider::center)
    .property("size", &Collider::size)
    .property( "inter_point", &Collider::interPoint )
    .property( "type", &Collider::type )
    .property( "is_collideable", &Collider::isCollideable )(rttr::policy::prop::bind_as_ptr)
    .property("is_collideable", &Collider::isCollided);


  rttr::registration::class_<Input>( "input" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Input>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "prev", &Input::previousKey )
    .property( "curr", &Input::currentKey );
}

}

namespace DeltaEngine::RT_Reflect
{

rttr::type RT_Checker( size_t bits )
{
  if ( rttr::type::get_by_name( "transform" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "transform" );
  if ( rttr::type::get_by_name( "collider" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "collider" );
  if ( rttr::type::get_by_name( "rigidbody" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "rigidbody" );
  if ( rttr::type::get_by_name( "input" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::type::get_by_name( "input" );
  return rttr::type::get<int>();
}

rttr::instance RT_Getter (EntityManager& em, EntityID& id, size_t bits)
{
  if ( rttr::type::get_by_name( "transform" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance(em.GetComponent<Transform>(id));
  if ( rttr::type::get_by_name( "collider" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance(em.GetComponent<Collider>(id));
  if ( rttr::type::get_by_name( "rigidbody" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance(em.GetComponent<RigidBody>(id));
  if ( rttr::type::get_by_name( "input" ).get_metadata( "bits" ).to_uint64() == bits )
    return rttr::instance(em.GetComponent<Input>(id));
  return rttr::instance();
}

void SerializeType(const std::string& str, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer , void* ptr)
{
  if (str == "transform" )
    Serialize::WriteObject( *static_cast<Transform *>(ptr), writer );
  else if (str == "collider" )
    Serialize::WriteObject( *static_cast<Collider *>(ptr), writer );
  else if (str == "rigidbody" )
    Serialize::WriteObject( *static_cast<RigidBody *>(ptr), writer );
  else if (str == "input" )
    Serialize::WriteObject( *static_cast<Input *>(ptr), writer );
}

void DeserializeType(const std::string& str, EntityManager& em, EntityID id, rttr::variant var)
{
  if (str == "transform" )
    em.AddComponent<Transform>( id, var.get_value<Transform>() );
  else if (str == "collider" )
    em.AddComponent<Collider>( id, var.get_value<Collider>() );
  else if (str == "rigidbody" )
    em.AddComponent<RigidBody>( id, var.get_value<RigidBody>() );
  else if (str == "input" )
    em.AddComponent<Input>( id, var.get_value<Input>() );
}

}