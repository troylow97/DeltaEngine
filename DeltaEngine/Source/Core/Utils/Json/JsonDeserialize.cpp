#define RAPIDJSON_HAS_STDSTRING 1

#include "JsonDeserialize.h"
#include "ECS/EntityManager.h" // Loading Entities
#include "Reflect/Reflect.h" // Reflecting Components

using namespace rapidjson;
using namespace rttr;

namespace DeltaEngine::Deserialize
{

/////////////////////////////////////////////////////////////////////////////////////////

// Forward Declaration
void ReadRecursive( instance obj, Value &json_object );
variant extract_value( Value::MemberIterator &itr, const type &t );
variant ExtractBasicType( Value &json_value );
void ExtractPointerType( variant& obj_dat, variant& extracted );

/////////////////////////////////////////////////////////////////////////////////////////

void ReadObject( instance obj, Document &doc )
{
  ReadRecursive( obj, doc );
}

/////////////////////////////////////////////////////////////////////////////////////////

void ReadArray( variant_sequential_view &view, Value &json_array_value )
{
  view.set_size( json_array_value.Size() );
  for ( SizeType i = 0; i < json_array_value.Size(); ++i )
  {
    auto &json_index_value = json_array_value[i];
    if ( json_index_value.IsArray() )
    {
      auto sub_array_view = view.get_value( i ).create_sequential_view();
      ReadArray( sub_array_view, json_index_value );
    }
    else if ( json_index_value.IsObject() )
    {
      variant var_tmp = view.get_value( i );
      variant wrapped_var = var_tmp.extract_wrapped_value();
      ReadRecursive( wrapped_var, json_index_value );
      view.set_value( i, wrapped_var );
    }
    else
    {
      const type array_type = view.get_rank_type( i );
      variant extracted_value = ExtractBasicType( json_index_value );
      if ( extracted_value.convert( array_type ) )
        view.set_value( i, extracted_value );
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

void ReadAssociative( variant_associative_view &view, Value &json_array_value )
{
  for ( SizeType i = 0; i < json_array_value.Size(); ++i )
  {
    auto &json_index_value = json_array_value[i];
    if ( json_index_value.IsObject() ) // a key-value associative view
    {
      Value::MemberIterator key_itr = json_index_value.FindMember( "key" );
      Value::MemberIterator value_itr = json_index_value.FindMember( "value" );
      if ( key_itr != json_index_value.MemberEnd() &&
           value_itr != json_index_value.MemberEnd() )
      {
        auto key_var = extract_value( key_itr, view.get_key_type() );
        auto value_var = extract_value( value_itr, view.get_value_type() );
        if ( key_var && value_var )
          view.insert( key_var, value_var );
      }
    }
    else // a key-only associative view
    {
      variant extracted_value = ExtractBasicType( json_index_value );
      if ( extracted_value && extracted_value.convert( view.get_key_type() ) )
        view.insert( extracted_value );
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

void ReadEntities( EntityManager &em, Value::MemberIterator it )
{
  for ( Value::ValueIterator itr = it->value.Begin(); itr != it->value.End(); itr++ )
  {
    EntityID id = em.CreateEntity();
#pragma push_macro("GetObject")
#undef GetObject
    auto ref = ( itr->GetObject() );
#pragma pop_macro("GetObject")

    for ( Value::MemberIterator mitr = itr->MemberBegin(); mitr != itr->MemberEnd(); mitr++ )
    {
      rttr::type type = rttr::type::get_by_name( mitr->name.GetString() );
      variant obj = type.get_constructor().invoke();
      auto &value = mitr->value;
      ReadRecursive( obj, value );
      RT_Reflect::DeserializeType( type.get_name().to_string(), em, id, obj );
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

void ReadRecursive( instance obj2, Value &json_object )
{
  instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
  const auto prop_list = obj.get_derived_type().get_properties();
  for ( auto prop : prop_list )
  {
    Value::MemberIterator ret = json_object.FindMember( prop.get_name().data() );
    if ( ret == json_object.MemberEnd() )
      continue;
    const type value_t = prop.get_type();

    auto &json_value = ret->value;
    switch ( json_value.GetType() )
    {
      case kArrayType:
      {
        variant var;
        if ( value_t.is_sequential_container() )
        {
          var = prop.get_value( obj );
          auto view = var.create_sequential_view();
          ReadArray( view, json_value );
        }
        else if ( value_t.is_associative_container() )
        {
          var = prop.get_value( obj );
          auto associative_view = var.create_associative_view();
          ReadAssociative( associative_view, json_value );
        }

        prop.set_value( obj, var );
        break;
      }
      case kObjectType:
      {
        variant var = prop.get_value( obj );
        ReadRecursive( var, json_value );
        prop.set_value( obj, var );
        break;
      }
      default:
      {
        variant extracted_value = ExtractBasicType( json_value );

        if ( extracted_value.convert( value_t ) )
          prop.set_value( obj, extracted_value );
        else if ( value_t.is_pointer() )
          ExtractPointerType( prop.get_value( obj ), extracted_value );
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

variant extract_value( Value::MemberIterator &itr, const type &t )
{
  auto &json_value = itr->value;
  variant extracted_value = ExtractBasicType( json_value );
  const bool could_convert = extracted_value.convert( t );
  if ( !could_convert )
  {
    if ( json_value.IsObject() )
    {
      constructor ctor = t.get_constructor();
      for ( auto &item : t.get_constructors() )
        if ( item.get_instantiated_type() == t )
          ctor = item;
      extracted_value = ctor.invoke();
      ReadRecursive( extracted_value, json_value );
    }
  }

  return extracted_value;
}

/////////////////////////////////////////////////////////////////////////////////////////

variant ExtractBasicType( Value &json_value )
{
  switch ( json_value.GetType() )
  {
    case kStringType:
      return std::string( json_value.GetString() );

    case kNullType:
      break;

    case kFalseType:
    case kTrueType:
      return json_value.GetBool();

    case kNumberType:
    {
      if ( json_value.IsInt() )
        return json_value.GetInt();
      if ( json_value.IsDouble() )
        return json_value.GetDouble();
      if ( json_value.IsUint() )
        return json_value.GetUint();
      if ( json_value.IsInt64() )
        return json_value.GetInt64();
      if ( json_value.IsUint64() )
        return json_value.GetUint64();
      break;
    }

    // we handle only the basic types here
    case kObjectType:
    case kArrayType:
      return variant();
  }
  return variant();
}

/////////////////////////////////////////////////////////////////////////////////////////

void ExtractPointerType( variant& obj_dat, variant& extracted )
{
  if (obj_dat.get_type() == type::get<bool*>())
    *obj_dat.get_value<bool *>() =  extracted.get_value<bool>();
  else if (obj_dat.get_type() == type::get<float*>())
    *obj_dat.get_value<float *>() =  static_cast<float>(extracted.get_value<double>());
}


}