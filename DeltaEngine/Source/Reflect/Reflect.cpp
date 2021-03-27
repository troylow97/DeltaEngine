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
#include "Components/ParticleEmitter.h"

namespace DeltaEngine
{
  RTTR_REGISTRATION
  {
    /*
    ********************************************************************************
    * Engine Configurations
    * Configuration structs for Engine / Audio FMOD
    ********************************************************************************
    */
#pragma region
    rttr::registration::class_<EngineConfig>("Config")
      .property("window", &EngineConfig::win_name)
      .property("width", &EngineConfig::width)
      .property("height", &EngineConfig::height)
      .property("fps", &EngineConfig::fps)
      .property("cam_size", &EngineConfig::cam_size)
      .property("fullscreen", &EngineConfig::fullscreen);

    rttr::registration::class_<SoundConfig>("Sound Configuration")
      .property("Sound Path", &SoundConfig::path)
      .property("Loop", &SoundConfig::loop)
      .property("Stream", &SoundConfig::stream)
      .property("Is 3D", &SoundConfig::is3D);

    rttr::registration::class_<BankConfig>("Bank Configuration")
      .property("Bank Path", &BankConfig::path);

    rttr::registration::class_<AudioConfig>("Audio Configuration")
      .property("Sounds Config", &AudioConfig::sounds_config)
      .property("Bank Config", &AudioConfig::banks_config);

#pragma endregion 
    /*
    ********************************************************************************
    */
    rttr::registration::enumeration<FillType>("Filltype")
        (
            rttr::value("none", FillType::None),
            rttr::value("horizontal_left_to_right", FillType::HorizontalLeftToRight),
            rttr::value("horizontal_right_to_left", FillType::HorizontalRightToLeft),
            rttr::value("vertical_top_to_bottom", FillType::VerticalTopToBottom),
            rttr::value("vertical_bottom_to_top", FillType::VerticalBottomToTop),
            rttr::value("radial_360_clockwise", FillType::Radial360Clockwise),
            rttr::value("radial_360_anticlockwise", FillType::Radial360AntiClockwise)
            );

    rttr::registration::class_<Waypoint>("Waypoint")
      .property("Waypoints", &Waypoint::Waypoints)
      .property("CurrentWaypoint", &Waypoint::CurrentWaypoint);

    rttr::registration::class_<EnemyData>("EnemyData")
      .property("transform_scale", &EnemyData::TransformScale)
      .property("collider_scale", &EnemyData::ColliderScale)
      .property("collider_offset", &EnemyData::ColliderOffset)
      .property("health", &EnemyData::Health)
      .property("movespeed", &EnemyData::Movespeed)
      .property("mass", &EnemyData::Mass)
      .property("damage", &EnemyData::Damage)
      .property("attackspeed", &EnemyData::Attackspeed);

    rttr::registration::class_<LancerAIData>("LancerAIData")
      .property("charge_detection_range", &LancerAIData::ChargeDetectionRange);

    rttr::registration::class_<FiddlerAIData>("FiddlerAIData")
      .property("waypoint", &FiddlerAIData::waypoint)
      .property("lost_detection_range", &FiddlerAIData::ChargeDetectionRange)
      .property("charge_detection_range", &FiddlerAIData::LostDetectionRange);

    rttr::registration::class_<SerpentipedeAIData>("SerpentipedeAIData")
      .property("cooldown", &SerpentipedeAIData::AttackCooldown)
      .property("points", &SerpentipedeAIData::Points)
      .property("detection_range", &SerpentipedeAIData::DetectionRange);
    
    rttr::registration::class_<EnemyWave>("EnemyWave")
      .property("enemy_count", &EnemyWave::EnemyCount)
      .property("enemy_type", &EnemyWave::EnemyType)
      .property("spawn_area", &EnemyWave::SpawnArea);

    rttr::registration::class_<Gauntlet>("Gauntlet")
      .property("enemy_waves", &Gauntlet::EnemyWaves)
      .property("activation_point", &Gauntlet::ActivationPoint)
      .property("wall_offset_right", &Gauntlet::WallOffsetRight)
      .property("wall_offset_left", &Gauntlet::WallOffsetLeft)
      .property("current_enemy_wave", &Gauntlet::CurrentEnemyWave)(rttr::metadata("NO_SERIALIZE", true))
      .property("is_activated", &Gauntlet::isActivated)(rttr::metadata("NO_SERIALIZE", true))
      .property("is_finished", &Gauntlet::isFinished)(rttr::metadata("NO_SERIALIZE", true));

    rttr::registration::class_<GauntletsList>("Gauntlets")
      .property("gauntlets", &GauntletsList::Gauntlets);

    rttr::registration::class_<RespawnPoints>("RespawnPoints")
      .property("respawn_points", &RespawnPoints::m_respawns);

    rttr::registration::class_<Vector2>("Vector2")
      .property("x", &Vector2::x)
      .property("y", &Vector2::y);

    rttr::registration::class_<Vector3>("Vector3")
      .property("x", &Vector3::x)
      .property("y", &Vector3::y)
      .property("z", &Vector3::z);

    rttr::registration::class_<Vector4>("Vector4")
      .property("x", &Vector4::x)
      .property("y", &Vector4::y)
      .property("z", &Vector4::z)
      .property("w", &Vector4::w);

    rttr::registration::class_<Quaternion>("Quaternion")
      .property("x", &Quaternion::x)
      .property("y", &Quaternion::y)
      .property("z", &Quaternion::z)
      .property("w", &Quaternion::w);

  rttr::registration::class_<BezierCurve>("BezierCurve")
    .property("Anchors", &BezierCurve::anchors)
    .property("ControlsLeft", &BezierCurve::controlsLeft)
    .property("ControlsRight", &BezierCurve::controlsRight)
    .property("Min", &BezierCurve::min)
    .property("Max", &BezierCurve::max);

  rttr::registration::class_<BezierRange>("BezierRange")
    .property("min", &BezierRange::min)
    .property("max", &BezierRange::max)
    .property("type", &BezierRange::type);

  rttr::registration::class_<BezierRange3>("BezierRange3")
    .property("minX", &BezierRange3::minX)
    .property("maxX", &BezierRange3::maxX)
    .property("minY", &BezierRange3::minY)
    .property("maxY", &BezierRange3::maxY)
    .property("minZ", &BezierRange3::minZ)
    .property("maxZ", &BezierRange3::maxZ)
    .property("type", &BezierRange3::type);

  rttr::registration::class_<Gradient>("Gradient")
    .property("AlphaKeys", &Gradient::alphaKeys)
    .property("ColorKeys", &Gradient::colorKeys);

  rttr::registration::class_<GradientRange>("GradientRange")
    .property("min", &GradientRange::min)
    .property("max", &GradientRange::max)
    .property("type", &GradientRange::type);

  rttr::registration::class_<Material>( "Material" )
    .property( "key", &Material::m_ShaderKey );

    rttr::registration::class_<Color>("Color")
      .property("r", &Color::r)
      .property("g", &Color::g)
      .property("b", &Color::b)
      .property("a", &Color::a);

    rttr::registration::class_<Material>("Material")
      .property("key", &Material::m_ShaderKey);

    rttr::registration::class_<Sprite>("Sprite")
      .property("key", &Sprite::m_Key)
      .property("index", &Sprite::m_Index);

    rttr::registration::enumeration<ColliderType>("ColliderType")
    (
      rttr::value("none", ColliderType::NONE),
      rttr::value("box", ColliderType::BOX),
      rttr::value("circle", ColliderType::CIRCLE),
      rttr::value("line", ColliderType::LINE),
      rttr::value("ray", ColliderType::RAY)
    );

    rttr::registration::enumeration<EntityCategory>("EntityCategory")
    (
      rttr::value("none", EntityCategory::E_NONE),
      rttr::value("wall", EntityCategory::E_WALL),
      rttr::value("player", EntityCategory::E_PLAYER),
      rttr::value("player body part rotatable", EntityCategory::E_PLAYER_BODYPART_ROTATABLE),
      rttr::value("player body part", EntityCategory::E_PLAYER_BODYPART),
      rttr::value("enemy", EntityCategory::E_ENEMY),
      rttr::value("player bullet", EntityCategory::E_PLAYER_BULLET),
      rttr::value("player bullet detection", EntityCategory::E_PLAYER_BULLET_DETECTION),
      rttr::value("player punch", EntityCategory::E_PLAYER_PUNCH),
      rttr::value("player dash", EntityCategory::E_PLAYER_DASH),
      rttr::value("player smg", EntityCategory::E_PLAYER_SMG),
      rttr::value("enemy bullet", EntityCategory::E_ENEMY_BULLET),
      rttr::value("enemy fiddler punch", EntityCategory::E_ENEMY_FIDDLER_PUNCH),
      rttr::value("enemy lancer punch", EntityCategory::E_ENEMY_LANCER_PUNCH),
      rttr::value("vfx", EntityCategory::E_VFX),
      rttr::value("checkpoint", EntityCategory::E_CHECKPOINT),
      rttr::value("mud", EntityCategory::E_MUD),
      rttr::value("concrete", EntityCategory::E_CONCRETE),
      rttr::value("healthup", EntityCategory::E_HEALTHUP),
      rttr::value("healthup_used", EntityCategory::E_HEALTHUP_USED)
    );

    rttr::registration::enumeration<Alignment>("Alignment")
    (
      rttr::value("align_left", Alignment::AlignLeft),
      rttr::value("align_right", Alignment::AlignRight),
      rttr::value("center", Alignment::Centralize)
    );

  rttr::registration::enumeration<Gradient::Type>( "Gradient Type" )
  (
    rttr::value( "ConstantColor", Gradient::Type::ConstantColor ),
    rttr::value( "ConstantGradient", Gradient::Type::ConstantGradient ),
    rttr::value( "RandomBetweenColors", Gradient::Type::RandomBetweenColors ),
    rttr::value( "RandomBetweenGradients", Gradient::Type::RandomBetweenGradients )
  );

  rttr::registration::enumeration<BezierCurve::Type>( "Bezier Type" )
  (
    rttr::value( "Constant", BezierCurve::Type::Constant ),
    rttr::value( "ConstantCurve", BezierCurve::Type::ConstantCurve ),
    rttr::value( "RandomBetweenConstants", BezierCurve::Type::RandomBetweenConstants ),
    rttr::value( "RandomBetweenCurves", BezierCurve::Type::RandomBetweenCurves )
  );

  rttr::registration::enumeration<ParticleEmitter::Shape>( "Particle Emitter Shape" )
  (
    rttr::value( "None", ParticleEmitter::Shape::None ),
    rttr::value( "Circle", ParticleEmitter::Shape::Circle ),
    rttr::value( "Line", ParticleEmitter::Shape::Line ),
    rttr::value( "Box", ParticleEmitter::Shape::Box )
  );

  rttr::registration::enumeration<ParticleEmitter::GenType>( "Particle Emitter Generation Mode" )
  (
    rttr::value( "Random", ParticleEmitter::GenType::Random ),
    rttr::value( "Loop", ParticleEmitter::GenType::Loop ),
    rttr::value( "PingPong", ParticleEmitter::GenType::PingPong ),
    rttr::value( "Spread", ParticleEmitter::GenType::Spread )
  );

    rttr::registration::enumeration<ParticleEmitter::GenType>("Generation Mode")
    (
      rttr::value("Random", ParticleEmitter::GenType::Random),
      rttr::value("Loop", ParticleEmitter::GenType::Loop),
      rttr::value("PingPong", ParticleEmitter::GenType::PingPong),
      rttr::value("Spread", ParticleEmitter::GenType::Spread)
    );


    /*
    ********************************************************************************
    * Core System
    * Components for Core
    ********************************************************************************
    */
#pragma region 
    rttr::registration::class_<Parent>("Parent")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Parent>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("ID", &Parent::p_id)(rttr::policy::prop::bind_as_ptr);


    rttr::registration::class_<EntityName>("Entity Name")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityName>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Name", &EntityName::name)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<EntityType>("Entity Type")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityType>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Category", &EntityType::type)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<AudioSource>( "Audio Source" )
      ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<AudioSource>()->bits ) )
      .constructor<>()( rttr::policy::ctor::as_object )
      .property("Audio File", &AudioSource::clip)(rttr::policy::prop::bind_as_ptr )
      .property( "Source ID", &AudioSource::id )(rttr::metadata("NO_SERIALIZE", true),
                                                  (rttr::metadata("NO_EDITOR", true)))
      .property( "Volume", &AudioSource::volume )( rttr::policy::prop::bind_as_ptr )
      .property( "Pitch", &AudioSource::pitch )( rttr::policy::prop::bind_as_ptr )
      .property( "Loop", &AudioSource::loop )( rttr::policy::prop::bind_as_ptr )
      .property( "Is Loop", &AudioSource::isLoop )( rttr::policy::prop::bind_as_ptr )
      .property( "Is Event", &AudioSource::isEvent )( rttr::policy::prop::bind_as_ptr )
      .property("Is Start", &AudioSource::isStart)(rttr::metadata("NO_SERIALIZE", true),
                                                    (rttr::metadata("NO_EDITOR", true)));

