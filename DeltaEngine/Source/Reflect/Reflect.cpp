#pragma once

#include "Reflect.h"

#include "EngineConfig.h"

#include "Core/Math/DE_Math.h"
#include "ECS/ComponentMeta.h"
#include "ECS/EntityManager.h"

#include <rttr/registration>


#include "../../../Sandbox/Source/Systems/RespawnSystem.h"
#include "Assets/AssetKey.h"
#include "Core/Utils/Json/JsonSerialize.h"
#include "Core/GlobalStruct.h"
#include "AI/Waypoint.h"
#include "AI/AI_State.h"
#include "../../Sandbox/Source/Systems/EnemySpawner/EnemySpawner.h"
#include "../../Sandbox/Source/Systems/EnemySpawner/EnemyData.h"

namespace DeltaEngine
{
  RTTR_REGISTRATION
  {
    rttr::registration::class_<Waypoint>("Waypoint")
      .property("Waypoints", &Waypoint::Waypoints)
      .property("CurrentWaypoint", &Waypoint::CurrentWaypoint);

  rttr::registration::class_<EnemyData>("EnemyData")
      .property("health", &EnemyData::Health)
      .property("movespeed", &EnemyData::Movespeed)
      .property("mass", &EnemyData::Mass)
      .property("damage", &EnemyData::Damage);

  rttr::registration::class_<LancerAIData>("LancerAIData")
      .property("charge_detection_range", &LancerAIData::ChargeDetectionRange);

  rttr::registration::class_<FiddlerAIData>("FiddlerAIData")
      .property("waypoint", &FiddlerAIData::waypoint)
      .property("lost_detection_range", &FiddlerAIData::ChargeDetectionRange)
      .property("charge_detection_range", &FiddlerAIData::LostDetectionRange);

  rttr::registration::class_<SerpentipedeAIData>("SerpentipedeAIData")
      .property("cooldown", &SerpentipedeAIData::MaxCooldown)
      .property("points", &SerpentipedeAIData::Points)
      .property("detection_range", &SerpentipedeAIData::DetectionRange);

  rttr::registration::class_<EnemyWave>("EnemyWave")
      .property("enemy_count", &EnemyWave::EnemyCount)
      .property("enemy_type", &EnemyWave::EnemyType)
      .property("spawn_area", &EnemyWave::SpawnArea);

  rttr::registration::class_<Gauntlet>("Gauntlet")
      .property("enemy_waves", &Gauntlet::EnemyWaves)
      .property("activation_point", &Gauntlet::ActivationPoint)
      .property("current_enemy_wave", &Gauntlet::CurrentEnemyWave)(rttr::metadata("NO_SERIALIZE", true))
      .property("is_activated", &Gauntlet::isActivated)(rttr::metadata("NO_SERIALIZE", true))
      .property("is_finished", &Gauntlet::isFinished)(rttr::metadata("NO_SERIALIZE", true));

  rttr::registration::class_<GauntletsList>("Gauntlets")
      .property("gauntlets", &GauntletsList::Gauntlets);


  rttr::registration::class_<RespawnPoints>("RespawnPoints")
      .property("respawn_points", &RespawnPoints::m_respawns);

    rttr::registration::class_<EngineConfig>("Config")
      .property("window", &EngineConfig::win_name)
      .property("width", &EngineConfig::width)
      .property("height", &EngineConfig::height)
      .property("fps", &EngineConfig::fps)
      .property("fullscreen", &EngineConfig::fullscreen);

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

    rttr::registration::class_<Point>("Point")
      .property("x", &Point::point_x)
      .property("y", &Point::point_y);

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
      rttr::value("enemy", EntityCategory::E_ENEMY),
      rttr::value("player_bullet", EntityCategory::E_PLAYER_BULLET),
      rttr::value("player_punch", EntityCategory::E_PLAYER_PUNCH),
      rttr::value("charge", EntityCategory::E_LANCER_CHARGE)
    );

