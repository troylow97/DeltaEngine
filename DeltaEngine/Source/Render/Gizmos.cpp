#include "Gizmos.h"
#include <GL/glew.h>

namespace DeltaEngine
{
	namespace Gizmos
	{


		void Init()
		{


		}
		void DrawWorldGrid()
		{

		}

		void Draw2DBox(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			Draw2DBox(Transform(position, rotation, scale));
		}
		void Draw2DBox(Transform transform)
		{

		}

		void Draw2DBoxWire(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			Draw2DBoxWire(Transform(position, rotation, scale));
		}
		void Draw2DBoxWire(Transform transform)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		}

		void DrawCube(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			DrawCube(Transform(position, rotation, scale));
		}
		void DrawCube(Transform transform)
		{

		}

		void DrawCubeWire(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			DrawCube(Transform(position, rotation, scale));
		}
		void DrawCubeWire(Transform transform)
		{

		}

		void DrawSphere(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			DrawSphere(Transform(position, rotation, scale));
		}
		void DrawSphere(Transform transform)
		{

		}

		void DrawSphereWire(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			DrawSphereWire(Transform(position, rotation, scale));
		}
		void DrawSphereWire(Transform transform)
		{

		}
	};
}
