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

		/**
		 * Add the given word, in full, to the tree.  False is returned here if it is already in the tree.
		 */
		bool AddWord(const char* word);

		/**
		 * Tell us if the given word is a prefix (or suffix) of any word in the tree.
		 */
		bool ContainsWord(const char* word, const Node** lastNode = nullptr) const;

		/**
		 * Remove all words from this tree, making it an empy tree.
		 */
		void Clear();

		/**
		 * Return the number of words in the tree.
		 */
		unsigned int GetNumWords() const;

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
		unsigned int numWords;
	};
}