    rttr::registration::enumeration<Alignment>("Alignment")
    (
      rttr::value("align_left", Alignment::AlignLeft),
      rttr::value("align_right", Alignment::AlignRight),
      rttr::value("center", Alignment::Centralize)
    );

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

    rttr::registration::class_<EntityName>("Entity Name")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityName>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Name", &EntityName::name)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Parent>("Parent")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Parent>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("ID", &Parent::p_id)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Transform>("Transform")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Transform>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Old Position", &Transform::old_position)(rttr::metadata("NO_SERIALIZE", true),
                                                           (rttr::metadata("NO_EDITOR", true)))
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
      .property("Moveable", &RigidBody::isMoveable)(rttr::policy::prop::bind_as_ptr)
      .property("Jumping", &RigidBody::isJumping)(rttr::policy::prop::bind_as_ptr)(
        rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));

    rttr::registration::class_<Collider>("Collider")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Collider>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Center", &Collider::center)(rttr::policy::prop::bind_as_ptr)(rttr::metadata("NO_SERIALIZE", true),(rttr::metadata("NO_EDITOR", true)))
      .property("Offset", &Collider::offset)(rttr::policy::prop::bind_as_ptr)
      .property("Size", &Collider::size)(rttr::policy::prop::bind_as_ptr)
      .property("Intersection Point", &Collider::interPoint)(rttr::metadata("NO_SERIALIZE", true),
                                                     (rttr::metadata("NO_EDITOR", true)))
      .property("Type", &Collider::type)
      .property("Trigger", &Collider::isTrigger)(rttr::policy::prop::bind_as_ptr)
      .property("Colliding On Floor", &Collider::isCollidingOnFloor)(rttr::metadata("NO_SERIALIZE", true),
                                                                        (rttr::metadata("NO_EDITOR", true)))
      .property("Collision Layer", &Collider::CollisionLayerCheck)(rttr::policy::prop::bind_as_ptr)
        .property("Collision Layer ID", &Collider::CollisionLayerID)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Animator>("Animator")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Animator>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Controller Key", &Animator::m_ControllerKey)(rttr::policy::prop::bind_as_ptr)
      .property("Clip Key", &Animator::m_ClipKey)(rttr::metadata("NO_SERIALIZE", true),
                                                  (rttr::metadata("NO_EDITOR", true)))
      .property("Timer", &Animator::m_Timer)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
      .property("Speed", &Animator::m_Speed)(rttr::policy::prop::bind_as_ptr)
      .property("Frame", &Animator::m_Frame)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));

    rttr::registration::class_<State>("State")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<State>()->bits))
      .property("Parameters", &State::parameters)(rttr::metadata("NO_SERIALIZE", true),
                                                  (rttr::metadata("NO_EDITOR", true)));

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

    rttr::registration::class_<Text>("Text")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Text>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Key", &Text::m_FontKey)(rttr::policy::prop::bind_as_ptr)
      .property("Detail", &Text::m_Text)(rttr::policy::prop::bind_as_ptr)
      .property("Alignment", &Text::alignment)(rttr::policy::prop::bind_as_ptr);

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
      .property("Transition", &AI::transition)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<EntityType>("EntityType")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<EntityType>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Category", &EntityType::type)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Health>("Health")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Health>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Current Health", &Health::CurrentHealth)(rttr::policy::prop::bind_as_ptr)
      .property("Max Health", &Health::MaxHealth)(rttr::policy::prop::bind_as_ptr)
      .property("Invulnerable", &Health::isInvulnerable)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Attack>("Attack")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Attack>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Range Damage", &Attack::RangedDamage)(rttr::policy::prop::bind_as_ptr)
      .property("Melee Damage", &Attack::MeleeDamage)(rttr::policy::prop::bind_as_ptr)
      .property("Max Cooldown", &Attack::MaxCooldown)(rttr::policy::prop::bind_as_ptr)
      .property("Cooldown Timer", &Attack::CooldownTimer)(rttr::policy::prop::bind_as_ptr)
      .property("Ranged Attack", &Attack::RangeAttack)(rttr::policy::prop::bind_as_ptr)
      .property("Melee Attack", &Attack::MeleeAttack)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Lifespan>("Lifespan")
      (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Lifespan>()->bits))
      .constructor<>()(rttr::policy::ctor::as_object)
      .property("Timer", &Lifespan::Timer)(rttr::policy::prop::bind_as_ptr);

    rttr::registration::class_<Player>("Player")
        (rttr::metadata("bits", ComponentMeta::GetComponentMeta<Player>()->bits))
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("Respawn Point", &Player::RespawnPoint)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)))
        .property("Is Dead", &Player::IsDead)(rttr::metadata("NO_SERIALIZE", true), (rttr::metadata("NO_EDITOR", true)));
  }

}

