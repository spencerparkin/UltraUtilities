#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * These are the elements of the sets within the forest.
	 * A disjoint set forest (or DSF) is a bunch of pair-wise
	 * disjoint sets where we can quickly determine if any two
	 * elements in any two sets are contained within the same
	 * or separate sets, and where we can also quickly take the
	 * union of sets.
	 */
	class UU_API DSFNode
	{
	public:
		DSFNode();
		virtual ~DSFNode();

		/**
		 * Determine whether the two given nodes are members
		 * of the same set.
		 */
		static bool MembersOfSameSet(const DSFNode* nodeA, const DSFNode* nodeB);

		/**
		 * Return the node in the same set as this node that
		 * serves as the set's representative.
		 */
		const DSFNode* FindSetRepresentative() const;

		/**
		 * Form a new set that is the union of the sets containing
		 * the two given nodes.  Failure occurs here if the two given
		 * nodes are already members of the same set.
		 */
		static bool MergeSets(DSFNode* nodeA, DSFNode* nodeB);

	private:
		mutable DSFNode* parentNode;
	};
}