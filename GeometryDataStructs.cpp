#include "GeometryDataStructs.h"
#include <utility>
#include "Graphics.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Plane;
using std::priority_queue;
using std::shared_ptr;
using std::pair;
using std::vector;

namespace Rendering
{
	/* Attempt at implementing Hausdorff-Distance. */
	float DirectedEdgeMesh::ErrorCost(uint32_t v1, uint32_t v2)
	{
		Vector3 position1 = vertices.at(v1).vertex.position;
		Vector3 position2 = vertices.at(v2).vertex.position;

		float distance = Vector3::Distance(position1, position2);

		vector<uint32_t> oneRing1 = GetOneRingFaces(v1);
		vector<uint32_t> oneRing2 = GetOneRingFaces(v2);

		std::vector<uint32_t> intersection;
		std::sort(oneRing1.begin(), oneRing1.end());
		std::sort(oneRing2.begin(), oneRing2.end());
		std::set_intersection(oneRing1.begin(), oneRing1.end(), oneRing2.begin(), oneRing2.end(), intersection);

		vector<float> costCandidates;

		for (auto face = oneRing1.begin(); face < oneRing1.end(); face++)
		{
			Vector3 normal1 = GetFaceNormal(*face);
			Vector3 normal2 = GetFaceNormal(intersection.at(0));

			float minCandidate1 = 1.0f - normal1.Dot(normal2) * 0.5f;

			normal2 = GetFaceNormal(intersection.at(1));
			float minCandidate2 = 1.0f - normal1.Dot(normal2) * 0.5f;

			float min = minCandidate1 < minCandidate2 ? minCandidate1 : minCandidate2;
			costCandidates.push_back(min);
		}

		std::sort(costCandidates.begin(), costCandidates.end());
		float max = costCandidates.back();
		return max * distance;
	}

	Vector3 DirectedEdgeMesh::GetFaceNormal(uint32_t faceIndex)
	{
		Vector3 point1 = vertices.at(edges.at(Halfedge(faceIndex, 0)).baseVertexIndex).vertex.position;
		Vector3 point2 = vertices.at(edges.at(Halfedge(faceIndex, 1)).baseVertexIndex).vertex.position;
		Vector3 point3 = vertices.at(edges.at(Halfedge(faceIndex, 2)).baseVertexIndex).vertex.position;
		Plane plane = Plane(point1, point2, point3);
		return plane.Normal();
	}

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

		while (newMesh->FaceCount() > targetFaceCount)
		{
			priority_queue<pair<float, uint32_t>, vector<pair<float, uint32_t>>> queue;

			for (uint32_t i = 0; i < newMesh->edges.size(); i++)
			{
				DirectedEdge edge = newMesh->edges.at(i);
				uint32_t v1 = edge.baseVertexIndex;
				uint32_t nextEdge = newMesh->NextEdge(i);
				uint32_t v2 = newMesh->edges.at(nextEdge).baseVertexIndex;

				std::vector<uint32_t> oneRing1 = newMesh->GetOneRing(v1);
				std::vector<uint32_t> oneRing2 = newMesh->GetOneRing(v2);

				// Two boundary vertices must be connected by a boundary halfedge
				if
				(
					newMesh->IsBoundaryVertex(v1) &&
					newMesh->IsBoundaryVertex(v2) &&
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

				queue.push(pair<float, uint32_t>(newMesh->ErrorCost(v1, v2), i));
			}

			uint32_t collapsedEdge = queue.top().second;
			uint32_t collapsedVertex = newMesh->edges.at(newMesh->NextEdge(collapsedEdge)).baseVertexIndex;
			uint32_t targetVertex = newMesh->edges.at(collapsedEdge).baseVertexIndex;

			for (auto it = newMesh->edges.begin(); it < newMesh->edges.end(); it++)
			{
				if (it->baseVertexIndex == collapsedVertex)
					it->baseVertexIndex = targetVertex;
			}

			// TODO: remove vertices/edges properly
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

	std::vector<uint32_t> DirectedEdgeMesh::GetOneRingFaces(uint32_t vertex)
	{
		std::vector<uint32_t> oneRing;

		uint32_t startEdge = vertices.at(vertex).directedEdgeIndex;
		uint32_t currentEdge = startEdge;
		oneRing.push_back(Face(currentEdge));

		uint32_t opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;

		while (opposite != startEdge && opposite != UINT32_MAX)
		{
			oneRing.push_back(Face(currentEdge));
			opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;
		}

		if (opposite == UINT32_MAX)
		{
			opposite = edges.at(startEdge).oppositeEdgeIndex;

			while (opposite != startEdge && opposite != UINT32_MAX)
			{
				oneRing.push_back(Face(currentEdge));
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