namespace DeltaEngine::RT_Reflect
{
  rttr::type RT_Checker(size_t bits)
  {
    if (ComponentMeta::GetComponentMeta<EntityName>()->bits == bits)
      return rttr::type::get_by_name("Entity Name");
    if (ComponentMeta::GetComponentMeta<Parent>()->bits == bits)
      return rttr::type::get_by_name("Parent");
    if (ComponentMeta::GetComponentMeta<Transform>()->bits == bits)
      return rttr::type::get_by_name("Transform");
    if (ComponentMeta::GetComponentMeta<Collider>()->bits == bits)
      return rttr::type::get_by_name("Collider");
    if (ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits)
      return rttr::type::get_by_name("Rigidbody");
    if (ComponentMeta::GetComponentMeta<Input>()->bits == bits)
      return rttr::type::get_by_name("Input");
    if (ComponentMeta::GetComponentMeta<Animator>()->bits == bits)
      return rttr::type::get_by_name("Animator");
    if (ComponentMeta::GetComponentMeta<State>()->bits == bits)
      return rttr::type::get_by_name("State");
    if (ComponentMeta::GetComponentMeta<Image>()->bits == bits)
      return rttr::type::get_by_name("Image");
    if (ComponentMeta::GetComponentMeta<Text>()->bits == bits)
      return rttr::type::get_by_name("Text");
    if (ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits)
      return rttr::type::get_by_name("Renderer2D");
    if (ComponentMeta::GetComponentMeta<AI>()->bits == bits)
      return rttr::type::get_by_name("AI");
    if (ComponentMeta::GetComponentMeta<EntityType>()->bits == bits)
      return rttr::type::get_by_name("EntityType");
    if (ComponentMeta::GetComponentMeta<Attack>()->bits == bits)
      return rttr::type::get_by_name("Attack");
    if (ComponentMeta::GetComponentMeta<Health>()->bits == bits)
      return rttr::type::get_by_name("Health");
    if (ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits)
      return rttr::type::get_by_name("Lifespan");
    if (ComponentMeta::GetComponentMeta<Player>()->bits == bits)
      return rttr::type::get_by_name("Player");
    return rttr::type::get<int>();
  }

