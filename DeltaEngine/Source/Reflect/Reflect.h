#pragma once

#include "Core/Math/DE_Math.h"
#include "ECS/Metatype.h"
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/Character.h"
#include "EngineConfig.h"
#include <rttr/registration>

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
    ( rttr::metadata( "hash", Metatype::GetMetatype<Transform>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "old_position", &Transform::old_position)
    .property( "position", &Transform::position )
    .property( "scale", &Transform::scale )
    .property( "rotation", &Transform::rotation )
    .method( "serialize", &Transform::Serialize )
    .method( "deserialize", &Transform::Deserialize );

  rttr::registration::class_<RigidBody>( "rigidbody" )
    ( rttr::metadata( "hash", Metatype::GetMetatype<RigidBody>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "direction", &RigidBody::Direction)
    .property( "velocity", &RigidBody::Velocity )
    .property( "reflected_vector", &RigidBody::ReflectedVector)
    .property( "acceleration", &RigidBody::Acceleration )
    .property( "mass", &RigidBody::Mass )
    .property("friction", &RigidBody::Friction)
    .property( "movespeed", &RigidBody::Movespeed)
    .property("inherent_acceleration", &RigidBody::inherentAcceleration)
    .property( "has_gravity", &RigidBody::hasGravity )
    .property("is_moveable", &RigidBody::isMoveable)
    .method( "serialize", &RigidBody::Serialize )
    .method( "deserialize", &RigidBody::Deserialize );

  rttr::registration::class_<Collider>( "collider" )
    ( rttr::metadata( "hash", Metatype::GetMetatype<Collider>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "inter_point", &Collider::interPoint )
    .property( "center", &Collider::center )
    .property( "size", &Collider::size )
    .property( "inter_point", &Collider::interPoint )
    .property( "type", &Collider::type )
    .property( "is_collideable", &Collider::isCollideable )
    .method( "serialize", &Collider::Serialize )
    .method( "deserialize", &Collider::Deserialize );


  rttr::registration::class_<Input>( "input" )
    ( rttr::metadata( "hash", Metatype::GetMetatype<Input>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "prev", &Input::previousKey )
    .property("curr", &Input::currentKey)
    .method( "serialize", &Input::Serialize )
    .method( "deserialize", &Input::Deserialize );

}

  namespace reflect
{
  inline rttr::type checker(size_t hash)
  {
    if ( rttr::type::get_by_name( "transform" ).get_metadata( "hash" ).to_uint64() == hash )
      return rttr::type::get_by_name( "transform" );
    else if ( rttr::type::get_by_name( "collider" ).get_metadata( "hash" ).to_uint64() == hash )
      return rttr::type::get_by_name( "collider" );
    else if ( rttr::type::get_by_name( "rigidbody" ).get_metadata( "hash" ).to_uint64() == hash )
      return rttr::type::get_by_name( "rigidbody" );
    else if ( rttr::type::get_by_name( "input" ).get_metadata( "hash" ).to_uint64() == hash )
      return rttr::type::get_by_name( "input" );
    return rttr::type::get<int>();
  }
}

}

