#pragma once

#include <DirectXMath.h>
#include "Graphics.h"
#include "framework.h"
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include "Search.h"

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;

namespace Rendering
{
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureCoordinate;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Mesh() {}

		Mesh(const Mesh& other)
		{
			this->vertices = other.vertices;
			this->indices = other.indices;
		}

		Mesh& operator=(const Mesh& other)
		{
			this->vertices = other.vertices;
			this->indices = other.indices;
			return *this;
		}
	};

	struct DirectedEdge
	{
		uint32_t oppositeEdgeIndex;
		uint32_t baseVertexIndex;
		bool deleted;

		DirectedEdge(uint32_t vertexIndex);
	};

	struct DirectedEdgeVertex
	{
		Vertex vertex;
		uint32_t directedEdgeIndex;
		bool deleted;

		DirectedEdgeVertex(Vertex vertex);
	};

	/** This data structure stores connectivity information by associating
		each face with three directed edges (halfedges). */
	class DirectedEdgeMesh
	{
		using VertexPair = std::pair<uint32_t, uint32_t>;

	private:
		std::vector<DirectedEdgeVertex> vertices;
		std::vector<DirectedEdge> edges;
		std::map<VertexPair, uint32_t> edgeMap;

		void CreateFromMesh(Mesh basicMesh);

		void RecalculateNormals();

		// Links every edge with their opposite edge, provided one exists.
		void LinkEdges();

		/** Returns the index of a halfedge in the specified face.
			IndexInFace is supposed to be used with one of the values
			0, 1 or 2. */
		uint32_t Halfedge(uint32_t faceId, uint8_t indexInFace) const;

		/** Returns the index of the face containing the specified edge. */
		uint32_t Face(uint32_t edgeId) const;

		/** Returns whether the specified edge is the first, second or third
			directed edge within its face. */
		uint8_t HalfedgeIndexWithinFace(uint32_t edgeId) const;

		uint32_t NextEdge(uint32_t edgeId) const;

		uint32_t PreviousEdge(uint32_t edgeId) const;

		/** Returns a reference to the edge object identified by the two vertices. */
		DirectedEdge& GetEdge(VertexPair pair);

		class ErrorComparison
		{
			bool operator()(std::pair<float, uint32_t> value1, std::pair<float, uint32_t> value2)
			{
				return value1.first > value2.first;
			}
		};

		float ErrorCost(uint32_t v1, uint32_t v2);

		DirectX::SimpleMath::Vector3 GetFaceNormal(uint32_t faceIndex);

		/** Assigns new edges to all the vertices. */
		void RelinkVertices();

		void CollapseEdge(uint32_t collapsedEdge);

		/** Safely remove all edges and vertices that are marked deleted from
			their respective lists.*/
		void CollectGarbage();

		void EstablishEdgeMap();

	public:

		DirectedEdgeMesh(Mesh basicMesh);
		DirectedEdgeMesh(const DirectedEdgeMesh& other);

		Mesh ExtractBasicMesh();

		uint32_t FaceCount();

		/** 
			Returns a new mesh with that has been reduced to the specified number of spaces. 
			Uses a greedy decimation scheme.
		*/
		void Decimate(uint32_t targetFaceCount);

		bool IsBoundaryVertex(uint32_t vertex);

		/** 
			Returns a list of vertices that make up the one-ring neighborhood
			for a given vertex.
		*/
		std::vector<uint32_t> GetOneRing(uint32_t vertex);

		std::vector<uint32_t> GetOneRingFaces(uint32_t vertex);
	};
}