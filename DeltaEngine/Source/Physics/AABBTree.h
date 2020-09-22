/*
#pragma once
#include "Collider.h"
#include <memory>
#include <forward_list>
#include <vector>
#include <map>
#define AABB_NULL_NODE 0xffffffff

namespace DeltaEngine
{
    class AABBTree
    {
    private:
        struct AABBNode
        {
            AABB aabb;
            std::shared_ptr<Collider> object;
            // tree links
            unsigned parentNodeIndex;
            unsigned leftNodeIndex;
            unsigned rightNodeIndex;
            // node linked list link
            unsigned nextNodeIndex;

            bool isLeaf() const { return leftNodeIndex == AABB_NULL_NODE; }

            AABBNode() :
                object(nullptr),
                parentNodeIndex(AABB_NULL_NODE),
                leftNodeIndex(AABB_NULL_NODE),
                rightNodeIndex(AABB_NULL_NODE),
                nextNodeIndex(AABB_NULL_NODE)
            {}
        };

        std::map<std::shared_ptr<AABB>, unsigned> _objectNodeIndexMap;
        std::vector<AABBNode> _nodes;
        unsigned _rootNodeIndex;
        unsigned _allocatedNodeCount;
        unsigned _nextFreeNodeIndex;
        unsigned _nodeCapacity;
        unsigned _growthSize;

        unsigned allocateNode();
        void deallocateNode(unsigned nodeIndex);
        void insertLeaf(unsigned leafNodeIndex);
        void removeLeaf(unsigned leafNodeIndex);
        void updateLeaf(unsigned leafNodeIndex, const AABB& newAaab);
        void fixUpwardsTree(unsigned treeNodeIndex);

    public:
        AABBTree(unsigned initialSize);
        ~AABBTree();

        void insertObject(const std::shared_ptr<AABB>& object);
        void removeObject(const std::shared_ptr<AABB>& object);
        void updateObject(const std::shared_ptr<AABB>& object);
        std::forward_list<std::shared_ptr<Collider>> queryOverlaps(const std::shared_ptr<AABB>& object) const;
    };
}
*/