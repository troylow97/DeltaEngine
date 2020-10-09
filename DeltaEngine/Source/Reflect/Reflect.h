#pragma once

#include "Core/Math/DE_Math.h"
#include "ECS/Components/Transform.h"
#include "ECS/Metatype.h"
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

  rttr::registration::class_<Transform>( "transform" )
      (rttr::metadata("Key", Metatype::get_metatype<Transform>() ))
    .property( "position", &Transform::position )
    .property( "rotation", &Transform::rotation )
    .property( "scale", &Transform::scale );

}

}

