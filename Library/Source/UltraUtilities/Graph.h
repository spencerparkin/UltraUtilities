#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	/**
	 * These are supposed to be general-purpose graphs built by the user.
	 * The nodes and edges are allocated by the user, but once given to
	 * the graph, owned by the graph.  Various graph algorithms are provided,
	 * such as depth-first search, breadth-first search, and Dijkstra's algorithm.
	 */
	class UU_API Graph
	{
	public:
		Graph();
		virtual ~Graph();

		class Node;
		class Edge;
		class NodeVisitor;

		/**
		 * Add a new node to this graph.  Note that it is up to the caller to link this node with others, if desired.
		 * 
		 * @param[in] node This should be a Node instance allocated by the caller.  Note that the graph takes ownership of the memory.
		 */
		void AddNode(Node* node);

		/**
		 * Add a new edge to the graph.  Note that it is up to the caller to link nodes together with this edge, if desired.
		 * 
		 * @param[in] edge This should be an Edge instance allocated by the caller.  Note that the graph takes ownership of the memory.
		 */
		void AddEdge(Edge* edge);

		/**
		 * Delete all nodes and edges of the graph, reclaiming the memory in the process.
		 */
		void Clear();

		/**
		 * Get the node in this graph at the given offset.
		 * Null is returned if the offset is out of bounds.
		 */
		Node* GetNode(unsigned int i);

		/**
		 * Get the edge in this graph at the given offset.
		 * Null is returned if the offset is out of bounds.
		 */
		Edge* GetEdge(unsigned int i);

		/**
		 * Return the number of nodes in this graph.
		 */
		unsigned int GetNumNodes() const;

		/**
		 * Return the number of edges in this graph.
		 */
		unsigned int GetNumEdges() const;

		/**
		 * Perform a depth-first traversal of this graph starting at the given node.
		 * Of course, only the connected component of this graph containing the
		 * given node is traversed.  Note that if the given node is not a member of this
		 * graph, then we leave the behavior of this function as undefined.
		 * 
		 * @param[in] node The traversal begins at this node, which is presumed to be a member of this graph.
		 * @param[in] visitor The @ref VisitNode method of this class is called for each node encountered.
		 * @return True is returned if all possible nodes are visited without early termination, which can occur if the @ref VisitNode method returns false.
		 */
		bool DepthFirstSearch(Node* node, NodeVisitor* visitor);

		/**
		 * See the documentation and arguments for @ref DepthFirstSearch.  All is the
		 * same here, except that we traverse breadth first.
		 */
		bool BreadthFirstSearch(Node* node, NodeVisitor* visitor);

		/**
		 * These are the nodes (or vertices) of the graph.  Typically, the user will
		 * define a derivative of this class so as to provide satalite data or override
		 * virtual methods.
		 */
		class UU_API Node
		{
			friend class Graph;

		public:
			Node();
			virtual ~Node();

			/**
			 * Some algorithms take node-weight into account.  For those that do, this
			 * method can be overridden to provide that weight.  By default, here we
			 * just return zero.
			 */
			virtual double GetWeight() const;

		protected:

			/**
			 * This is used internally by the @ref DepthFirstSearch method.
			 */
			bool DepthFirstSearchRecursive(NodeVisitor* visitor);

			DArray<Edge*> adjacencyArray;
			mutable bool considered;
		};

		/**
		 * These are the edges of the graph (connections between nodes.)  The user may
		 * wish to define a derivative of this class to add satalite data and/or override
		 * virtual methods.
		 */
		class UU_API Edge
		{
		public:
			Edge();
			virtual ~Edge();

			/**
			 * Some algorithms take edge-weight into account.  For those that do, this
			 * method can be overridden to provide that weight.  By default, here we
			 * just return zero.
			 */
			virtual double GetWeight() const;

			/**
			 * Have this edge connect the two given nodes.  This will fail if this
			 * edge already connects two nodes in the graph.
			 */
			bool Link(Node* nodeA, Node* nodeB);

			/**
			 * Disconnect the nodes joined by this edge.  This will fail if this
			 * edge does not already connect two nodes in the graph.
			 */
			bool Unlink();

			/**
			 * Follow this edge from the given node to the other node connected
			 * by this edge.  This will fail if this edge is not linked or if the
			 * given node is not one of the nodes linked by this edge.
			 */
			Node* Follow(Node* node);

		protected:
			Node* nodeArray[2];
		};

		/**
		 * This defines an interface that can be used during graph traversal
		 * to visit nodes in the graph.
		 */
		class UU_API NodeVisitor
		{
		public:
			/**
			 * Override this to process the given node.
			 * 
			 * @return Indicate whether traversal should continue (true) or stop (false.)
			 */
			virtual bool VisitNode(Node* node) = 0;
		};

		/**
		 * For convenience, here a visitor is provided that can make use of lambdas.
		 */
		template<typename L>
		class UU_API NodeLambdaVisitor : public NodeVisitor
		{
		public:
			NodeLambdaVisitor(L lambda)
			{
				this->lambda = lambda;
			}

			virtual bool VisitNode(Node* node) override
			{
				return this->lambda(node);
			}

		private:
			L lambda;
		};

	protected:

		/**
		 * This is used internally to prepare for graph traversal.
		 */
		void ClearTraversalFlags();

		DArray<Node*> nodeArray;
		DArray<Edge*> edgeArray;
	};
}