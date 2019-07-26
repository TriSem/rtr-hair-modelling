#include "GeometryDataStructs.h"

using std::priority_queue;
using std::shared_ptr;

namespace Rendering
{
	DirectedEdgeMesh::DirectedEdgeMesh(Mesh basicMesh)
	{
		CreateFromMesh(basicMesh);
	}

	Mesh& DirectedEdgeMesh::ExtractBasicMesh()
	{
		Mesh mesh;

		for(auto it = edges.begin(); it != edges.end(); it++)
		{
			mesh.indices.push_back(it->baseVertexIndex);
		}

		for (auto it = vertices.begin(); it != vertices.end(); it++)
		{
			mesh.vertices.push_back(it->vertex);
		}

		return mesh;
	}

	/** 
		The mesh is created in 4 steps:
			1. Determine the amount of faces.
			2. Create all vertices.
			3. Create all edges. Link them with their respective vertex.
			4. Connect all edges with their opposite edge if there is one.
	*/
	void DirectedEdgeMesh::CreateFromMesh(Mesh basicMesh)
	{
		faceCount = basicMesh.indices.size() / 3;

		for (auto it = basicMesh.vertices.begin(); it != basicMesh.vertices.end(); it++)
		{
			vertices.push_back(DirectedEdgeVertex(*it));
		}

		// Create the directed edges and link them with the vertices.
		for (auto it = basicMesh.indices.begin(); it != basicMesh.indices.end(); it++)
		{
			edges.push_back(DirectedEdge(*it));

			DirectedEdgeVertex& vertex = vertices.at(*it);

			if (vertex.directedEdgeIndex == UINT32_MAX)
				vertex.directedEdgeIndex = edges.size() - 1;
		}

		LinkEdges();
	}

	/* Search through the edges and try to find another edge
		that is connecting the same vertices in the opposite direction. */
	void DirectedEdgeMesh::LinkEdges()
	{
		for (int i = 0; i < edges.size(); i++)
		{
			DirectedEdge& edge = edges.at(i);
			uint32_t nextVertex = edges.at(NextEdge(i)).baseVertexIndex;

			if (edge.oppositeEdgeIndex == UINT32_MAX)
			{
				for (uint32_t j = 0; j < edges.size(); j++)
				{
					DirectedEdge& opposite = edges.at(j);
					uint32_t oppositeNextVertex = edges.at(NextEdge(j)).baseVertexIndex;
					if ((opposite.baseVertexIndex) == nextVertex && edge.baseVertexIndex == oppositeNextVertex)
					{
						edge.oppositeEdgeIndex = j;
						opposite.oppositeEdgeIndex = i;
					}
				}
			}
		}
	}

	void DirectedEdgeMesh::RelinkEdges()
	{
		for (auto it = edges.begin(); it != edges.end(); it++)
		{
			it->oppositeEdgeIndex = UINT32_MAX;
		}

		LinkEdges();
	}

	uint32_t DirectedEdgeMesh::FaceCount()
	{
		return edges.size() / 3;
	}

	shared_ptr<DirectedEdgeMesh> DirectedEdgeMesh::Decimate(uint32_t targetFaceCount)
	{
		shared_ptr<DirectedEdgeMesh> newMesh = std::make_shared<DirectedEdgeMesh>(this);

		// The mesh will always contain at least one triangle.
		if (newMesh->vertices.size() < 4)
			return newMesh;

		for (uint32_t i = 0; i < edges.size(); i++)
		{
			DirectedEdge edge = edges.at(i);
			uint32_t v1 = edge.baseVertexIndex;
			uint32_t nextEdge = NextEdge(i);
			uint32_t v2 = edges.at(nextEdge).baseVertexIndex;

			std::vector<uint32_t> oneRing1 = GetOneRing(v1);
			std::vector<uint32_t> oneRing2 = GetOneRing(v2);

			// Two boundary vertices must be connected by a boundary halfedge
			if
			(
				IsBoundaryVertex(v1) &&
				IsBoundaryVertex(v2) &&
				edge.oppositeEdgeIndex != UINT32_MAX
			)
			{
				continue;
			}

			std::vector<uint32_t> intersection;
			std::sort(oneRing1.begin(), oneRing1.end());
			std::sort(oneRing2.begin(), oneRing2.end());
			std::set_intersection(oneRing1.begin(), oneRing1.end(), oneRing2.begin(), oneRing2.end(), intersection);

			// Prevent vertices with valence lower than three.
			if (intersection.size() > 2)
				continue;
		}

		return newMesh;
	}

	bool DirectedEdgeMesh::IsBoundaryVertex(uint32_t vertex)
	{
		uint32_t startEdge = vertices.at(vertex).directedEdgeIndex;
		uint32_t currentEdge = startEdge;
		uint32_t opposite = edges.at(PreviousEdge(startEdge)).oppositeEdgeIndex;

		while (opposite != startEdge)
		{
			if (opposite == UINT32_MAX)
				return true;

			opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;
		}

		return false;
	}

	std::vector<uint32_t> DirectedEdgeMesh::GetOneRing(uint32_t vertex)
	{
		std::vector<uint32_t> oneRing;

		uint32_t startEdge = vertices.at(vertex).directedEdgeIndex;
		uint32_t currentEdge = startEdge;
		oneRing.push_back(edges.at(NextEdge(currentEdge)).baseVertexIndex);

		uint32_t opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;

		while (opposite != startEdge && opposite != UINT32_MAX)
		{
			oneRing.push_back(edges.at(NextEdge(currentEdge)).baseVertexIndex);
			opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;
		}

		if (opposite == UINT32_MAX)
		{
			opposite = edges.at(startEdge).oppositeEdgeIndex;

			while (opposite != startEdge && opposite != UINT32_MAX)
			{
				oneRing.push_back(edges.at(PreviousEdge(currentEdge)).baseVertexIndex);
				opposite = edges.at(NextEdge(currentEdge)).oppositeEdgeIndex;
			}
		}

		return oneRing;
	}

	uint32_t DirectedEdgeMesh::Halfedge(uint32_t faceId, uint8_t indexInFace) const
	{
		return 3 * faceId + indexInFace;
	}

	uint32_t DirectedEdgeMesh::Face(uint32_t edgeId) const
	{
		return edgeId / 3;
	}

	uint8_t DirectedEdgeMesh::HalfedgeIndexWithinFace(uint32_t edgeId) const
	{
		return edgeId % 3;
	}

	uint32_t DirectedEdgeMesh::NextEdge(uint32_t edgeId) const
	{
		return (edgeId + 1) % 3;
	}

	uint32_t DirectedEdgeMesh::PreviousEdge(uint32_t edgeId) const
	{
		return (edgeId + 2) % 3;
	}


	DirectedEdge::DirectedEdge(uint32_t vertexIndex) :
		oppositeEdgeIndex(UINT32_MAX),
		baseVertexIndex(vertexIndex)
	{
	}


	DirectedEdgeVertex::DirectedEdgeVertex(Vertex vertex) :
		vertex(vertex),
		directedEdgeIndex(UINT32_MAX)
	{
	}
}
