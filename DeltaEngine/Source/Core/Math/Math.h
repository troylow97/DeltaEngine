#pragma once

namespace DeltaEngine::Math
{

const float pi = 3.14159265358979323846f;

template <typename T>
const T &math_max( const T &a, const T &b )
{
  return ( a < b ) ? b : a;     // or: return comp(a,b)?b:a; for version (2)
}

template <typename T>
const T &math_min( const T &a, const T &b )
{
  return !( b < a ) ? a : b;     // or: return !comp(b,a)?a:b; for version (2)
}

int Clamp( int value, int min, int max );
float Clamp( float value, float min, float max );
float Clamp01( float value );
float Lerp( float a, float b, float t );
int RoundDown( float value );
int RoundDownToNearest( float value, int multiple );
float RoundDownf( float value );
float RoundDownToNearestf( float value, int multiple );

}