  void RT_Destroy(EntityManager& em, EntityID id, size_t bits)
  {
    if (ComponentMeta::GetComponentMeta<EntityName>()->bits == bits)
      em.RemoveComponent<EntityName>(id);
    if (ComponentMeta::GetComponentMeta<Parent>()->bits == bits)
      em.RemoveComponent<Parent>(id);
    if (ComponentMeta::GetComponentMeta<Transform>()->bits == bits)
      em.RemoveComponent<Transform>(id);
    if (ComponentMeta::GetComponentMeta<Collider>()->bits == bits)
      em.RemoveComponent<Collider>(id);
    if (ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits)
      em.RemoveComponent<RigidBody>(id);
    if (ComponentMeta::GetComponentMeta<Input>()->bits == bits)
      em.RemoveComponent<Input>(id);
    if (ComponentMeta::GetComponentMeta<Animator>()->bits == bits)
      em.RemoveComponent<Animator>(id);
    if (ComponentMeta::GetComponentMeta<State>()->bits == bits)
      em.RemoveComponent<State>(id);
    if (ComponentMeta::GetComponentMeta<Image>()->bits == bits)
      em.RemoveComponent<Image>(id);
    if (ComponentMeta::GetComponentMeta<Text>()->bits == bits)
      em.RemoveComponent<Text>(id);
    if (ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits)
      em.RemoveComponent<Renderer2D>(id);
    if (ComponentMeta::GetComponentMeta<AI>()->bits == bits)
      em.RemoveComponent<AI>(id);
    if (ComponentMeta::GetComponentMeta<EntityType>()->bits == bits)
      em.RemoveComponent<EntityType>(id);
    if (ComponentMeta::GetComponentMeta<Attack>()->bits == bits)
      em.RemoveComponent<Attack>(id);
    if (ComponentMeta::GetComponentMeta<Health>()->bits == bits)
      em.RemoveComponent<Health>(id);
    if (ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits)
      em.RemoveComponent<Lifespan>(id);
    if (ComponentMeta::GetComponentMeta<Player>()->bits == bits)
        em.RemoveComponent<Player>(id);
  }

  void RT_Setter(EntityManager& em, EntityID id, size_t bits)
  {
    if (ComponentMeta::GetComponentMeta<EntityName>()->bits == bits)
      em.AddComponent<EntityName>(id);
    if (ComponentMeta::GetComponentMeta<Parent>()->bits == bits)
      em.AddComponent<Parent>(id);
    if (ComponentMeta::GetComponentMeta<Transform>()->bits == bits)
      em.AddComponent<Transform>(id);
    if (ComponentMeta::GetComponentMeta<Collider>()->bits == bits)
      em.AddComponent<Collider>(id);
    if (ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits)
      em.AddComponent<RigidBody>(id);
    if (ComponentMeta::GetComponentMeta<Input>()->bits == bits)
      em.AddComponent<Input>(id);
    if (ComponentMeta::GetComponentMeta<Animator>()->bits == bits)
      em.AddComponent<Animator>(id);
    if (ComponentMeta::GetComponentMeta<State>()->bits == bits)
      em.AddComponent<State>(id);
    if (ComponentMeta::GetComponentMeta<Image>()->bits == bits)
      em.AddComponent<Image>(id);
    if (ComponentMeta::GetComponentMeta<Text>()->bits == bits)
      em.AddComponent<Text>(id);
    if (ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits)
      em.AddComponent<Renderer2D>(id);
    if (ComponentMeta::GetComponentMeta<AI>()->bits == bits)
      em.AddComponent<AI>(id);
    if (ComponentMeta::GetComponentMeta<EntityType>()->bits == bits)
      em.AddComponent<EntityType>(id);
    if (ComponentMeta::GetComponentMeta<Attack>()->bits == bits)
      em.AddComponent<Attack>(id);
    if (ComponentMeta::GetComponentMeta<Health>()->bits == bits)
      em.AddComponent<Health>(id);
    if (ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits)
      em.AddComponent<Lifespan>(id);
    if (ComponentMeta::GetComponentMeta<Player>()->bits == bits)
      em.AddComponent<Player>(id);
  }

