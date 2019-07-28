#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <queue>

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
	};

	struct DirectedEdge
	{
		uint32_t oppositeEdgeIndex;
		uint32_t baseVertexIndex;

		DirectedEdge(uint32_t vertexIndex);
	};

	struct DirectedEdgeVertex
	{
		Vertex vertex;
		uint32_t directedEdgeIndex;

		DirectedEdgeVertex(Vertex vertex);
	};

	/** This data structure stores connectivity information by associating
		each face with three directed edges (halfedges). */
	class DirectedEdgeMesh
	{
	private:
		std::vector<DirectedEdgeVertex> vertices;
		std::vector<DirectedEdge> edges;

		void CreateFromMesh(Mesh basicMesh);

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

		class CompairError
		{
			bool operator()(pair<double, uint32_t> value1, pair<double, uint32_t> value2)
			{
				return value1.first > value2.first;
			}
		};

		double ErrorCost(uint32_t v1, uint32_t v2);

		Vector3 GetFaceNormal(uint32_t faceIndex);

	public:

		DirectedEdgeMesh(Mesh basicMesh);

		Mesh& ExtractBasicMesh();

		// Reset all edges before linking them to their opposite edge.
		void RelinkEdges();

		uint32_t FaceCount();

		std::shared_ptr<DirectedEdgeMesh> Decimate(uint32_t targetFaceCount);

		bool IsBoundaryVertex(uint32_t vertex);

		/** 
			Returns a list of vertices that make up the one-ring neighborhood
			for a given vertex.
		*/
		std::vector<uint32_t> GetOneRing(uint32_t vertex);

		std::vector<uint32_t> GetOneRingFaces(uint32_t vertex);
	};

	
}