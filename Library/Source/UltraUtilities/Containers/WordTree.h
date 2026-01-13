#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Containers/List.hpp"
#include "UltraUtilities/String.h"

namespace UU
{
	/**
	 * 
	 */
	class UU_API WordTree
	{
	public:
		enum Mode
		{
			PrefixTree,
			SuffixTree
		};

		class Node;
		
		WordTree(Mode mode);
		virtual ~WordTree();

		bool AddWord(const char* word);

		bool ContainsWord(const char* word, const Node** lastNode = nullptr) const;

		/**
		 * If in prefix mode, return all strings in this word tree with the given prefix.
		 * If in suffix mode, return all strings in this word tree with the given suffix.
		 * 
		 * @param[in] word This is the given prefix or suffix, depending on the mode of the tree.
		 * @param[out] completedWordArray This will get populated with all strings having the given prefix (or suffix).
		 */
		void GetAllWordCompletions(const char* word, DArray<String>& completedWordArray) const;

		class Node
		{
		public:
			Node();
			virtual ~Node();

			void ExploreAll(List<char>& charList, DArray<String>& completedWordArray, Mode mode) const;

			Node* childNodeArray[256];
		};

	private:
		Mode mode;

		struct WordTraversalParams
		{
			int delta;
			int start;
			int stop;
		};

		bool MakeWordTraversalParams(const char* word, WordTraversalParams& params) const;

		Node* rootNode;
	};
}