  rttr::instance RT_Getter(EntityManager& em, EntityID& id, size_t bits)
  {
    if (ComponentMeta::GetComponentMeta<EntityName>()->bits == bits)
      return rttr::instance(em.GetComponent<EntityName>(id));
    if (ComponentMeta::GetComponentMeta<Parent>()->bits == bits)
      return rttr::instance(em.GetComponent<Parent>(id));
    if (ComponentMeta::GetComponentMeta<Transform>()->bits == bits)
      return rttr::instance(em.GetComponent<Transform>(id));
    if (ComponentMeta::GetComponentMeta<Collider>()->bits == bits)
      return rttr::instance(em.GetComponent<Collider>(id));
    if (ComponentMeta::GetComponentMeta<RigidBody>()->bits == bits)
      return rttr::instance(em.GetComponent<RigidBody>(id));
    if (ComponentMeta::GetComponentMeta<Input>()->bits == bits)
      return rttr::instance(em.GetComponent<Input>(id));
    if (ComponentMeta::GetComponentMeta<Animator>()->bits == bits)
      return rttr::instance(em.GetComponent<Animator>(id));
    if (ComponentMeta::GetComponentMeta<State>()->bits == bits)
      return rttr::instance(em.GetComponent<State>(id));
    if (ComponentMeta::GetComponentMeta<Image>()->bits == bits)
      return rttr::instance(em.GetComponent<Image>(id));
    if (ComponentMeta::GetComponentMeta<Text>()->bits == bits)
      return rttr::instance(em.GetComponent<Text>(id));
    if (ComponentMeta::GetComponentMeta<Renderer2D>()->bits == bits)
      return rttr::instance(em.GetComponent<Renderer2D>(id));
    if (ComponentMeta::GetComponentMeta<AI>()->bits == bits)
      return rttr::instance(em.GetComponent<AI>(id));
    if (ComponentMeta::GetComponentMeta<EntityType>()->bits == bits)
      return rttr::instance(em.GetComponent<EntityType>(id));
    if (ComponentMeta::GetComponentMeta<Attack>()->bits == bits)
      return rttr::instance(em.GetComponent<Attack>(id));
    if (ComponentMeta::GetComponentMeta<Health>()->bits == bits)
      return rttr::instance(em.GetComponent<Health>(id));
    if (ComponentMeta::GetComponentMeta<Lifespan>()->bits == bits)
      return rttr::instance(em.GetComponent<Lifespan>(id));
    if (ComponentMeta::GetComponentMeta<Player>()->bits == bits)
      return rttr::instance(em.GetComponent<Player>(id));
    return rttr::instance();
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
    else if (str == "Renderer2D")
      Serialize::WriteObject(*static_cast<Renderer2D*>(ptr), writer);
    else if (str == "AI")
      Serialize::WriteObject(*static_cast<AI*>(ptr), writer);
    else if (str == "EntityType")
      Serialize::WriteObject(*static_cast<EntityType*>(ptr), writer);
    else if (str == "Attack")
      Serialize::WriteObject(*static_cast<Attack*>(ptr), writer);
    else if (str == "Health")
      Serialize::WriteObject(*static_cast<Health*>(ptr), writer);
    else if (str == "Lifespan")
      Serialize::WriteObject(*static_cast<Lifespan*>(ptr), writer);
    else if (str == "Player")
      Serialize::WriteObject(*static_cast<Player*>(ptr), writer);
  }

  void DeserializeType(const std::string& str, EntityManager& em, EntityID id, rttr::variant var)
  {
    if (str == "Entity Name")
      em.AddComponent<EntityName>(id, var.get_value<EntityName>());
    else if (str == "Parent")
      em.AddComponent<Parent>(id, var.get_value<Parent>());
    else if (str == "Transform")
      em.GetComponent<Transform>(id) = var.get_value<Transform>();
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
    else if (str == "Renderer2D")
      em.AddComponent<Renderer2D>(id, var.get_value<Renderer2D>());
    else if (str == "AI")
      em.AddComponent<AI>(id, var.get_value<AI>());
    else if (str == "EntityType")
      em.AddComponent<EntityType>(id, var.get_value<EntityType>());
    else if (str == "Attack")
      em.AddComponent<Attack>(id, var.get_value<Attack>());
    else if (str == "Health")
      em.AddComponent<Health>(id, var.get_value<Health>());
    else if (str == "Lifespan")
      em.AddComponent<Lifespan>(id, var.get_value<Lifespan>());
    else if (str == "Player")
      em.AddComponent<Player>(id, var.get_value<Player>());
  }
}