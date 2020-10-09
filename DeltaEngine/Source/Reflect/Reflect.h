#pragma once

#include "Core/Math/DE_Math.h"
#include "ECS/Metatype.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/RigidBody.h"
#include <rttr/registration>

namespace DeltaEngine
{

RTTR_REGISTRATION
{

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
    ( rttr::metadata( "hash", Metatype::get_metatype<Transform>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "old_position", &Transform::old_position)
    .property( "position", &Transform::position )
    .property( "scale", &Transform::scale )
    .property( "rotation", &Transform::rotation )
    .method( "serialize", &Transform::Serialize )
    .method( "deserialize", &Transform::Deserialize );

  rttr::registration::class_<RigidBody>( "rigidbody" )
    ( rttr::metadata( "hash", Metatype::get_metatype<RigidBody>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "velocity", &RigidBody::Velocity )
    .property( "acceleration", &RigidBody::Acceleration )
    .property( "mass", &RigidBody::Mass )
    .property( "has_gravity", &RigidBody::hasGravity )
    .method( "serialize", &RigidBody::Serialize )
    .method( "deserialize", &RigidBody::Deserialize );

  rttr::registration::class_<Collider>( "collider" )
    ( rttr::metadata( "hash", Metatype::get_metatype<Collider>()->hash.digest ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "inter_point", &Collider::interPoint )
    .property( "direction_vector", &Collider::DirectionVector )
    .property( "reflection_vector", &Collider::ReflectionVector )
    .property( "point_end", &Collider::PointEnd )
    .property( "normal", &Collider::normal )
    .property( "center", &Collider::center )
    .property( "size", &Collider::size )
    .property( "type", &Collider::type )
    .property( "inter_time", &Collider::interTime )
    .property( "is_collideable", &Collider::isCollideable )
    .method( "serialize", &Collider::Serialize )
    .method( "deserialize", &Collider::Deserialize );

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
    return rttr::type::get<int>();
  }
}

}

