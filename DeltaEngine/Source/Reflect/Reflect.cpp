/**********************************************************************************
* \file   Reflect.cpp
* \brief  The file contains the function to reflect and serialise data
* \author Chin, Clara,   25% Code Contribution
* \author Low, Troy,     25% Code Contribution
* \author Ong, Graeme,   25% Code Contribution
* \author Tan, Tong Wee, 25% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <rttr/registration>
#include "Reflect.h"

#include "EngineConfig.h"

#include "Core/Math/DE_Math.h"
#include "ECS/ComponentMeta.h"
#include "ECS/EntityManager.h"

#include "Assets/AssetKey.h"
#include "Core/Utils/Json/JsonSerialize.h"
#include "Core/GlobalStruct.h"
#include "../../Sandbox/Source/Systems/AI/Waypoint.h"
#include "../../Sandbox/Source/Systems/AI/AI_State.h"
#include "../../Sandbox/Source/Systems/EnemySpawner/EnemySpawner.h"
#include "../../Sandbox/Source/Systems/EnemySpawner/EnemyData.h"
#include "../../Sandbox/Source/Systems/RespawnSystem.h"
#include "../../Sandbox/Source/Systems/AttackSystem.h"
#include "Audio/AudioConfig.h"

namespace DeltaEngine
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Waypoint>( "Waypoint" )
    .property( "Waypoints", &Waypoint::Waypoints )
    .property( "CurrentWaypoint", &Waypoint::CurrentWaypoint );

rttr::registration::class_<EnemyData>( "EnemyData" )
    .property("transform_scale", &EnemyData::TransformScale)
    .property("collider_scale", &EnemyData::ColliderScale)
    .property("collider_offset", &EnemyData::ColliderOffset)
    .property( "health", &EnemyData::Health )
    .property( "movespeed", &EnemyData::Movespeed )
    .property( "mass", &EnemyData::Mass )
    .property( "damage", &EnemyData::Damage )
    .property("attackspeed", &EnemyData::Attackspeed);

rttr::registration::class_<LancerAIData>( "LancerAIData" )
    .property( "charge_detection_range", &LancerAIData::ChargeDetectionRange );

rttr::registration::class_<FiddlerAIData>( "FiddlerAIData" )
    .property( "waypoint", &FiddlerAIData::waypoint )
    .property( "lost_detection_range", &FiddlerAIData::ChargeDetectionRange )
    .property( "charge_detection_range", &FiddlerAIData::LostDetectionRange );

rttr::registration::class_<SerpentipedeAIData>( "SerpentipedeAIData" )
    .property( "cooldown", &SerpentipedeAIData::AttackCooldown )
    .property( "points", &SerpentipedeAIData::Points )
    .property( "detection_range", &SerpentipedeAIData::DetectionRange );

rttr::registration::class_<EnemyWave>( "EnemyWave" )
    .property( "enemy_count", &EnemyWave::EnemyCount )
    .property( "enemy_type", &EnemyWave::EnemyType )
    .property( "spawn_area", &EnemyWave::SpawnArea );

  rttr::registration::class_<Gauntlet>( "Gauntlet" )
      .property( "enemy_waves", &Gauntlet::EnemyWaves )
      .property( "activation_point", &Gauntlet::ActivationPoint )
      .property( "wall_offset_right", &Gauntlet::WallOffsetRight )
      .property( "wall_offset_left", &Gauntlet::WallOffsetLeft )
      .property( "current_enemy_wave", &Gauntlet::CurrentEnemyWave )( rttr::metadata( "NO_SERIALIZE", true ) )
      .property( "is_activated", &Gauntlet::isActivated )( rttr::metadata( "NO_SERIALIZE", true ) )
      .property( "is_finished", &Gauntlet::isFinished )( rttr::metadata( "NO_SERIALIZE", true ) );

  rttr::registration::class_<GauntletsList>( "Gauntlets" )
      .property( "gauntlets", &GauntletsList::Gauntlets );

  rttr::registration::class_<RespawnPoints>( "RespawnPoints" )
      .property( "respawn_points", &RespawnPoints::m_respawns );

  rttr::registration::class_<EngineConfig>( "Config" )
    .property( "window", &EngineConfig::win_name )
    .property( "width", &EngineConfig::width )
    .property( "height", &EngineConfig::height )
    .property( "fps", &EngineConfig::fps )
    .property( "cam_size", &EngineConfig::cam_size )
    .property( "fullscreen", &EngineConfig::fullscreen );

  rttr::registration::class_<SoundConfig>( "Sound Configuration" )
    .property( "Sound Path", &SoundConfig::path )
    .property( "Loop", &SoundConfig::loop )
    .property( "Stream", &SoundConfig::stream )
    .property( "Is 3D", &SoundConfig::is3D );

  rttr::registration::class_<BankConfig>( "Bank Configuration" )
    .property( "Bank Path", &BankConfig::path );

  rttr::registration::class_<AudioConfig>( "Audio Configuration" )
    .property( "Sounds Config", &AudioConfig::sounds_config )
    .property( "Bank Config", &AudioConfig::banks_config );

  rttr::registration::class_<Vector2>( "Vector2" )
    .property( "x", &Vector2::x )
    .property( "y", &Vector2::y );

  rttr::registration::class_<Vector3>( "Vector3" )
    .property( "x", &Vector3::x )
    .property( "y", &Vector3::y )
    .property( "z", &Vector3::z );

  rttr::registration::class_<Vector4>( "Vector4" )
    .property( "x", &Vector4::x )
    .property( "y", &Vector4::y )
    .property( "z", &Vector4::z )
    .property( "w", &Vector4::w );

  rttr::registration::class_<Quaternion>( "Quaternion" )
    .property( "x", &Quaternion::x )
    .property( "y", &Quaternion::y )
    .property( "z", &Quaternion::z )
    .property( "w", &Quaternion::w );

  rttr::registration::class_<Point>( "Point" )
    .property( "x", &Point::point_x )
    .property( "y", &Point::point_y );

  rttr::registration::class_<Color>( "Color" )
    .property( "r", &Color::r )
    .property( "g", &Color::g )
    .property( "b", &Color::b )
    .property( "a", &Color::a );

  rttr::registration::class_<Material>( "Material" )
    .property( "key", &Material::m_ShaderKey );

  rttr::registration::class_<Sprite>( "Sprite" )
    .property( "key", &Sprite::m_Key )
    .property( "index", &Sprite::m_Index );

  rttr::registration::enumeration<ColliderType>( "ColliderType" )
  (
    rttr::value( "none", ColliderType::NONE ),
    rttr::value( "box", ColliderType::BOX ),
    rttr::value( "circle", ColliderType::CIRCLE ),
    rttr::value( "line", ColliderType::LINE ),
    rttr::value( "ray", ColliderType::RAY )
  );

  rttr::registration::enumeration<EntityCategory>( "EntityCategory" )
  (
    rttr::value( "none", EntityCategory::E_NONE ),
    rttr::value( "wall", EntityCategory::E_WALL ),
    rttr::value( "player", EntityCategory::E_PLAYER ),
    rttr::value( "enemy", EntityCategory::E_ENEMY),
    rttr::value( "player bullet", EntityCategory::E_PLAYER_BULLET),
    rttr::value( "player bullet detection", EntityCategory::E_PLAYER_BULLET_DETECTION),
    rttr::value( "player punch", EntityCategory::E_PLAYER_PUNCH),
    rttr::value( "player dash", EntityCategory::E_PLAYER_DASH),
    rttr::value( "player smg", EntityCategory::E_PLAYER_SMG),
    rttr::value( "enemy bullet", EntityCategory::E_ENEMY_BULLET),
    rttr::value( "enemy fiddler punch", EntityCategory::E_ENEMY_FIDDLER_PUNCH),
    rttr::value( "enemy lancer punch", EntityCategory::E_ENEMY_LANCER_PUNCH),
    rttr::value( "vfx", EntityCategory::E_VFX )
  );

  rttr::registration::enumeration<Alignment>( "Alignment" )
  (
    rttr::value( "align_left", Alignment::AlignLeft ),
    rttr::value( "align_right", Alignment::AlignRight ),
    rttr::value( "center", Alignment::Centralize )
  );

  rttr::registration::enumeration<FillType>( "Filltype" )
  (
    rttr::value( "none", FillType::None ),
    rttr::value( "horizontal_left_to_right", FillType::HorizontalLeftToRight ),
    rttr::value( "horizontal_right_to_left", FillType::HorizontalRightToLeft ),
    rttr::value( "vertical_top_to_bottom", FillType::VerticalTopToBottom ),
    rttr::value( "vertical_bottom_to_top", FillType::VerticalBottomToTop ),
    rttr::value( "radial_360_clockwise", FillType::Radial360Clockwise ),
    rttr::value( "radial_360_anticlockwise", FillType::Radial360AntiClockwise )
  );

  rttr::registration::enumeration<ParticleEmitter::Shape>( "Shape" )
  (
    rttr::value( "None", ParticleEmitter::Shape::None ),
    rttr::value( "Circle", ParticleEmitter::Shape::Circle ),
    rttr::value( "Line", ParticleEmitter::Shape::Line ),
    rttr::value( "Box", ParticleEmitter::Shape::Box )
  );

  rttr::registration::enumeration<ParticleEmitter::GenType>( "Generation Mode" )
  (
    rttr::value( "Random", ParticleEmitter::GenType::Random ),
    rttr::value( "Loop", ParticleEmitter::GenType::Loop ),
    rttr::value( "PingPong", ParticleEmitter::GenType::PingPong ),
    rttr::value( "Spread", ParticleEmitter::GenType::Spread )
  );

  rttr::registration::enumeration<UIType>( "UIType" )
    (
      rttr::value("Screen", UIType::Screen),
      rttr::value("Interface", UIType::Interface),
      rttr::value("Button", UIType::Button),
      rttr::value("Slider", UIType::Slider),
      rttr::value("HealthBar_Fill", UIType::Healthbar),
      rttr::value("HealthBar_Base", UIType::Healthbar_base)
    );

  rttr::registration::class_<UI>( "UI" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<UI>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Function", &UI::functor_key )( rttr::policy::prop::bind_as_ptr )
    .property( "UI Type", &UI::ui_type )( rttr::policy::prop::bind_as_ptr )
    .property( "Screen", &UI::screen )( rttr::policy::prop::bind_as_ptr )
    .property( "Target", &UI::target_screen)(rttr::policy::prop::bind_as_ptr)
    .property( "Previous", &UI::previous_screen )( rttr::policy::prop::bind_as_ptr )
    .property( "Overlay", &UI::overlay)( rttr::policy::prop::bind_as_ptr );



  rttr::registration::class_<EntityName>( "Entity Name" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<EntityName>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Name", &EntityName::name )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Parent>( "Parent" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Parent>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "ID", &Parent::p_id )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Transform>( "Transform" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Transform>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Old Position", &Transform::old_position )( rttr::metadata( "NO_SERIALIZE", true ),
                                                         ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "Position", &Transform::position )( rttr::policy::prop::bind_as_ptr )
    .property( "Scale", &Transform::scale )( rttr::policy::prop::bind_as_ptr )
    .property( "Rotation", &Transform::rotation )( rttr::policy::prop::bind_as_ptr );

    rttr::registration::class_<RigidBody>( "Rigidbody" )
      ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<RigidBody>()->bits ) )
      .constructor<>()( rttr::policy::ctor::as_object )
      .property( "Direction", &RigidBody::Direction )( rttr::metadata( "NO_SERIALIZE", true ),
                                                    ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Velocity", &RigidBody::Velocity )( rttr::metadata( "NO_SERIALIZE", true ),
                                                   ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Reflected Vector", &RigidBody::ReflectedVector )( rttr::metadata( "NO_SERIALIZE", true ),
                                                                 ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Acceleration", &RigidBody::Acceleration )( rttr::metadata( "NO_SERIALIZE", true ),
                                                          ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Accumulated Force", &RigidBody::AccumulatedForce )( rttr::metadata( "NO_SERIALIZE", true ),
                                                                    ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Point End", &RigidBody::PointEnd )( rttr::metadata( "NO_SERIALIZE", true ),
                                                    ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Mass", &RigidBody::Mass )( rttr::policy::prop::bind_as_ptr )
      .property( "Move Speed", &RigidBody::Movespeed )( rttr::policy::prop::bind_as_ptr )
      .property( "Restitution", &RigidBody::Restitution )( rttr::policy::prop::bind_as_ptr )
      .property( "Friction Coefficient", &RigidBody::FrictionCoeff )( rttr::policy::prop::bind_as_ptr )
      .property( "Inherent Acceleration", &RigidBody::InherentAcceleration )( rttr::policy::prop::bind_as_ptr )(
        rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Max Acceleration", &RigidBody::MaxAcceleration )( rttr::policy::prop::bind_as_ptr )
      .property( "Acceleration Pickup", &RigidBody::AccelerationPickup )( rttr::policy::prop::bind_as_ptr )
      .property( "Gravity", &RigidBody::hasGravity )( rttr::policy::prop::bind_as_ptr )
      .property( "Moveable", &RigidBody::isMoveable )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Collider>( "Collider" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Collider>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Center", &Collider::center )( rttr::policy::prop::bind_as_ptr )( rttr::metadata( "NO_SERIALIZE", true ),( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "Offset", &Collider::offset )( rttr::policy::prop::bind_as_ptr )
    .property( "Size", &Collider::size )( rttr::policy::prop::bind_as_ptr )
    .property( "Intersection Point", &Collider::interPoint )( rttr::metadata( "NO_SERIALIZE", true ),
                                                   ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "Type", &Collider::type )
    .property( "Collision Layer", &Collider::CollisionLayerCheck )( rttr::policy::prop::bind_as_ptr )
      .property( "Collision Layer ID", &Collider::CollisionLayerID )( rttr::policy::prop::bind_as_ptr )

      .property("Trigger", &Collider::isTrigger)(rttr::policy::prop::bind_as_ptr)
      .property("Colliding On Floor", &Collider::isCollidingOnFloor)(rttr::metadata("NO_SERIALIZE", true),
          (rttr::metadata("NO_EDITOR", true)))
      .property("Platform", &Collider::isPlatform)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<Animator>( "Animator" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Animator>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Controller Key", &Animator::m_ControllerKey )( rttr::policy::prop::bind_as_ptr )
    .property( "Clip Key", &Animator::m_ClipKey )( rttr::metadata( "NO_SERIALIZE", true ),
                                                ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "Timer", &Animator::m_Timer )( rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) )
    .property( "Speed", &Animator::m_Speed )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<State>( "State" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<State>()->bits ) )
    .property( "Parameters", &State::parameters )( rttr::metadata( "NO_SERIALIZE", true ),
                                                ( rttr::metadata( "NO_EDITOR", true ) ) );

  rttr::registration::class_<Camera>( "Camera" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Camera>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Camera Size", &Camera::m_Size )( rttr::policy::prop::bind_as_ptr )
    .property( "Background Color", &Camera::backgroundColor )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Image>( "Image" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Image>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Sprite", &Image::m_Sprite )( rttr::policy::prop::bind_as_ptr )
    .property( "Size", &Image::m_Size )( rttr::policy::prop::bind_as_ptr )
    .property( "Offset", &Image::m_Offset )( rttr::policy::prop::bind_as_ptr )
    .property( "Tiling", &Image::m_Tiling )( rttr::policy::prop::bind_as_ptr )
    .property( "Fill Type", &Image::m_FillType )( rttr::policy::prop::bind_as_ptr )
    .property( "Fill Amount", &Image::m_FillAmount )( rttr::policy::prop::bind_as_ptr )
    .property( "Overall Angle", &Image::m_OverallAngle )( rttr::policy::prop::bind_as_ptr )
    .property( "Start Angle", &Image::m_StartAngle )( rttr::policy::prop::bind_as_ptr )
    .property( "End Angle", &Image::m_EndAngle )( rttr::policy::prop::bind_as_ptr )
    .property( "Flip X", &Image::m_FlipX )( rttr::policy::prop::bind_as_ptr )
    .property( "Flip Y", &Image::m_FlipY )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Text>( "Text" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Text>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Key", &Text::m_FontKey )( rttr::policy::prop::bind_as_ptr )
    .property( "Detail", &Text::m_Text )( rttr::policy::prop::bind_as_ptr )
    .property( "Alignment", &Text::alignment )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<ParticleEmitter>( "ParticleEmitter" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<ParticleEmitter>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "playOnAwake", &ParticleEmitter::playOnAwake)( rttr::policy::prop::bind_as_ptr )
    .property( "duration", &ParticleEmitter::duration)( rttr::policy::prop::bind_as_ptr )
    .property( "looping", &ParticleEmitter::looping)( rttr::policy::prop::bind_as_ptr )
    .property( "prewarm", &ParticleEmitter::prewarm)( rttr::policy::prop::bind_as_ptr )
    .property( "startDelay", &ParticleEmitter::startDelay)( rttr::policy::prop::bind_as_ptr )
    .property( "startLifetimeMin", &ParticleEmitter::startLifetimeMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startLifetimeMax", &ParticleEmitter::startLifetimeMax)( rttr::policy::prop::bind_as_ptr )
    .property( "startColorMin", &ParticleEmitter::startColorMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startColorMax", &ParticleEmitter::startColorMax)( rttr::policy::prop::bind_as_ptr )
    .property( "startRotationMin", &ParticleEmitter::startRotationMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startRotationMax", &ParticleEmitter::startRotationMax)( rttr::policy::prop::bind_as_ptr )
    .property( "startSizeMin", &ParticleEmitter::startSizeMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startSizeMax", &ParticleEmitter::startSizeMax)( rttr::policy::prop::bind_as_ptr )
    .property( "maxParticles", &ParticleEmitter::maxParticles)( rttr::policy::prop::bind_as_ptr )
    .property( "rateOverTime", &ParticleEmitter::rateOverTime)( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Renderer2D>( "Renderer2D" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Renderer2D>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Materials", &Renderer2D::m_Material )( rttr::policy::prop::bind_as_ptr )
    .property( "Colors", &Renderer2D::m_Color )( rttr::policy::prop::bind_as_ptr )
    .property( "Layer", &Renderer2D::m_SortingLayer )( rttr::policy::prop::bind_as_ptr )
    .property( "Order", &Renderer2D::m_SortingOrder )( rttr::policy::prop::bind_as_ptr )
    .property( "Active", &Renderer2D::m_Active )( rttr::policy::prop::bind_as_ptr )
    .property( "Shaded", &Renderer2D::m_Shaded )( rttr::policy::prop::bind_as_ptr )
    .property( "Wireframe", &Renderer2D::m_Wireframe )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<RendererOverlay>( "RendererOverlay" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<RendererOverlay>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Pos", &RendererOverlay::pos )( rttr::policy::prop::bind_as_ptr )
    .property( "Width/Height", &RendererOverlay::size )( rttr::policy::prop::bind_as_ptr )
    .property( "Left", &RendererOverlay::left )( rttr::policy::prop::bind_as_ptr )
    .property( "Right", &RendererOverlay::right )( rttr::policy::prop::bind_as_ptr )
    .property( "Top", &RendererOverlay::top )( rttr::policy::prop::bind_as_ptr )
    .property( "Bottom", &RendererOverlay::bottom )( rttr::policy::prop::bind_as_ptr )
    .property( "Anchor Min", &RendererOverlay::anchorMin )( rttr::policy::prop::bind_as_ptr )
    .property( "Anchor Max", &RendererOverlay::anchorMax )( rttr::policy::prop::bind_as_ptr )
    .property( "Pivot", &RendererOverlay::pivot)( rttr::policy::prop::bind_as_ptr )
    .property( "Materials", &RendererOverlay::m_Material )( rttr::policy::prop::bind_as_ptr )
    .property( "Colors", &RendererOverlay::m_Color )( rttr::policy::prop::bind_as_ptr )
    .property( "Layer", &RendererOverlay::m_SortingLayer )( rttr::policy::prop::bind_as_ptr )
    .property( "Order", &RendererOverlay::m_SortingOrder )( rttr::policy::prop::bind_as_ptr )
    .property( "Preserve Aspect", &RendererOverlay::m_PreserveAspect )( rttr::policy::prop::bind_as_ptr )
    .property( "Active", &RendererOverlay::m_Active )( rttr::policy::prop::bind_as_ptr )
    .property( "Shaded", &RendererOverlay::m_Shaded )( rttr::policy::prop::bind_as_ptr )
    .property( "Wireframe", &RendererOverlay::m_Wireframe )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Input>( "Input" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Input>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Previous", &Input::previousKey )
    .property( "Current", &Input::currentKey );

    rttr::registration::class_<AI>("AI")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<AI>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Original Point", &AI::original_point)(rttr::policy::prop::bind_as_ptr)
      .property("AIState", &AI::key)(rttr::policy::prop::bind_as_ptr)
      .property("Transition", &AI::transition)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));

  rttr::registration::class_<EntityType>( "Entity Type" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<EntityType>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Category", &EntityType::type )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Health>( "Health" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Health>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "Current Health", &Health::CurrentHealth )( rttr::policy::prop::bind_as_ptr )
    .property( "Max Health", &Health::MaxHealth )( rttr::policy::prop::bind_as_ptr )
    .property("TakenDamageTimer", &Health::isDamagedTimer)(rttr::policy::prop::bind_as_ptr)
    .property( "Invulnerable", &Health::isInvulnerable )( rttr::policy::prop::bind_as_ptr );

    rttr::registration::class_<Attack>("Attack")
        (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Attack>()->bits))
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("Range Damage", &Attack::RangedDamage)(rttr::policy::prop::bind_as_ptr)
        .property("Melee Damage", &Attack::MeleeDamage)(rttr::policy::prop::bind_as_ptr)
        .property("SMG Damage", &Attack::SMGDamage)(rttr::policy::prop::bind_as_ptr)
        .property("Melee Combo Damage", &Attack::MeleeComboDamage)(rttr::policy::prop::bind_as_ptr)
        .property("Number Of Combo", &Attack::NumberOfCombos)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true))
        .property("Max Combo Number", &Attack::MaxComboNumber)(rttr::policy::prop::bind_as_ptr)
        .property("Melee Cooldown", &Attack::MeleeCooldown)(rttr::policy::prop::bind_as_ptr)
        .property("Range Cooldown", &Attack::RangeCooldown)(rttr::policy::prop::bind_as_ptr)
        .property("SMG Cooldown", &Attack::SMGCooldown)(rttr::policy::prop::bind_as_ptr)
        .property("Melee Cooldown Timer", &Attack::MeleeCooldownTimer)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Range Cooldown Timer", &Attack::RangeCooldownTimer)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Combo Duration", &Attack::ComboDuration)(rttr::policy::prop::bind_as_ptr)
        .property("Combo Cooldown Timer", &Attack::ComboCooldownTimer)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Knockback Amount", &Attack::KnockbackAmount)(rttr::policy::prop::bind_as_ptr)
        .property("Knockback Combo Amount", &Attack::KnockbackComboAmount)(rttr::policy::prop::bind_as_ptr)
        .property("SMG Fire Rate", &Attack::SMGFireRate)(rttr::policy::prop::bind_as_ptr)
        .property("Ranged Attack", &Attack::RangeAttack)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Melee Attack", &Attack::MeleeAttack)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("SMG Attack", &Attack::SMGAttack)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Allow SMG Attack", &Attack::AllowSMGAttack)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Blocking", &Attack::Blocking)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Start SMG Cooldown Timer", &Attack::StartSMGCooldownTimer)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Start Combo Cooldown Timer", &Attack::StartComboCooldownTimer)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));

    rttr::registration::class_<Lifespan>( "Lifespan" )
      ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Lifespan>()->bits ) )
      .constructor<>()( rttr::policy::ctor::as_object )
      .property( "Timer", &Lifespan::Timer )( rttr::policy::prop::bind_as_ptr );

  rttr::registration::class_<Player>("Player")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Player>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Respawn Point", &Player::RespawnPoint)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Enemies Defeated", &Player::EnemiesDefeated)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Upgrade Points Per Enemy Defeated", &Player::UpgradePoints)(rttr::policy::prop::bind_as_ptr)
      .property("Points Needed For Upgrade", &Player::PointsNeededForUpgrade)(rttr::policy::prop::bind_as_ptr)
      .property("Attack Upgrade Increase", &Player::AttackUpgradeIncrease)(rttr::policy::prop::bind_as_ptr)
      .property("Health Upgrade Attack", &Player::HealthUpgradeIncrease)(rttr::policy::prop::bind_as_ptr)
      .property("Dashing Timer Duration", &Player::DashingTimerDuration)(rttr::policy::prop::bind_as_ptr)
      .property("Dashing Timer Cooldown", &Player::DashingTimerCooldown)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Dead", &Player::IsDead)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Jumping", &Player::IsJumping)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Dashing", &Player::IsDashing)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Dodging", &Player::IsDodging)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Start Dashing Timer", &Player::StartDashingTimer)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Allow Dashing", &Player::AllowDashing)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Dash Direction", &Player::DashDirectionRight)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Upgrade Attack", &Player::UpgradeAtk)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Upgrade Health", &Player::UpgradeHP)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Upgraded Attack", &Player::UpgradedAtk)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Upgraded Health", &Player::UpgradedHP)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));
  }

}

namespace DeltaEngine::RT_Reflect
{
rttr::type RT_Checker( size_t bits )
{
  if ( ComponentMeta::GetComponentMeta<EntityName>()->bits == bits )
    return rttr::type::get_by_name( "Entity Name" );
  if ( ComponentMeta::GetComponentMeta<Parent>()->bits == bits )
    return rttr::type::get_by_name( "Parent" );
  if ( ComponentMeta::GetComponentMeta<Transform>()->bits == bits )
    return rttr::type::get_by_name( "Transform" );
  if ( ComponentMeta::GetComponentMeta<Collider>()->bits == bits )
    return rttr::type::get_by_name( "Collider" );
  if ( ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits )
    return rttr::type::get_by_name( "Rigidbody" );
  if ( ComponentMeta::GetComponentMeta<Input>()->bits == bits )
    return rttr::type::get_by_name( "Input" );
  if ( ComponentMeta::GetComponentMeta<Animator>()->bits == bits )
    return rttr::type::get_by_name( "Animator" );
  if ( ComponentMeta::GetComponentMeta<State>()->bits == bits )
    return rttr::type::get_by_name( "State" );
  if ( ComponentMeta::GetComponentMeta<Image>()->bits == bits )
    return rttr::type::get_by_name( "Image" );
  if ( ComponentMeta::GetComponentMeta<Text>()->bits == bits )
    return rttr::type::get_by_name( "Text" );
  if ( ComponentMeta::GetComponentMeta<ParticleEmitter>()->bits == bits )
    return rttr::type::get_by_name( "ParticleEmitter" );
  if ( ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits )
    return rttr::type::get_by_name( "Renderer2D" );
  if ( ComponentMeta::GetComponentMeta<RendererOverlay>()->bits == bits )
    return rttr::type::get_by_name( "RendererOverlay" );
  if ( ComponentMeta::GetComponentMeta<AI>()->bits == bits )
    return rttr::type::get_by_name( "AI" );
  if ( ComponentMeta::GetComponentMeta<EntityType>()->bits == bits )
    return rttr::type::get_by_name( "Entity Type" );
  if ( ComponentMeta::GetComponentMeta<Attack>()->bits == bits )
    return rttr::type::get_by_name( "Attack" );
  if ( ComponentMeta::GetComponentMeta<Health>()->bits == bits )
    return rttr::type::get_by_name( "Health" );
  if ( ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits )
    return rttr::type::get_by_name( "Lifespan" );
  if ( ComponentMeta::GetComponentMeta<Player>()->bits == bits )
    return rttr::type::get_by_name( "Player" );
  if ( ComponentMeta::GetComponentMeta<Camera>()->bits == bits )
    return rttr::type::get_by_name( "Camera" );
  if ( ComponentMeta::GetComponentMeta<UI>()->bits == bits )
    return rttr::type::get_by_name( "UI" );
  return rttr::type::get<int>();
}

void RT_Destroy( EntityManager &em, EntityID id, size_t bits )
{
  if ( ComponentMeta::GetComponentMeta<EntityName>()->bits == bits )
    em.RemoveComponent<EntityName>( id );
  if ( ComponentMeta::GetComponentMeta<Parent>()->bits == bits )
    em.RemoveComponent<Parent>( id );
  if ( ComponentMeta::GetComponentMeta<Transform>()->bits == bits )
    em.RemoveComponent<Transform>( id );
  if ( ComponentMeta::GetComponentMeta<Collider>()->bits == bits )
    em.RemoveComponent<Collider>( id );
  if ( ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits )
    em.RemoveComponent<RigidBody>( id );
  if ( ComponentMeta::GetComponentMeta<Input>()->bits == bits )
    em.RemoveComponent<Input>( id );
  if ( ComponentMeta::GetComponentMeta<Animator>()->bits == bits )
    em.RemoveComponent<Animator>( id );
  if ( ComponentMeta::GetComponentMeta<State>()->bits == bits )
    em.RemoveComponent<State>( id );
  if ( ComponentMeta::GetComponentMeta<Image>()->bits == bits )
    em.RemoveComponent<Image>( id );
  if ( ComponentMeta::GetComponentMeta<Text>()->bits == bits )
    em.RemoveComponent<Text>( id );
  if ( ComponentMeta::GetComponentMeta<ParticleEmitter>()->bits == bits )
    em.RemoveComponent<ParticleEmitter>( id );
  if ( ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits )
    em.RemoveComponent<Renderer2D>( id );
  if ( ComponentMeta::GetComponentMeta<RendererOverlay>()->bits == bits )
    em.RemoveComponent<RendererOverlay>( id );
  if ( ComponentMeta::GetComponentMeta<AI>()->bits == bits )
    em.RemoveComponent<AI>( id );
  if ( ComponentMeta::GetComponentMeta<EntityType>()->bits == bits )
    em.RemoveComponent<EntityType>( id );
  if ( ComponentMeta::GetComponentMeta<Attack>()->bits == bits )
    em.RemoveComponent<Attack>( id );
  if ( ComponentMeta::GetComponentMeta<Health>()->bits == bits )
    em.RemoveComponent<Health>( id );
  if ( ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits )
    em.RemoveComponent<Lifespan>( id );
  if ( ComponentMeta::GetComponentMeta<Player>()->bits == bits )
    em.RemoveComponent<Player>( id );
  if ( ComponentMeta::GetComponentMeta<Camera>()->bits == bits )
    em.RemoveComponent<Camera>( id );
  if ( ComponentMeta::GetComponentMeta<UI>()->bits == bits )
    em.RemoveComponent<UI>( id );
}

void RT_Setter( EntityManager &em, EntityID id, size_t bits )
{
  if ( ComponentMeta::GetComponentMeta<EntityName>()->bits == bits )
    em.AddComponent<EntityName>( id );
  if ( ComponentMeta::GetComponentMeta<Parent>()->bits == bits )
    em.AddComponent<Parent>( id );
  if ( ComponentMeta::GetComponentMeta<Transform>()->bits == bits )
    em.AddComponent<Transform>( id );
  if ( ComponentMeta::GetComponentMeta<Collider>()->bits == bits )
    em.AddComponent<Collider>( id );
  if ( ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits )
    em.AddComponent<RigidBody>( id );
  if ( ComponentMeta::GetComponentMeta<Input>()->bits == bits )
    em.AddComponent<Input>( id );
  if ( ComponentMeta::GetComponentMeta<Animator>()->bits == bits )
    em.AddComponent<Animator>( id );
  if ( ComponentMeta::GetComponentMeta<State>()->bits == bits )
    em.AddComponent<State>( id );
  if ( ComponentMeta::GetComponentMeta<Image>()->bits == bits )
    em.AddComponent<Image>( id );
  if ( ComponentMeta::GetComponentMeta<Text>()->bits == bits )
    em.AddComponent<Text>( id );
  if ( ComponentMeta::GetComponentMeta<ParticleEmitter>()->bits == bits )
    em.AddComponent<ParticleEmitter>( id );
  if ( ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits )
    em.AddComponent<Renderer2D>( id );
  if ( ComponentMeta::GetComponentMeta<RendererOverlay>()->bits == bits )
    em.AddComponent<RendererOverlay>( id );
  if ( ComponentMeta::GetComponentMeta<AI>()->bits == bits )
    em.AddComponent<AI>( id );
  if ( ComponentMeta::GetComponentMeta<EntityType>()->bits == bits )
    em.AddComponent<EntityType>( id );
  if ( ComponentMeta::GetComponentMeta<Attack>()->bits == bits )
    em.AddComponent<Attack>( id );
  if ( ComponentMeta::GetComponentMeta<Health>()->bits == bits )
    em.AddComponent<Health>( id );
  if ( ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits )
    em.AddComponent<Lifespan>( id );
  if ( ComponentMeta::GetComponentMeta<Player>()->bits == bits )
    em.AddComponent<Player>( id );
  if ( ComponentMeta::GetComponentMeta<Camera>()->bits == bits )
    em.AddComponent<Camera>( id );
  if ( ComponentMeta::GetComponentMeta<UI>()->bits == bits )
    em.AddComponent<UI>( id );
}

rttr::instance RT_Getter( EntityManager &em, EntityID &id, size_t bits )
{
  if ( ComponentMeta::GetComponentMeta<EntityName>()->bits == bits )
    return rttr::instance( em.GetComponent<EntityName>( id ) );
  if ( ComponentMeta::GetComponentMeta<Parent>()->bits == bits )
    return rttr::instance( em.GetComponent<Parent>( id ) );
  if ( ComponentMeta::GetComponentMeta<Transform>()->bits == bits )
    return rttr::instance( em.GetComponent<Transform>( id ) );
  if ( ComponentMeta::GetComponentMeta<Collider>()->bits == bits )
    return rttr::instance( em.GetComponent<Collider>( id ) );
  if ( ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits )
    return rttr::instance( em.GetComponent<RigidBody>( id ) );
  if ( ComponentMeta::GetComponentMeta<Input>()->bits == bits )
    return rttr::instance( em.GetComponent<Input>( id ) );
  if ( ComponentMeta::GetComponentMeta<Animator>()->bits == bits )
    return rttr::instance( em.GetComponent<Animator>( id ) );
  if ( ComponentMeta::GetComponentMeta<State>()->bits == bits )
    return rttr::instance( em.GetComponent<State>( id ) );
  if ( ComponentMeta::GetComponentMeta<Image>()->bits == bits )
    return rttr::instance( em.GetComponent<Image>( id ) );
  if ( ComponentMeta::GetComponentMeta<Text>()->bits == bits )
    return rttr::instance( em.GetComponent<Text>( id ) );
  if ( ComponentMeta::GetComponentMeta<ParticleEmitter>()->bits == bits )
    return rttr::instance( em.GetComponent<ParticleEmitter>( id ) );
  if ( ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits )
    return rttr::instance( em.GetComponent<Renderer2D>( id ) );
  if ( ComponentMeta::GetComponentMeta<RendererOverlay>()->bits == bits )
    return rttr::instance( em.GetComponent<RendererOverlay>( id ) );
  if ( ComponentMeta::GetComponentMeta<AI>()->bits == bits )
    return rttr::instance( em.GetComponent<AI>( id ) );
  if ( ComponentMeta::GetComponentMeta<EntityType>()->bits == bits )
    return rttr::instance( em.GetComponent<EntityType>( id ) );
  if ( ComponentMeta::GetComponentMeta<Attack>()->bits == bits )
    return rttr::instance( em.GetComponent<Attack>( id ) );
  if ( ComponentMeta::GetComponentMeta<Health>()->bits == bits )
    return rttr::instance( em.GetComponent<Health>( id ) );
  if ( ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits )
    return rttr::instance( em.GetComponent<Lifespan>( id ) );
  if ( ComponentMeta::GetComponentMeta<Player>()->bits == bits )
    return rttr::instance( em.GetComponent<Player>( id ) );
  if ( ComponentMeta::GetComponentMeta<Camera>()->bits == bits )
    return rttr::instance( em.GetComponent<Camera>( id ) );
  if ( ComponentMeta::GetComponentMeta<UI>()->bits == bits )
    return rttr::instance( em.GetComponent<UI>( id ) );
  return rttr::instance();
}

void SerializeType( const std::string &str, rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, void *ptr )
{
  if ( str == "Entity Name" )
    Serialize::WriteObject( *static_cast<EntityName *>( ptr ), writer );
  else if ( str == "Parent" )
    Serialize::WriteObject( *static_cast<Parent *>( ptr ), writer );
  else if ( str == "Transform" )
    Serialize::WriteObject( *static_cast<Transform *>( ptr ), writer );
  else if ( str == "Collider" )
    Serialize::WriteObject( *static_cast<Collider *>( ptr ), writer );
  else if ( str == "Rigidbody" )
    Serialize::WriteObject( *static_cast<RigidBody *>( ptr ), writer );
  else if ( str == "Input" )
    Serialize::WriteObject( *static_cast<Input *>( ptr ), writer );
  else if ( str == "Animator" )
    Serialize::WriteObject( *static_cast<Animator *>( ptr ), writer );
  else if ( str == "State" )
    Serialize::WriteObject( *static_cast<State *>( ptr ), writer );
  else if ( str == "Image" )
    Serialize::WriteObject( *static_cast<Image *>( ptr ), writer );
  else if ( str == "Text" )
    Serialize::WriteObject( *static_cast<Text *>( ptr ), writer );
  else if ( str == "ParticleEmitter" )
    Serialize::WriteObject( *static_cast<ParticleEmitter*>( ptr ), writer );
  else if ( str == "Renderer2D" )
    Serialize::WriteObject( *static_cast<Renderer2D *>( ptr ), writer );
  else if ( str == "RendererOverlay" )
    Serialize::WriteObject( *static_cast<RendererOverlay*>( ptr ), writer );
  else if ( str == "AI" )
    Serialize::WriteObject( *static_cast<AI *>( ptr ), writer );
  else if ( str == "Entity Type" )
    Serialize::WriteObject( *static_cast<EntityType *>( ptr ), writer );
  else if ( str == "Attack" )
    Serialize::WriteObject( *static_cast<Attack *>( ptr ), writer );
  else if ( str == "Health" )
    Serialize::WriteObject( *static_cast<Health *>( ptr ), writer );
  else if ( str == "Lifespan" )
    Serialize::WriteObject( *static_cast<Lifespan *>( ptr ), writer );
  else if ( str == "Player" )
    Serialize::WriteObject( *static_cast<Player *>( ptr ), writer );
  else if ( str == "Camera" )
    Serialize::WriteObject( *static_cast<Camera *>( ptr ), writer );
  else if ( str == "UI" )
    Serialize::WriteObject( *static_cast<UI *>( ptr ), writer );
}

void DeserializeType( const std::string &str, EntityManager &em, EntityID id, rttr::variant var )
{
  if ( str == "Entity Name" )
    em.GetComponent<EntityName>( id ) = var.get_value<EntityName>();
  else if ( str == "Parent" )
    em.GetComponent<Parent>( id ) = var.get_value<Parent>();
  else if ( str == "Transform" )
    em.GetComponent<Transform>( id ) = var.get_value<Transform>();
  else if ( str == "Entity Type" )
    em.GetComponent<EntityType>( id ) = var.get_value<EntityType>();
  else if ( str == "Collider" )
    em.AddComponent<Collider>( id, var.get_value<Collider>() );
  else if ( str == "Rigidbody" )
    em.AddComponent<RigidBody>( id, var.get_value<RigidBody>() );
  else if ( str == "Input" )
    em.AddComponent<Input>( id, var.get_value<Input>() );
  else if ( str == "Animator" )
    em.AddComponent<Animator>( id, var.get_value<Animator>() );
  else if ( str == "State" )
    em.AddComponent<State>( id );
  else if ( str == "Image" )
    em.AddComponent<Image>( id, var.get_value<Image>() );
  else if ( str == "Text" )
    em.AddComponent<Text>( id, var.get_value<Text>() );
  else if ( str == "ParticleEmitter" )
    em.AddComponent<ParticleEmitter>( id, var.get_value<ParticleEmitter>() );
  else if ( str == "Renderer2D" )
    em.AddComponent<Renderer2D>( id, var.get_value<Renderer2D>() );
  else if ( str == "RendererOverlay" )
    em.AddComponent<RendererOverlay>( id, var.get_value<RendererOverlay>() );
  else if ( str == "AI" )
    em.AddComponent<AI>( id, var.get_value<AI>() );
  else if ( str == "Attack" )
    em.AddComponent<Attack>( id, var.get_value<Attack>() );
  else if ( str == "Health" )
    em.AddComponent<Health>( id, var.get_value<Health>() );
  else if ( str == "Lifespan" )
    em.AddComponent<Lifespan>( id, var.get_value<Lifespan>() );
  else if ( str == "Player" )
    em.AddComponent<Player>( id, var.get_value<Player>() );
  else if ( str == "Camera" )
    em.AddComponent<Camera>( id, var.get_value<Camera>() );
  else if ( str == "UI" )
    em.AddComponent<UI>( id, var.get_value<UI>() );
}
}