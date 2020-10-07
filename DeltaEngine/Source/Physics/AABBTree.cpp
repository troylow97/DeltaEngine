//#include "Physics/AABBTree.h"
//#define AABB_Extension 5
//#define AABB_Mul 5.0f
//namespace DeltaEngine
//{
//	DynamicAABBTree::DynamicAABBTree()
//	{
//		m_root = NULL_NODE;
//
//		m_nodeCapacity = 16;
//		m_nodeCount = 0;
//		m_nodes = (Node*)malloc(m_nodeCapacity * sizeof(Node));
//		memset(m_nodes, 0, m_nodeCapacity * sizeof(Node));
//
//		// Build a linked list for the free list.
//		for (i32 i = 0; i < m_nodeCapacity - 1; ++i)
//		{
//			m_nodes[i].next = i + 1;
//			m_nodes[i].height = -1;
//		}
//		m_nodes[m_nodeCapacity - 1].next = NULL_NODE;
//		m_nodes[m_nodeCapacity - 1].height = -1;
//		m_freeList = 0;
//
//		m_insertionCount = 0;
//	}
//
//	DynamicAABBTree::~DynamicAABBTree()
//	{
//		// This frees the entire tree in one shot.
//		free(m_nodes);
//	}
//
//	// Allocate a node from the pool. Grow the pool if necessary.
//	i32 DynamicAABBTree::AllocateNode()
//	{
//		// Expand the node pool as needed.
//		if (m_freeList == NULL_NODE)
//		{
//			assert(m_nodeCount == m_nodeCapacity);
//
//			// The free list is empty. Rebuild a bigger pool.
//			Node* oldNodes = m_nodes;
//			m_nodeCapacity *= 2;
//			m_nodes = (Node*)malloc(m_nodeCapacity * sizeof(Node));
//			memcpy(m_nodes, oldNodes, m_nodeCount * sizeof(Node));
//			free(oldNodes);
//
//			// Build a linked list for the free list. The parent
//			// pointer becomes the "next" pointer.
//			for (i32 i = m_nodeCount; i < m_nodeCapacity - 1; ++i)
//			{
//				m_nodes[i].next = i + 1;
//				m_nodes[i].height = -1;
//			}
//			m_nodes[m_nodeCapacity - 1].next = NULL_NODE;
//			m_nodes[m_nodeCapacity - 1].height = -1;
//			m_freeList = m_nodeCount;
//		}
//
//		// Peel a node off the free list.
//		i32 nodeId = m_freeList;
//		m_freeList = m_nodes[nodeId].next;
//		m_nodes[nodeId].parent = NULL_NODE;
//		m_nodes[nodeId].child1 = NULL_NODE;
//		m_nodes[nodeId].child2 = NULL_NODE;
//		m_nodes[nodeId].height = 0;
//		m_nodes[nodeId].userData = nullptr;
//		m_nodes[nodeId].moved = false;
//		++m_nodeCount;
//		return nodeId;
//	}
//
//	// Return a node to the pool.
//	void DynamicAABBTree::FreeNode(i32 nodeId)
//	{
//		assert(0 <= nodeId && nodeId < m_nodeCapacity);
//		assert(0 < m_nodeCount);
//		m_nodes[nodeId].next = m_freeList;
//		m_nodes[nodeId].height = -1;
//		m_freeList = nodeId;
//		--m_nodeCount;
//	}
//
//	// Create a proxy in the tree as a leaf node. We return the index
//	// of the node instead of a pointer so that we can grow
//	// the node pool.
//	i32 DynamicAABBTree::CreateProxy(const AABB& aabb, void* userData)
//	{
//		i32 proxyId = AllocateNode();
//
//		// Fatten the aabb.
//		Vector2 r(AABB_Extension, AABB_Extension);
//		m_nodes[proxyId].aabb.min = aabb.min - r;
//		m_nodes[proxyId].aabb.max = aabb.max + r;
//		m_nodes[proxyId].userData = userData;
//		m_nodes[proxyId].height = 0;
//		m_nodes[proxyId].moved = true;
//
//		InsertLeaf(proxyId);
//
//		return proxyId;
//	}
//
//	void DynamicAABBTree::DestroyProxy(i32 proxyId)
//	{
//		assert(0 <= proxyId && proxyId < m_nodeCapacity);
//		assert(m_nodes[proxyId].IsLeaf());
//
//		RemoveLeaf(proxyId);
//		FreeNode(proxyId);
//	}
//
//	bool DynamicAABBTree::MoveProxy(i32 proxyId, const AABB& aabb, const Vector2& displacement)
//	{
//		assert(0 <= proxyId && proxyId < m_nodeCapacity);
//
//		assert(m_nodes[proxyId].IsLeaf());
//
//		// Extend AABB
//		AABB fatAABB;
//		Vector2 r(AABB_Extension, AABB_Extension);
//		fatAABB.min = aabb.min - r;
//		fatAABB.max = aabb.max + r;
//
//		// Predict AABB movement
//		Vector2 d = displacement * AABB_Mul;
//
//		if (d.x < 0.0f)
//		{
//			fatAABB.min.x += d.x;
//		}
//		else
//		{
//			fatAABB.max.x += d.x;
//		}
//
//		if (d.y < 0.0f)
//		{
//			fatAABB.min.y += d.y;
//		}
//		else
//		{
//			fatAABB.max.y += d.y;
//		}
//
//		const AABB& treeAABB = m_nodes[proxyId].aabb;
//		if (treeAABB.contains(aabb))
//		{
//			// The tree AABB still contains the object, but it might be too large.
//			// Perhaps the object was moving fast but has since gone to sleep.
//			// The huge AABB is larger than the new fat AABB.
//			AABB hugeAABB;
//			hugeAABB.min = fatAABB.min - r * 4.0f;
//			hugeAABB.max = fatAABB.max + r * 4.0f;
//
//			if (hugeAABB.contains(treeAABB))
//			{
//				// The tree AABB contains the object AABB and the tree AABB is
//				// not too large. No tree update needed.
//				return false;
//			}
//
//			// Otherwise the tree AABB is huge and needs to be shrunk
//		}
//
//		RemoveLeaf(proxyId);
//
//		m_nodes[proxyId].aabb = fatAABB;
//
//		InsertLeaf(proxyId);
//
//		m_nodes[proxyId].moved = true;
//
//		return true;
//	}
//
//	void DynamicAABBTree::InsertLeaf(i32 leaf)
//	{
//		++m_insertionCount;
//
//		if (m_root == NULL_NODE)
//		{
//			m_root = leaf;
//			m_nodes[m_root].parent = NULL_NODE;
//			return;
//		}
//
//		// Find the best sibling for this node
//		AABB leafAABB = m_nodes[leaf].aabb;
//		i32 index = m_root;
//		while (m_nodes[index].IsLeaf() == false)
//		{
//			i32 child1 = m_nodes[index].child1;
//			i32 child2 = m_nodes[index].child2;
//
//			float area = m_nodes[index].aabb.GetPerimeter();
//
//			AABB combinedAABB;
//			combinedAABB.combine(m_nodes[index].aabb, leafAABB);
//			float combinedArea = combinedAABB.GetPerimeter();
//
//			// Cost of creating a new parent for this node and the new leaf
//			float cost = 2.0f * combinedArea;
//
//			// Minimum cost of pushing the leaf further down the tree
//			float inheritanceCost = 2.0f * (combinedArea - area);
//
//			// Cost of descending into child1
//			float cost1;
//			if (m_nodes[child1].IsLeaf())
//			{
//				AABB aabb;
//				aabb.combine(leafAABB, m_nodes[child1].aabb);
//				cost1 = aabb.GetPerimeter() + inheritanceCost;
//			}
//			else
//			{
//				AABB aabb;
//				aabb.combine(leafAABB, m_nodes[child1].aabb);
//				float oldArea = m_nodes[child1].aabb.GetPerimeter();
//				float newArea = aabb.GetPerimeter();
//				cost1 = (newArea - oldArea) + inheritanceCost;
//			}
//
//			// Cost of descending into child2
//			float cost2;
//			if (m_nodes[child2].IsLeaf())
//			{
//				AABB aabb;
//				aabb.combine(leafAABB, m_nodes[child2].aabb);
//				cost2 = aabb.GetPerimeter() + inheritanceCost;
//			}
//			else
//			{
//				AABB aabb;
//				aabb.combine(leafAABB, m_nodes[child2].aabb);
//				float oldArea = m_nodes[child2].aabb.GetPerimeter();
//				float newArea = aabb.GetPerimeter();
//				cost2 = newArea - oldArea + inheritanceCost;
//			}
//
//			// Descend according to the minimum cost.
//			if (cost < cost1 && cost < cost2)
//			{
//				break;
//			}
//
//			// Descend
//			if (cost1 < cost2)
//			{
//				index = child1;
//			}
//			else
//			{
//				index = child2;
//			}
//		}
//
//		i32 sibling = index;
//
//		// Create a new parent.
//		i32 oldParent = m_nodes[sibling].parent;
//		i32 newParent = AllocateNode();
//		m_nodes[newParent].parent = oldParent;
//		m_nodes[newParent].userData = nullptr;
//		m_nodes[newParent].aabb.combine(leafAABB, m_nodes[sibling].aabb);
//		m_nodes[newParent].height = m_nodes[sibling].height + 1;
//
//		if (oldParent != NULL_NODE)
//		{
//			// The sibling was not the root.
//			if (m_nodes[oldParent].child1 == sibling)
//			{
//				m_nodes[oldParent].child1 = newParent;
//			}
//			else
//			{
//				m_nodes[oldParent].child2 = newParent;
//			}
//
//			m_nodes[newParent].child1 = sibling;
//			m_nodes[newParent].child2 = leaf;
//			m_nodes[sibling].parent = newParent;
//			m_nodes[leaf].parent = newParent;
//		}
//		else
//		{
//			// The sibling was the root.
//			m_nodes[newParent].child1 = sibling;
//			m_nodes[newParent].child2 = leaf;
//			m_nodes[sibling].parent = newParent;
//			m_nodes[leaf].parent = newParent;
//			m_root = newParent;
//		}
//
//		// Walk back up the tree fixing heights and AABBs
//		index = m_nodes[leaf].parent;
//		while (index != NULL_NODE)
//		{
//			index = Balance(index);
//
//			i32 child1 = m_nodes[index].child1;
//			i32 child2 = m_nodes[index].child2;
//
//			assert(child1 != NULL_NODE);
//			assert(child2 != NULL_NODE);
//
//			m_nodes[index].height = 1 + max(m_nodes[child1].height, m_nodes[child2].height);
//			m_nodes[index].aabb.combine(m_nodes[child1].aabb, m_nodes[child2].aabb);
//
//			index = m_nodes[index].parent;
//		}
//
//		//Validate();
//	}
//
//	void DynamicAABBTree::RemoveLeaf(i32 leaf)
//	{
//		if (leaf == m_root)
//		{
//			m_root = NULL_NODE;
//			return;
//		}
//
//		i32 parent = m_nodes[leaf].parent;
//		i32 grandParent = m_nodes[parent].parent;
//		i32 sibling;
//		if (m_nodes[parent].child1 == leaf)
//		{
//			sibling = m_nodes[parent].child2;
//		}
//		else
//		{
//			sibling = m_nodes[parent].child1;
//		}
//
//		if (grandParent != NULL_NODE)
//		{
//			// Destroy parent and connect sibling to grandParent.
//			if (m_nodes[grandParent].child1 == parent)
//			{
//				m_nodes[grandParent].child1 = sibling;
//			}
//			else
//			{
//				m_nodes[grandParent].child2 = sibling;
//			}
//			m_nodes[sibling].parent = grandParent;
//			FreeNode(parent);
//
//			// Adjust ancestor bounds.
//			i32 index = grandParent;
//			while (index != NULL_NODE)
//			{
//				index = Balance(index);
//
//				i32 child1 = m_nodes[index].child1;
//				i32 child2 = m_nodes[index].child2;
//
//				m_nodes[index].aabb.combine(m_nodes[child1].aabb, m_nodes[child2].aabb);
//				m_nodes[index].height = 1 + max(m_nodes[child1].height, m_nodes[child2].height);
//
//				index = m_nodes[index].parent;
//			}
//		}
//		else
//		{
//			m_root = sibling;
//			m_nodes[sibling].parent = NULL_NODE;
//			FreeNode(parent);
//		}
//
//		//Validate();
//	}
//
//	// Perform a left or right rotation if node A is imbalanced.
//	// Returns the new root index.
//	i32 DynamicAABBTree::Balance(i32 iA)
//	{
//		assert(iA != NULL_NODE);
//
//		Node* A = m_nodes + iA;
//		if (A->IsLeaf() || A->height < 2)
//		{
//			return iA;
//		}
//
//		i32 iB = A->child1;
//		i32 iC = A->child2;
//		assert(0 <= iB && iB < m_nodeCapacity);
//		assert(0 <= iC && iC < m_nodeCapacity);
//
//		Node* B = m_nodes + iB;
//		Node* C = m_nodes + iC;
//
//		i32 balance = C->height - B->height;
//
//		// Rotate C up
//		if (balance > 1)
//		{
//			i32 iF = C->child1;
//			i32 iG = C->child2;
//			Node* F = m_nodes + iF;
//			Node* G = m_nodes + iG;
//			assert(0 <= iF && iF < m_nodeCapacity);
//			assert(0 <= iG && iG < m_nodeCapacity);
//
//			// Swap A and C
//			C->child1 = iA;
//			C->parent = A->parent;
//			A->parent = iC;
//
//			// A's old parent should point to C
//			if (C->parent != NULL_NODE)
//			{
//				if (m_nodes[C->parent].child1 == iA)
//				{
//					m_nodes[C->parent].child1 = iC;
//				}
//				else
//				{
//					assert(m_nodes[C->parent].child2 == iA);
//					m_nodes[C->parent].child2 = iC;
//				}
//			}
//			else
//			{
//				m_root = iC;
//			}
//
//			// Rotate
//			if (F->height > G->height)
//			{
//				C->child2 = iF;
//				A->child2 = iG;
//				G->parent = iA;
//				A->aabb.combine(B->aabb, G->aabb);
//				C->aabb.combine(A->aabb, F->aabb);
//
//				A->height = 1 + max(B->height, G->height);
//				C->height = 1 + max(A->height, F->height);
//			}
//			else
//			{
//				C->child2 = iG;
//				A->child2 = iF;
//				F->parent = iA;
//				A->aabb.combine(B->aabb, F->aabb);
//				C->aabb.combine(A->aabb, G->aabb);
//
//				A->height = 1 + max(B->height, F->height);
//				C->height = 1 + max(A->height, G->height);
//			}
//
//			return iC;
//		}
//
//		// Rotate B up
//		if (balance < -1)
//		{
//			i32 iD = B->child1;
//			i32 iE = B->child2;
//			Node* D = m_nodes + iD;
//			Node* E = m_nodes + iE;
//			assert(0 <= iD && iD < m_nodeCapacity);
//			assert(0 <= iE && iE < m_nodeCapacity);
//
//			// Swap A and B
//			B->child1 = iA;
//			B->parent = A->parent;
//			A->parent = iB;
//
//			// A's old parent should point to B
//			if (B->parent != NULL_NODE)
//			{
//				if (m_nodes[B->parent].child1 == iA)
//				{
//					m_nodes[B->parent].child1 = iB;
//				}
//				else
//				{
//					assert(m_nodes[B->parent].child2 == iA);
//					m_nodes[B->parent].child2 = iB;
//				}
//			}
//			else
//			{
//				m_root = iB;
//			}
//
//			// Rotate
//			if (D->height > E->height)
//			{
//				B->child2 = iD;
//				A->child1 = iE;
//				E->parent = iA;
//				A->aabb.combine(C->aabb, E->aabb);
//				B->aabb.combine(A->aabb, D->aabb);
//
//				A->height = 1 + max(C->height, E->height);
//				B->height = 1 + max(A->height, D->height);
//			}
//			else
//			{
//				B->child2 = iE;
//				A->child1 = iD;
//				D->parent = iA;
//				A->aabb.combine(C->aabb, D->aabb);
//				B->aabb.combine(A->aabb, E->aabb);
//
//				A->height = 1 + max(C->height, D->height);
//				B->height = 1 + max(A->height, E->height);
//			}
//
//			return iB;
//		}
//
//		return iA;
//	}
//
//	i32 DynamicAABBTree::GetHeight() const
//	{
//		if (m_root == NULL_NODE)
//		{
//			return 0;
//		}
//
//		return m_nodes[m_root].height;
//	}
//
//	//
//	float DynamicAABBTree::GetAreaRatio() const
//	{
//		if (m_root == NULL_NODE)
//		{
//			return 0.0f;
//		}
//
//		const Node* root = m_nodes + m_root;
//		float rootArea = root->aabb.GetPerimeter();
//
//		float totalArea = 0.0f;
//		for (i32 i = 0; i < m_nodeCapacity; ++i)
//		{
//			const Node* node = m_nodes + i;
//			if (node->height < 0)
//			{
//				// Free node in pool
//				continue;
//			}
//
//			totalArea += node->aabb.GetPerimeter();
//		}
//
//		return totalArea / rootArea;
//	}
//
//	// Compute the height of a sub-tree.
//	i32 DynamicAABBTree::ComputeHeight(i32 nodeId) const
//	{
//		assert(0 <= nodeId && nodeId < m_nodeCapacity);
//		Node* node = m_nodes + nodeId;
//
//		if (node->IsLeaf())
//		{
//			return 0;
//		}
//
//		i32 height1 = ComputeHeight(node->child1);
//		i32 height2 = ComputeHeight(node->child2);
//		return 1 + max(height1, height2);
//	}
//
//	i32 DynamicAABBTree::ComputeHeight() const
//	{
//		i32 height = ComputeHeight(m_root);
//		return height;
//	}
//
//	void DynamicAABBTree::ValidateStructure(i32 index) const
//	{
//		if (index == NULL_NODE)
//		{
//			return;
//		}
//
//		if (index == m_root)
//		{
//			assert(m_nodes[index].parent == NULL_NODE);
//		}
//
//		const Node* node = m_nodes + index;
//
//		i32 child1 = node->child1;
//		i32 child2 = node->child2;
//
//		if (node->IsLeaf())
//		{
//			assert(child1 == NULL_NODE);
//			assert(child2 == NULL_NODE);
//			assert(node->height == 0);
//			return;
//		}
//
//		assert(0 <= child1 && child1 < m_nodeCapacity);
//		assert(0 <= child2 && child2 < m_nodeCapacity);
//
//		assert(m_nodes[child1].parent == index);
//		assert(m_nodes[child2].parent == index);
//
//		ValidateStructure(child1);
//		ValidateStructure(child2);
//	}
//
//	void DynamicAABBTree::ValidateMetrics(i32 index) const
//	{
//		if (index == NULL_NODE)
//		{
//			return;
//		}
//
//		const Node* node = m_nodes + index;
//
//		i32 child1 = node->child1;
//		i32 child2 = node->child2;
//
//		if (node->IsLeaf())
//		{
//			assert(child1 == NULL_NODE);
//			assert(child2 == NULL_NODE);
//			assert(node->height == 0);
//			return;
//		}
//
//		assert(0 <= child1 && child1 < m_nodeCapacity);
//		assert(0 <= child2 && child2 < m_nodeCapacity);
//
//		i32 height1 = m_nodes[child1].height;
//		i32 height2 = m_nodes[child2].height;
//		i32 height;
//		height = 1 + max(height1, height2);
//		assert(node->height == height);
//
//		AABB aabb;
//		aabb.combine(m_nodes[child1].aabb, m_nodes[child2].aabb);
//
//		//assert(aabb.min == node->aabb.min);
//		//assert(aabb.max == node->aabb.max);
//
//		ValidateMetrics(child1);
//		ValidateMetrics(child2);
//	}
//
//	void DynamicAABBTree::Validate() const
//	{
//#if defined(b2DEBUG)
//		ValidateStructure(m_root);
//		ValidateMetrics(m_root);
//
//		i32 freeCount = 0;
//		i32 freeIndex = m_freeList;
//		while (freeIndex != NULL_NODE)
//		{
//			assert(0 <= freeIndex && freeIndex < m_nodeCapacity);
//			freeIndex = m_nodes[freeIndex].next;
//			++freeCount;
//		}
//
//		assert(GetHeight() == ComputeHeight());
//
//		assert(m_nodeCount + freeCount == m_nodeCapacity);
//#endif
//	}
//
//	i32 DynamicAABBTree::GetMaxBalance() const
//	{
//		i32 maxBalance = 0;
//		for (i32 i = 0; i < m_nodeCapacity; ++i)
//		{
//			const Node* node = m_nodes + i;
//			if (node->height <= 1)
//			{
//				continue;
//			}
//
//			assert(node->IsLeaf() == false);
//
//			i32 child1 = node->child1;
//			i32 child2 = node->child2;
//			i32 balance = abs(m_nodes[child2].height - m_nodes[child1].height);
//			maxBalance = max(maxBalance, balance);
//		}
//
//		return maxBalance;
//	}
//
//	void DynamicAABBTree::RebuildBottomUp()
//	{
//		i32* nodes = (i32*)malloc(m_nodeCount * sizeof(i32));
//		i32 count = 0;
//
//		// Build array of leaves. Free the rest.
//		for (i32 i = 0; i < m_nodeCapacity; ++i)
//		{
//			if (m_nodes[i].height < 0)
//			{
//				// free node in pool
//				continue;
//			}
//
//			if (m_nodes[i].IsLeaf())
//			{
//				m_nodes[i].parent = NULL_NODE;
//				nodes[count] = i;
//				++count;
//			}
//			else
//			{
//				FreeNode(i);
//			}
//		}
//
//		while (count > 1)
//		{
//			float minCost = 100000000;
//			i32 iMin = -1, jMin = -1;
//			for (i32 i = 0; i < count; ++i)
//			{
//				AABB aabbi = m_nodes[nodes[i]].aabb;
//
//				for (i32 j = i + 1; j < count; ++j)
//				{
//					AABB aabbj = m_nodes[nodes[j]].aabb;
//					AABB b;
//					b.combine(aabbi, aabbj);
//					float cost = b.GetPerimeter();
//					if (cost < minCost)
//					{
//						iMin = i;
//						jMin = j;
//						minCost = cost;
//					}
//				}
//			}
//
//			i32 index1 = nodes[iMin];
//			i32 index2 = nodes[jMin];
//			Node* child1 = m_nodes + index1;
//			Node* child2 = m_nodes + index2;
//
//			i32 parentIndex = AllocateNode();
//			Node* parent = m_nodes + parentIndex;
//			parent->child1 = index1;
//			parent->child2 = index2;
//			parent->height = 1 + max(child1->height, child2->height);
//			parent->aabb.combine(child1->aabb, child2->aabb);
//			parent->parent = NULL_NODE;
//
//			child1->parent = parentIndex;
//			child2->parent = parentIndex;
//
//			nodes[jMin] = nodes[count - 1];
//			nodes[iMin] = parentIndex;
//			--count;
//		}
//
//		m_root = nodes[0];
//		free(nodes);
//
//		Validate();
//	}
//
//	void DynamicAABBTree::ShiftOrigin(const Vector2& newOrigin)
//	{
//		// Build array of leaves. Free the rest.
//		for (i32 i = 0; i < m_nodeCapacity; ++i)
//		{
//			m_nodes[i].aabb.min -= newOrigin;
//			m_nodes[i].aabb.max -= newOrigin;
//		}
//	}
//
//}