#pragma endregion  
    /*
    ********************************************************************************
    */
    /*
    ********************************************************************************
    * GUI System
    * Components for GUI
    ********************************************************************************
    */
#pragma region 

    rttr::registration::enumeration<GUIType>("GUIType")
      (
      rttr::value("Canvas", GUIType::Canvas),
      rttr::value("Text", GUIType::Text),
      rttr::value("Image", GUIType::Image),
      rttr::value("Button", GUIType::Button),
      rttr::value("Toggle", GUIType::Toggle),
      rttr::value("Slider", GUIType::Slider)
      );

    rttr::registration::class_<GUI>("GUI")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<GUI>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Type", &GUI::type)(rttr::policy::prop::bind_as_ptr)
      .property("Screen", &GUI::screen)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Button>( "Button" )
      ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Button>()->bits ) )
      .constructor<>()( rttr::policy::ctor::as_object )
      .property( "On Hover", &Button::on_hover )( rttr::policy::prop::bind_as_ptr )
      .property( "On Click", &Button::on_click )( rttr::policy::prop::bind_as_ptr )
      .property( "On Exit", &Button::on_exit )( rttr::policy::prop::bind_as_ptr )
      ("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true));


    rttr::registration::class_<Toggle>( "Toggle" )
      ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Toggle>()->bits ) )
      .constructor<>()( rttr::policy::ctor::as_object )
      .property( "On Change", &Toggle::on_change )( rttr::policy::prop::bind_as_ptr )
      .property( "Value", &Toggle::value )( rttr::policy::prop::bind_as_ptr );

    rttr::registration::class_<Slider>("Slider")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Slider>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Fill Entity", &Slider::fill_entity)(rttr::policy::prop::bind_as_ptr)
      .property("Handle Entity", &Slider::handle_entity)(rttr::policy::prop::bind_as_ptr)
      .property("Min", &Slider::min)(rttr::policy::prop::bind_as_ptr)
      .property("Max", &Slider::max)(rttr::policy::prop::bind_as_ptr)
      .property("Value", &Slider::value)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<RigidBody>("Rigidbody")
    (rttr::metadata("bits", ComponentMeta::GetComponentMeta<RigidBody>()->bits))
    .constructor<>()(rttr::policy::ctor::as_object)
    .property("Direction", &RigidBody::Direction)(rttr::metadata("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true)))
    .property("Velocity", &RigidBody::Velocity)(rttr::metadata("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true)))
    .property("Reflected Vector", &RigidBody::ReflectedVector)(rttr::metadata("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true)))
    .property("Acceleration", &RigidBody::Acceleration)(rttr::metadata("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true)))
    .property("Accumulated Force", &RigidBody::AccumulatedForce)(rttr::metadata("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true)))
    .property("Point End", &RigidBody::PointEnd)(rttr::metadata("NO_SERIALIZE", true),
      (rttr::metadata("NO_EDITOR", true)))
    .property("Mass", &RigidBody::Mass)(rttr::policy::prop::bind_as_ptr)
    .property("Move Speed", &RigidBody::Movespeed)(rttr::policy::prop::bind_as_ptr)
    .property("Restitution", &RigidBody::Restitution)(rttr::policy::prop::bind_as_ptr)
    .property("Friction Coefficient", &RigidBody::FrictionCoeff)(rttr::policy::prop::bind_as_ptr)
    .property("Inherent Acceleration", &RigidBody::InherentAcceleration)(rttr::policy::prop::bind_as_ptr)(
      rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
    .property("Max Acceleration", &RigidBody::MaxAcceleration)(rttr::policy::prop::bind_as_ptr)
    .property("Acceleration Pickup", &RigidBody::AccelerationPickup)(rttr::policy::prop::bind_as_ptr)
    .property("Gravity", &RigidBody::hasGravity)(rttr::policy::prop::bind_as_ptr)
    .property("Moveable", &RigidBody::isMoveable)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Transform>("Transform")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Transform>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Position", &Transform::position)(rttr::policy::prop::bind_as_ptr)
      .property("Scale", &Transform::scale)(rttr::policy::prop::bind_as_ptr)
      .property("Rotation", &Transform::rotation)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<RigidBody>("Rigidbody")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<RigidBody>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Direction", &RigidBody::Direction)(rttr::metadata("NO_SERIALIZE", true),
                                                    (rttr::metadata("NO_EDITOR", true)))
      .property("Velocity", &RigidBody::Velocity)(rttr::metadata("NO_SERIALIZE", true),
                                                  (rttr::metadata("NO_EDITOR", true)))
      .property("Reflected Vector", &RigidBody::ReflectedVector)(rttr::metadata("NO_SERIALIZE", true),
                                                                 (rttr::metadata("NO_EDITOR", true)))
      .property("Acceleration", &RigidBody::Acceleration)(rttr::metadata("NO_SERIALIZE", true),
                                                          (rttr::metadata("NO_EDITOR", true)))
      .property("Accumulated Force", &RigidBody::AccumulatedForce)(rttr::metadata("NO_SERIALIZE", true),
                                                                   (rttr::metadata("NO_EDITOR", true)))
      .property("Point End", &RigidBody::PointEnd)(rttr::metadata("NO_SERIALIZE", true),
                                                   (rttr::metadata("NO_EDITOR", true)))
      .property("Mass", &RigidBody::Mass)(rttr::policy::prop::bind_as_ptr)
      .property("Move Speed", &RigidBody::Movespeed)(rttr::policy::prop::bind_as_ptr)
      .property("Restitution", &RigidBody::Restitution)(rttr::policy::prop::bind_as_ptr)
      .property("Friction Coefficient", &RigidBody::FrictionCoeff)(rttr::policy::prop::bind_as_ptr)
      .property("Inherent Acceleration", &RigidBody::InherentAcceleration)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Max Acceleration", &RigidBody::MaxAcceleration)(rttr::policy::prop::bind_as_ptr)
      .property("Acceleration Pickup", &RigidBody::AccelerationPickup)(rttr::policy::prop::bind_as_ptr)
      .property("Gravity", &RigidBody::hasGravity)(rttr::policy::prop::bind_as_ptr)
      .property("Moveable", &RigidBody::isMoveable)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Collider>("Collider")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Collider>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Center", &Collider::center)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Offset", &Collider::offset)(rttr::policy::prop::bind_as_ptr)
      .property("Size", &Collider::size)(rttr::policy::prop::bind_as_ptr)
      .property("Intersection Point", &Collider::interPoint)(rttr::metadata("NO_SERIALIZE", true),
                                                             (rttr::metadata("NO_EDITOR", true)))
      .property("Type", &Collider::type)
      .property("Collision Layer", &Collider::CollisionLayerCheck)(rttr::policy::prop::bind_as_ptr)
      .property("Collision Layer ID", &Collider::CollisionLayerID)(rttr::policy::prop::bind_as_ptr)

      .property("Trigger", &Collider::isTrigger)(rttr::policy::prop::bind_as_ptr)
      .property("Colliding On Floor", &Collider::isCollidingOnFloor)(rttr::metadata("NO_SERIALIZE", true),
                                                                     (rttr::metadata("NO_EDITOR", true)))
      .property("Platform", &Collider::isPlatform)(rttr::policy::prop::bind_as_ptr);

#pragma endregion
    /*
    ********************************************************************************
    */
    /*
    ********************************************************************************
    * Render System
    * Components for Render
    ********************************************************************************
    */
#pragma region

    rttr::registration::class_<Image>("Image")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Image>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Sprite", &Image::m_Sprite)(rttr::policy::prop::bind_as_ptr)
      .property("Size", &Image::m_Size)(rttr::policy::prop::bind_as_ptr)
      .property("Offset", &Image::m_Offset)(rttr::policy::prop::bind_as_ptr)
      .property("Tiling", &Image::m_Tiling)(rttr::policy::prop::bind_as_ptr)
      .property("Fill Type", &Image::m_FillType)(rttr::policy::prop::bind_as_ptr)
      .property("Fill Amount", &Image::m_FillAmount)(rttr::policy::prop::bind_as_ptr)
      .property("Overall Angle", &Image::m_OverallAngle)(rttr::policy::prop::bind_as_ptr)
      .property("Start Angle", &Image::m_StartAngle)(rttr::policy::prop::bind_as_ptr)
      .property("End Angle", &Image::m_EndAngle)(rttr::policy::prop::bind_as_ptr)
      .property("Flip X", &Image::m_FlipX)(rttr::policy::prop::bind_as_ptr)
      .property("Flip Y", &Image::m_FlipY)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Renderer2D>("Renderer2D")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Renderer2D>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Materials", &Renderer2D::m_Material)(rttr::policy::prop::bind_as_ptr)
      .property("Colors", &Renderer2D::m_Color)(rttr::policy::prop::bind_as_ptr)
      .property("Layer", &Renderer2D::m_SortingLayer)(rttr::policy::prop::bind_as_ptr)
      .property("Order", &Renderer2D::m_SortingOrder)(rttr::policy::prop::bind_as_ptr)
      .property("Active", &Renderer2D::m_Active)(rttr::policy::prop::bind_as_ptr)
      .property("Shaded", &Renderer2D::m_Shaded)(rttr::policy::prop::bind_as_ptr)
      .property("Wireframe", &Renderer2D::m_Wireframe)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<RendererOverlay>("RendererOverlay")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<RendererOverlay>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Pos", &RendererOverlay::pos)(rttr::policy::prop::bind_as_ptr)
      .property("Width/Height", &RendererOverlay::size)(rttr::policy::prop::bind_as_ptr)
      .property("Left", &RendererOverlay::left)(rttr::policy::prop::bind_as_ptr)
      .property("Right", &RendererOverlay::right)(rttr::policy::prop::bind_as_ptr)
      .property("Top", &RendererOverlay::top)(rttr::policy::prop::bind_as_ptr)
      .property("Bottom", &RendererOverlay::bottom)(rttr::policy::prop::bind_as_ptr)
      .property("Anchor Min", &RendererOverlay::anchorMin)(rttr::policy::prop::bind_as_ptr)
      .property("Anchor Max", &RendererOverlay::anchorMax)(rttr::policy::prop::bind_as_ptr)
      .property("Pivot", &RendererOverlay::pivot)(rttr::policy::prop::bind_as_ptr)
      .property("Materials", &RendererOverlay::m_Material)(rttr::policy::prop::bind_as_ptr)
      .property("Colors", &RendererOverlay::m_Color)(rttr::policy::prop::bind_as_ptr)
      .property("Layer", &RendererOverlay::m_SortingLayer)(rttr::policy::prop::bind_as_ptr)
      .property("Order", &RendererOverlay::m_SortingOrder)(rttr::policy::prop::bind_as_ptr)
      .property("Preserve Aspect", &RendererOverlay::m_PreserveAspect)(rttr::policy::prop::bind_as_ptr)
      .property("Active", &RendererOverlay::m_Active)(rttr::policy::prop::bind_as_ptr)
      .property("Shaded", &RendererOverlay::m_Shaded)(rttr::policy::prop::bind_as_ptr)
      .property("Wireframe", &RendererOverlay::m_Wireframe)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Text>("Text")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Text>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Key", &Text::m_FontKey)(rttr::policy::prop::bind_as_ptr)
      .property("Detail", &Text::m_Text)(rttr::policy::prop::bind_as_ptr)
      .property("Alignment", &Text::alignment)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Animator>( "Animator" )
      ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<Animator>()->bits ) )
      .constructor<>()( rttr::policy::ctor::as_object )
      .property( "Controller Key", &Animator::m_ControllerKey )( rttr::policy::prop::bind_as_ptr )
      .property( "Clip Key", &Animator::m_ClipKey )( rttr::metadata( "NO_SERIALIZE", true ),
                                                     ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Timer", &Animator::m_Timer )( rttr::metadata( "NO_SERIALIZE", true ), ( rttr::metadata( "NO_EDITOR", true ) ) )
      .property( "Speed", &Animator::m_Speed )( rttr::policy::prop::bind_as_ptr );


    rttr::registration::class_<State>("State")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<State>()->bits))
      .property("Parameters", &State::parameters)(rttr::metadata("NO_SERIALIZE", true),
                                                  (rttr::metadata("NO_EDITOR", true)));

    rttr::registration::class_<Camera>("Camera")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Camera>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Camera Size", &Camera::m_Size)(rttr::policy::prop::bind_as_ptr)
      .property("Background Color", &Camera::backgroundColor)(rttr::policy::prop::bind_as_ptr);

  rttr::registration::class_<ParticleEmitter>( "ParticleEmitter" )
    ( rttr::metadata( "bits", ComponentMeta::GetComponentMeta<ParticleEmitter>()->bits ) )
    .constructor<>()( rttr::policy::ctor::as_object )
    .property( "playOnAwake", &ParticleEmitter::playOnAwake)( rttr::policy::prop::bind_as_ptr )
    .property( "duration", &ParticleEmitter::duration)( rttr::policy::prop::bind_as_ptr )
    .property( "looping", &ParticleEmitter::looping)( rttr::policy::prop::bind_as_ptr )
    //.property( "prewarm", &ParticleEmitter::prewarm)( rttr::policy::prop::bind_as_ptr )
    //.property( "startDelay", &ParticleEmitter::startDelay)( rttr::policy::prop::bind_as_ptr )
    .property( "startLifetimeMin", &ParticleEmitter::startLifetimeMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startLifetimeMax", &ParticleEmitter::startLifetimeMax)( rttr::policy::prop::bind_as_ptr )
    .property( "startColorMin", &ParticleEmitter::startColorMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startColorMax", &ParticleEmitter::startColorMax)( rttr::policy::prop::bind_as_ptr )
    .property( "startRotationMin", &ParticleEmitter::startRotationMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startRotationMax", &ParticleEmitter::startRotationMax)( rttr::policy::prop::bind_as_ptr )
    .property( "startSizeMin", &ParticleEmitter::startSizeMin)( rttr::policy::prop::bind_as_ptr )
    .property( "startSizeMax", &ParticleEmitter::startSizeMax)( rttr::policy::prop::bind_as_ptr )
    .property( "velocityOverLifetime", &ParticleEmitter::velocityOverLifetime)( rttr::policy::prop::bind_as_ptr )
    .property( "colorOverLifetime", &ParticleEmitter::colorOverLifetime)( rttr::policy::prop::bind_as_ptr )
    .property( "rotationOverLifetime", &ParticleEmitter::rotationOverLifetime)( rttr::policy::prop::bind_as_ptr )
    .property( "sizeOverLifetime", &ParticleEmitter::sizeOverLifetime)( rttr::policy::prop::bind_as_ptr )
    .property( "maxParticles", &ParticleEmitter::maxParticles)( rttr::policy::prop::bind_as_ptr )
    .property( "rateOverTime", &ParticleEmitter::rateOverTime)( rttr::policy::prop::bind_as_ptr )
    .property( "shape", &ParticleEmitter::shape)( rttr::policy::prop::bind_as_ptr )
    .property( "genType", &ParticleEmitter::genType)( rttr::policy::prop::bind_as_ptr )
    .property( "radius", &ParticleEmitter::radius)( rttr::policy::prop::bind_as_ptr )
    .property( "shapeTransform", &ParticleEmitter::shapeTransform)( rttr::policy::prop::bind_as_ptr );

#pragma endregion
    /*
    ********************************************************************************
    */

    rttr::registration::class_<Input>("Input")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Input>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Previous", &Input::previousKey)
      .property("Current", &Input::currentKey);

    rttr::registration::class_<AI>("AI")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<AI>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Original Point", &AI::original_point)(rttr::policy::prop::bind_as_ptr)
      .property("AIState", &AI::key)(rttr::policy::prop::bind_as_ptr)
      .property("Transition", &AI::transition)(rttr::metadata("NO_SERIALIZE", true),
                                               (rttr::metadata("NO_EDITOR", true)));

    rttr::registration::class_<Health>("Health")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Health>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Current Health", &Health::CurrentHealth)(rttr::policy::prop::bind_as_ptr)
      .property("Max Health", &Health::MaxHealth)(rttr::policy::prop::bind_as_ptr)
      .property("TakenDamageTimer", &Health::isDamagedTimer)(rttr::policy::prop::bind_as_ptr)
      .property("Invulnerable", &Health::isInvulnerable)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Attack>("Attack")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Attack>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Range Damage", &Attack::RangedDamage)(rttr::policy::prop::bind_as_ptr)
      .property("Melee Damage", &Attack::MeleeDamage)(rttr::policy::prop::bind_as_ptr)
      .property("SMG Damage", &Attack::SMGDamage)(rttr::policy::prop::bind_as_ptr)
      .property("Melee Combo Damage", &Attack::MeleeComboDamage)(rttr::policy::prop::bind_as_ptr)
      .property("Number Of Combo", &Attack::NumberOfCombos)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true))
      .property("Max Combo Number", &Attack::MaxComboNumber)(rttr::policy::prop::bind_as_ptr)
      .property("Melee Cooldown", &Attack::MeleeCooldown)(rttr::policy::prop::bind_as_ptr)
      .property("Range Cooldown", &Attack::RangeCooldown)(rttr::policy::prop::bind_as_ptr)
      .property("SMG Cooldown", &Attack::SMGCooldown)(rttr::policy::prop::bind_as_ptr)
      .property("Melee Cooldown Timer", &Attack::MeleeCooldownTimer)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Range Cooldown Timer", &Attack::RangeCooldownTimer)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Combo Duration", &Attack::ComboDuration)(rttr::policy::prop::bind_as_ptr)
      .property("Combo Cooldown Timer", &Attack::ComboCooldownTimer)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Knockback Amount", &Attack::KnockbackAmount)(rttr::policy::prop::bind_as_ptr)
      .property("Knockback Combo Amount", &Attack::KnockbackComboAmount)(rttr::policy::prop::bind_as_ptr)
      .property("SMG Fire Rate", &Attack::SMGFireRate)(rttr::policy::prop::bind_as_ptr)
      .property("Attack Delay", &Attack::AttackDelay)(rttr::policy::prop::bind_as_ptr)
      .property("Current Dodge Cooldown", &Attack::CurrentDodgeCooldown)(rttr::policy::prop::bind_as_ptr)(
          rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Dodge Cooldown", &Attack::DodgeCooldown)(rttr::policy::prop::bind_as_ptr)
      .property("Ranged Attack", &Attack::RangeAttack)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Melee Attack", &Attack::MeleeAttack)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("SMG Attack", &Attack::SMGAttack)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Allow SMG Attack", &Attack::AllowSMGAttack)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Blocking", &Attack::Blocking)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Start SMG Cooldown Timer", &Attack::StartSMGCooldownTimer)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Start Combo Cooldown Timer", &Attack::StartComboCooldownTimer)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Damage Enemy", &Attack::DamageEnemy)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));

    rttr::registration::class_<Lifespan>("Lifespan")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Lifespan>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Timer", &Lifespan::Timer)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Player>("Player")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Player>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Respawn Point", &Player::RespawnPoint)(rttr::metadata("NO_SERIALIZE", true),
                                                        (rttr::metadata("NO_EDITOR", true)))
      .property("Enemies Defeated", &Player::EnemiesDefeated)(rttr::metadata("NO_SERIALIZE", true),
                                                              (rttr::metadata("NO_EDITOR", true)))
      .property("Upgrade Points Per Enemy Defeated", &Player::UpgradePoints)(rttr::policy::prop::bind_as_ptr)
      .property("Points Needed For Upgrade", &Player::PointsNeededForUpgrade)(rttr::policy::prop::bind_as_ptr)
      .property("Attack Upgrade Increase", &Player::AttackUpgradeIncrease)(rttr::policy::prop::bind_as_ptr)
      .property("Health Upgrade Attack", &Player::HealthUpgradeIncrease)(rttr::policy::prop::bind_as_ptr)
      .property("Dashing Timer Duration", &Player::DashingTimerDuration)(rttr::policy::prop::bind_as_ptr)
      .property("Dashing Timer Cooldown", &Player::DashingTimerCooldown)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Fading Timer", &Player::FadingTimer)(rttr::policy::prop::bind_as_ptr)
      .property("Fading Countdown", &Player::FadingCountdown)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Dead", &Player::IsDead)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Running", &Player::IsRunning)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Jumping", &Player::IsJumping)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Dashing", &Player::IsDashing)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Dodging", &Player::IsDodging)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Punching", &Player::IsPunching)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Shooting", &Player::IsShooting)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Is Blocking", &Player::IsBlocking)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Allow Running", &Player::AllowRunning)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Allow Punching", &Player::AllowPunching)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Allow Jumping", &Player::AllowJumping)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Allow Shooting", &Player::AllowShooting)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
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
  rttr::type RT_Checker(size_t bits)
  {
    switch (bits)
    {
    case ComponentMeta::ComponentBits<EntityName>():
      return rttr::type::get_by_name("Entity Name");
    case ComponentMeta::ComponentBits<Parent>():
      return rttr::type::get_by_name("Parent");
    case ComponentMeta::ComponentBits<Transform>():
      return rttr::type::get_by_name("Transform");
    case ComponentMeta::ComponentBits<Collider>():
      return rttr::type::get_by_name("Collider");
    case ComponentMeta::ComponentBits<RigidBody>():
      return rttr::type::get_by_name("Rigidbody");
    case ComponentMeta::ComponentBits<Input>():
      return rttr::type::get_by_name("Input");
    case ComponentMeta::ComponentBits<Animator>():
      return rttr::type::get_by_name("Animator");
    case ComponentMeta::ComponentBits<State>():
      return rttr::type::get_by_name("State");
    case ComponentMeta::ComponentBits<Image>():
      return rttr::type::get_by_name("Image");
    case ComponentMeta::ComponentBits<Text>():
      return rttr::type::get_by_name("Text");
    case ComponentMeta::ComponentBits<ParticleEmitter>():
      return rttr::type::get_by_name("ParticleEmitter");
    case ComponentMeta::ComponentBits<Renderer2D>():
      return rttr::type::get_by_name("Renderer2D");
    case ComponentMeta::ComponentBits<RendererOverlay>():
      return rttr::type::get_by_name("RendererOverlay");
    case ComponentMeta::ComponentBits<AI>():
      return rttr::type::get_by_name("AI");
    case ComponentMeta::ComponentBits<EntityType>():
      return rttr::type::get_by_name("Entity Type");
    case ComponentMeta::ComponentBits<Attack>():
      return rttr::type::get_by_name("Attack");
    case ComponentMeta::ComponentBits<Health>():
      return rttr::type::get_by_name("Health");
    case ComponentMeta::ComponentBits<Lifespan>():
      return rttr::type::get_by_name("Lifespan");
    case ComponentMeta::ComponentBits<Player>():
      return rttr::type::get_by_name("Player");
    case ComponentMeta::ComponentBits<Camera>():
      return rttr::type::get_by_name("Camera");
    case ComponentMeta::ComponentBits<GUI>():
      return rttr::type::get_by_name("GUI");
    case ComponentMeta::ComponentBits<Button>():
      return rttr::type::get_by_name("Button");
    case ComponentMeta::ComponentBits<Toggle>():
      return rttr::type::get_by_name("Toggle");
    case ComponentMeta::ComponentBits<Slider>():
      return rttr::type::get_by_name("Slider");
    case ComponentMeta::ComponentBits<AudioSource>():
      return rttr::type::get_by_name("Audio Source");
    default:
      return rttr::type::get<int>();
    }
  }

  void RT_Destroy(EntityManager& em, EntityID id, size_t bits)
  {
    switch (bits)
    {
    case ComponentMeta::ComponentBits<EntityName>():
      em.RemoveComponent<EntityName>(id);
      break;
    case ComponentMeta::ComponentBits<Parent>():
      em.RemoveComponent<Parent>(id);
      break;
    case ComponentMeta::ComponentBits<Transform>():
      em.RemoveComponent<Transform>(id);
      break;
    case ComponentMeta::ComponentBits<Collider>():
      em.RemoveComponent<Collider>(id);
      break;
    case ComponentMeta::ComponentBits<RigidBody>():
      em.RemoveComponent<RigidBody>(id);
      break;
    case ComponentMeta::ComponentBits<Input>():
      em.RemoveComponent<Input>(id);
      break;
    case ComponentMeta::ComponentBits<Animator>():
      em.RemoveComponent<Animator>(id);
      break;
    case ComponentMeta::ComponentBits<State>():
      em.RemoveComponent<State>(id);
      break;
    case ComponentMeta::ComponentBits<Image>():
      em.RemoveComponent<Image>(id);
      break;
    case ComponentMeta::ComponentBits<Text>():
      em.RemoveComponent<Text>(id);
      break;
    case ComponentMeta::ComponentBits<ParticleEmitter>():
      em.RemoveComponent<ParticleEmitter>(id);
      break;
    case ComponentMeta::ComponentBits<Renderer2D>():
      em.RemoveComponent<Renderer2D>(id);
      break;
    case ComponentMeta::ComponentBits<RendererOverlay>():
      em.RemoveComponent<RendererOverlay>(id);
      break;
    case ComponentMeta::ComponentBits<AI>():
      em.RemoveComponent<AI>(id);
      break;
    case ComponentMeta::ComponentBits<EntityType>():
      em.RemoveComponent<EntityType>(id);
      break;
    case ComponentMeta::ComponentBits<Attack>():
      em.RemoveComponent<Attack>(id);
      break;
    case ComponentMeta::ComponentBits<Health>():
      em.RemoveComponent<Health>(id);
      break;
    case ComponentMeta::ComponentBits<Lifespan>():
      em.RemoveComponent<Lifespan>(id);
      break;
    case ComponentMeta::ComponentBits<Player>():
      em.RemoveComponent<Player>(id);
      break;
    case ComponentMeta::ComponentBits<Camera>():
      em.RemoveComponent<Camera>(id);
      break;
    case ComponentMeta::ComponentBits<GUI>():
      em.RemoveComponent<GUI>(id);
      break;
    case ComponentMeta::ComponentBits<Button>():
      em.RemoveComponent<Button>(id);
      break;
    case ComponentMeta::ComponentBits<Toggle>():
      em.RemoveComponent<Toggle>(id);
      break;
    case ComponentMeta::ComponentBits<Slider>():
      em.RemoveComponent<Slider>(id);
      break;
    case ComponentMeta::ComponentBits<AudioSource>():
      em.RemoveComponent<AudioSource>(id);
      break;
    }
  }

  void RT_Setter(EntityManager& em, EntityID id, size_t bits)
  {
    switch (bits)
    {
    case ComponentMeta::ComponentBits<EntityName>():
      em.AddComponent<EntityName>(id);
      break;
    case ComponentMeta::ComponentBits<Parent>():
      em.AddComponent<Parent>(id);
      break;
    case ComponentMeta::ComponentBits<Transform>():
      em.AddComponent<Transform>(id);
      break;
    case ComponentMeta::ComponentBits<Collider>():
      em.AddComponent<Collider>(id);
      break;
    case ComponentMeta::ComponentBits<RigidBody>():
      em.AddComponent<RigidBody>(id);
      break;
    case ComponentMeta::ComponentBits<Input>():
      em.AddComponent<Input>(id);
      break;
    case ComponentMeta::ComponentBits<Animator>():
      em.AddComponent<Animator>(id);
      break;
    case ComponentMeta::ComponentBits<State>():
      em.AddComponent<State>(id);
      break;
    case ComponentMeta::ComponentBits<Image>():
      em.AddComponent<Image>(id);
      break;
    case ComponentMeta::ComponentBits<Text>():
      em.AddComponent<Text>(id);
      break;
    case ComponentMeta::ComponentBits<ParticleEmitter>():
      em.AddComponent<ParticleEmitter>(id);
      break;
    case ComponentMeta::ComponentBits<Renderer2D>():
      em.AddComponent<Renderer2D>(id);
      break;
    case ComponentMeta::ComponentBits<RendererOverlay>():
      em.AddComponent<RendererOverlay>(id);
      break;
    case ComponentMeta::ComponentBits<AI>():
      em.AddComponent<AI>(id);
      break;
    case ComponentMeta::ComponentBits<EntityType>():
      em.AddComponent<EntityType>(id);
      break;
    case ComponentMeta::ComponentBits<Attack>():
      em.AddComponent<Attack>(id);
      break;
    case ComponentMeta::ComponentBits<Health>():
      em.AddComponent<Health>(id);
      break;
    case ComponentMeta::ComponentBits<Lifespan>():
      em.AddComponent<Lifespan>(id);
      break;
    case ComponentMeta::ComponentBits<Player>():
      em.AddComponent<Player>(id);
      break;
    case ComponentMeta::ComponentBits<Camera>():
      em.AddComponent<Camera>(id);
      break;
    case ComponentMeta::ComponentBits<GUI>():
      em.AddComponent<GUI>(id);
      break;
    case ComponentMeta::ComponentBits<Button>():
      em.AddComponent<Button>(id);
      break;
    case ComponentMeta::ComponentBits<Toggle>():
      em.AddComponent<Toggle>(id);
      break;
    case ComponentMeta::ComponentBits<Slider>():
      em.AddComponent<Slider>(id);
      break;
    case ComponentMeta::ComponentBits<AudioSource>():
      em.AddComponent<AudioSource>(id);
      break;
    }
  }

  rttr::instance RT_Getter(EntityManager& em, EntityID& id, size_t bits)
  {
    switch (bits)
    {
    case ComponentMeta::ComponentBits<EntityName>():
      return rttr::instance(em.GetComponent<EntityName>(id));
    case ComponentMeta::ComponentBits<Parent>():
      return rttr::instance(em.GetComponent<Parent>(id));
    case ComponentMeta::ComponentBits<Transform>():
      return rttr::instance(em.GetComponent<Transform>(id));
    case ComponentMeta::ComponentBits<Collider>():
      return rttr::instance(em.GetComponent<Collider>(id));
    case ComponentMeta::ComponentBits<RigidBody>():
      return rttr::instance(em.GetComponent<RigidBody>(id));
    case ComponentMeta::ComponentBits<Input>():
      return rttr::instance(em.GetComponent<Input>(id));
    case ComponentMeta::ComponentBits<Animator>():
      return rttr::instance(em.GetComponent<Animator>(id));
    case ComponentMeta::ComponentBits<State>():
      return rttr::instance(em.GetComponent<State>(id));
    case ComponentMeta::ComponentBits<Image>():
      return rttr::instance(em.GetComponent<Image>(id));
    case ComponentMeta::ComponentBits<Text>():
      return rttr::instance(em.GetComponent<Text>(id));
    case ComponentMeta::ComponentBits<ParticleEmitter>():
      return rttr::instance(em.GetComponent<ParticleEmitter>(id));
    case ComponentMeta::ComponentBits<Renderer2D>():
      return rttr::instance(em.GetComponent<Renderer2D>(id));
    case ComponentMeta::ComponentBits<RendererOverlay>():
      return rttr::instance(em.GetComponent<RendererOverlay>(id));
    case ComponentMeta::ComponentBits<AI>():
      return rttr::instance(em.GetComponent<AI>(id));
    case ComponentMeta::ComponentBits<EntityType>():
      return rttr::instance(em.GetComponent<EntityType>(id));
    case ComponentMeta::ComponentBits<Attack>():
      return rttr::instance(em.GetComponent<Attack>(id));
    case ComponentMeta::ComponentBits<Health>():
      return rttr::instance(em.GetComponent<Health>(id));
    case ComponentMeta::ComponentBits<Lifespan>():
      return rttr::instance(em.GetComponent<Lifespan>(id));
    case ComponentMeta::ComponentBits<Player>():
      return rttr::instance(em.GetComponent<Player>(id));
    case ComponentMeta::ComponentBits<Camera>():
      return rttr::instance(em.GetComponent<Camera>(id));
    case ComponentMeta::ComponentBits<GUI>():
      return rttr::instance(em.GetComponent<GUI>(id));
    case ComponentMeta::ComponentBits<Button>():
      return rttr::instance(em.GetComponent<Button>(id));
    case ComponentMeta::ComponentBits<Toggle>():
      return rttr::instance(em.GetComponent<Toggle>(id));
    case ComponentMeta::ComponentBits<Slider>():
      return rttr::instance(em.GetComponent<Slider>(id));
    case ComponentMeta::ComponentBits<AudioSource>() :
      return rttr::instance( em.GetComponent<AudioSource>( id ) );
    default:
      return rttr::instance();
    }
  }

  void SerializeType(const std::string& str, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& writer, void* ptr)
  {
    if (str == "Entity Name")
      Serialize::WriteObject(*static_cast<EntityName*>(ptr), writer);
    else if (str == "Parent")
      Serialize::WriteObject(*static_cast<Parent*>(ptr), writer);
    else if (str == "Transform")
      Serialize::WriteObject(*static_cast<Transform*>(ptr), writer);
    else if (str == "Collider")
      Serialize::WriteObject(*static_cast<Collider*>(ptr), writer);
    else if (str == "Rigidbody")
      Serialize::WriteObject(*static_cast<RigidBody*>(ptr), writer);
    else if (str == "Input")
      Serialize::WriteObject(*static_cast<Input*>(ptr), writer);
    else if (str == "Animator")
      Serialize::WriteObject(*static_cast<Animator*>(ptr), writer);
    else if (str == "State")
      Serialize::WriteObject(*static_cast<State*>(ptr), writer);
    else if (str == "Image")
      Serialize::WriteObject(*static_cast<Image*>(ptr), writer);
    else if (str == "Text")
      Serialize::WriteObject(*static_cast<Text*>(ptr), writer);
    else if (str == "ParticleEmitter")
      Serialize::WriteObject(*static_cast<ParticleEmitter*>(ptr), writer);
    else if (str == "Renderer2D")
      Serialize::WriteObject(*static_cast<Renderer2D*>(ptr), writer);
    else if (str == "RendererOverlay")
      Serialize::WriteObject(*static_cast<RendererOverlay*>(ptr), writer);
    else if (str == "AI")
      Serialize::WriteObject(*static_cast<AI*>(ptr), writer);
    else if (str == "Entity Type")
      Serialize::WriteObject(*static_cast<EntityType*>(ptr), writer);
    else if (str == "Attack")
      Serialize::WriteObject(*static_cast<Attack*>(ptr), writer);
    else if (str == "Health")
      Serialize::WriteObject(*static_cast<Health*>(ptr), writer);
    else if (str == "Lifespan")
      Serialize::WriteObject(*static_cast<Lifespan*>(ptr), writer);
    else if (str == "Player")
      Serialize::WriteObject(*static_cast<Player*>(ptr), writer);
    else if (str == "Camera")
      Serialize::WriteObject(*static_cast<Camera*>(ptr), writer);
    else if (str == "GUI")
      Serialize::WriteObject(*static_cast<GUI*>(ptr), writer);
    else if (str == "Button")
      Serialize::WriteObject(*static_cast<Button*>(ptr), writer);
    else if (str == "Toggle")
      Serialize::WriteObject(*static_cast<Toggle*>(ptr), writer);
    else if (str == "Slider")
      Serialize::WriteObject(*static_cast<Slider*>(ptr), writer);
    else if (str == "Audio Source" )
      Serialize::WriteObject(*static_cast<AudioSource*>(ptr), writer);
  }

  void DeserializeType(const std::string& str, EntityManager& em, EntityID id, rttr::variant var, size_t p_adj)
  {
    if (str == "Entity Name")
      em.GetComponent<EntityName>(id) = var.get_value<EntityName>();
    else if (str == "Parent")
      em.AddComponent<Parent>( id, { var.get_value<Parent>().p_id + p_adj } );
    else if (str == "Transform")
      em.GetComponent<Transform>(id) = var.get_value<Transform>();
    else if (str == "Entity Type")
      em.GetComponent<EntityType>(id) = var.get_value<EntityType>();
    else if (str == "Collider")
      em.AddComponent<Collider>(id, var.get_value<Collider>());
    else if (str == "Rigidbody")
      em.AddComponent<RigidBody>(id, var.get_value<RigidBody>());
    else if (str == "Input")
      em.AddComponent<Input>(id, var.get_value<Input>());
    else if (str == "Animator")
      em.AddComponent<Animator>(id, var.get_value<Animator>());
    else if (str == "State")
      em.AddComponent<State>(id);
    else if (str == "Image")
      em.AddComponent<Image>(id, var.get_value<Image>());
    else if (str == "Text")
      em.AddComponent<Text>(id, var.get_value<Text>());
    else if (str == "ParticleEmitter")
      em.AddComponent<ParticleEmitter>(id, var.get_value<ParticleEmitter>());
    else if (str == "Renderer2D")
      em.AddComponent<Renderer2D>(id, var.get_value<Renderer2D>());
    else if (str == "RendererOverlay")
      em.AddComponent<RendererOverlay>(id, var.get_value<RendererOverlay>());
    else if (str == "AI")
      em.AddComponent<AI>(id, var.get_value<AI>());
    else if (str == "Attack")
      em.AddComponent<Attack>(id, var.get_value<Attack>());
    else if (str == "Health")
      em.AddComponent<Health>(id, var.get_value<Health>());
    else if (str == "Lifespan")
      em.AddComponent<Lifespan>(id, var.get_value<Lifespan>());
    else if (str == "Player")
      em.AddComponent<Player>(id, var.get_value<Player>());
    else if (str == "Camera")
      em.AddComponent<Camera>(id, var.get_value<Camera>());
    else if (str == "GUI")
      em.AddComponent<GUI>(id, var.get_value<GUI>());
    else if (str == "Button")
      em.AddComponent<Button>(id, var.get_value<Button>());
    else if (str == "Toggle")
      em.AddComponent<Toggle>(id, var.get_value<Toggle>());
    else if (str == "Slider")
      em.AddComponent<Slider>(id, var.get_value<Slider>());
    else if (str == "Audio Source")
      em.AddComponent<AudioSource>(id, var.get_value<AudioSource>());
  }
}
