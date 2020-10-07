#include "Collider.h"

namespace DeltaEngine
{
    void OnDrawGizmos(const Collider& col)
    {
        switch (col.type)
        {
        case ColliderType::BOX:
            Gizmos::Draw2DWireBox(col.center, col.size);
            break;
        case ColliderType::CIRCLE:
            Gizmos::Draw2DCircle(col.center, col.size);
            break;
        case ColliderType::RAY:
            Gizmos::Draw2DWireBox(col.center, col.size); //TO CHANGE
            break;
        case ColliderType::LINE:
            Gizmos::Draw2DWireBox(col.center, col.size); //TO CHANGE
            break;
        }
    }
}