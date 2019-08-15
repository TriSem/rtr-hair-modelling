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
	DirectedEdgeMesh::DirectedEdgeMesh(Mesh basicMesh)
	{
		CreateFromMesh(basicMesh);
	}

	DirectedEdgeMesh::DirectedEdgeMesh(const DirectedEdgeMesh& other)
	{
		this->vertices = other.vertices;
		this->edges = other.edges;
	}

	Mesh DirectedEdgeMesh::ExtractBasicMesh()
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
		}

		EstablishEdgeMap();
		RelinkVertices();
		LinkEdges();
	}

	/* Search through the edges and try to find another edge
		that is connecting the same vertices in the opposite direction. */
	void DirectedEdgeMesh::LinkEdges()
	{
		for (int i = 0; i < edges.size(); i++)
		{
			DirectedEdge& e = edges.at(i);

			if (e.oppositeEdgeIndex == UINT_MAX)
			{
				uint32_t v1 = e.baseVertexIndex;
				uint32_t v2 = edges.at(NextEdge(i)).baseVertexIndex;

				auto searchTarget = std::pair<uint32_t, uint32_t>(v2, v1);
				auto it = edgeMap.find(searchTarget);

				if (it != edgeMap.end())
				{
					e.oppositeEdgeIndex = it->second;
					edges.at(it->second).oppositeEdgeIndex = i;
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

	void DirectedEdgeMesh::Decimate(uint32_t targetFaceCount)
	{
		// The mesh will always contain at least one triangle.
		if (vertices.size() < 4)
			return;

		uint32_t candidatesPerDecimation = 8;
		std::random_device seed;
		std::mt19937 generator(seed());

		while (FaceCount() > targetFaceCount)
		{
			priority_queue<pair<float, uint32_t>, vector<pair<float, uint32_t>>, std::greater<pair<float, uint32_t>>> queue;
			if (candidatesPerDecimation > edgeMap.size())
				candidatesPerDecimation = edgeMap.size();

			std::uniform_int_distribution<uint32_t> distribution(0, edgeMap.size() - 1);
			vector<uint32_t> randomCandidates;
			randomCandidates.reserve(candidatesPerDecimation);

			for (int i = 0; i < candidatesPerDecimation; i++)
			{
				uint32_t randomNumber = distribution(generator);
				while (std::find(randomCandidates.begin(), randomCandidates.end(), randomNumber) != randomCandidates.end() || edges.at(randomNumber).deleted)
				{
					randomNumber = distribution(generator);
				}

				randomCandidates.push_back(randomNumber);
			}

			for (auto it = randomCandidates.begin(); it != randomCandidates.end(); it++)
			{
				uint32_t v1 = edges.at(*it).baseVertexIndex;
				uint32_t v2 = edges.at(NextEdge(*it)).baseVertexIndex;
				VertexPair oppositeEdge = VertexPair(v2, v1);

				std::vector<uint32_t> oneRing1 = GetOneRing(v1);
				std::vector<uint32_t> oneRing2 = GetOneRing(v2);

				auto opposite = edgeMap.find(oppositeEdge);

				// Two boundary vertices must be connected by a boundary halfedge
				if (IsBoundaryVertex(v1) && IsBoundaryVertex(v2) && edges.at(opposite->second).oppositeEdgeIndex != UINT32_MAX)
				{
					continue;
				}

				std::vector<uint32_t> intersection;
				std::sort(oneRing1.begin(), oneRing1.end());
				std::sort(oneRing2.begin(), oneRing2.end());
				std::set_intersection(oneRing1.begin(), oneRing1.end(), oneRing2.begin(), oneRing2.end(), std::back_inserter(intersection));

				// Prevent vertices with valence lower than three.
				//if (intersection.size() > 2)
				//	continue;

				queue.push(pair<float, uint32_t>(ErrorCost(v1, v2), *it));
			}

			if (edges.at(queue.top().second).deleted)
				DebugBreak();
			CollapseEdge(queue.top().second);
		}

		CollectGarbage();
		EstablishEdgeMap();
		RelinkVertices();
		RelinkEdges();
	}

	/** 
		Performs a halfedge collapse on the specified edge. The vertex
		at its foot will be merged into the vertex at its tip.
	*/
	void DirectedEdgeMesh::CollapseEdge(uint32_t collapsedEdge)
	{
		uint32_t collapsedOpposite = edges.at(collapsedEdge).oppositeEdgeIndex;
		uint32_t collapsedVertex = edges.at(collapsedEdge).baseVertexIndex;
		uint32_t targetVertex = edges.at(NextEdge(collapsedEdge)).baseVertexIndex;

		vector<uint32_t> oneRing = GetOneRing(collapsedVertex); 
		vector<uint32_t> targetOneRing = GetOneRing(targetVertex);

		std::set<VertexPair> removedPairs;

		removedPairs.insert(VertexPair(collapsedVertex, targetVertex));

		if (collapsedOpposite != UINT_MAX)
		{
			removedPairs.insert(VertexPair(targetVertex, collapsedVertex));
			edges.at(collapsedOpposite).deleted = true;
		}
		
		uint32_t wingVertex1 = edges.at(PreviousEdge(collapsedEdge)).baseVertexIndex;

		VertexPair wc = VertexPair(wingVertex1, collapsedVertex);
		VertexPair cw = VertexPair(collapsedVertex, wingVertex1);
		VertexPair tw = VertexPair(targetVertex, wingVertex1);
		VertexPair wt = VertexPair(wingVertex1, targetVertex);

		GetEdge(tw).deleted = true;
		GetEdge(wc).deleted = true;

		edgeMap.at(tw) = edgeMap.at(cw);
		GetEdge(tw).baseVertexIndex = targetVertex;
		GetEdge(tw).oppositeEdgeIndex = edgeMap.at(wt);
		GetEdge(wt).oppositeEdgeIndex = edgeMap.at(tw);
		vertices.at(wingVertex1).directedEdgeIndex = edgeMap.at(wt);
		removedPairs.insert(wc);
		removedPairs.insert(cw);

		uint32_t wingVertex2 = UINT_MAX;

		if (collapsedOpposite != UINT_MAX)
		{
			wingVertex2 = edges.at(PreviousEdge(collapsedOpposite)).baseVertexIndex;

			wc = VertexPair(wingVertex2, collapsedVertex);
			cw = VertexPair(collapsedVertex, wingVertex2);
			tw = VertexPair(targetVertex, wingVertex2);
			wt = VertexPair(wingVertex2, targetVertex);

			GetEdge(wt).deleted = true;
			GetEdge(cw).deleted = true;

			edgeMap.at(wt) = edgeMap.at(wc);
			GetEdge(wt).oppositeEdgeIndex = edgeMap.at(tw);
			GetEdge(tw).oppositeEdgeIndex = edgeMap.at(wt);
			vertices.at(wingVertex2).directedEdgeIndex = edgeMap.at(wt);
			removedPairs.insert(cw);
			removedPairs.insert(wc);
		}

		for (uint32_t vertex : oneRing)
		{
			if (vertex == targetVertex || vertex == wingVertex1 || vertex == wingVertex2)
				continue;

			VertexPair replaced = VertexPair(collapsedVertex, vertex);
			uint32_t index = edgeMap.at(replaced);
			edgeMap.insert(std::pair<VertexPair, uint32_t>(VertexPair(targetVertex, vertex), index));
			edges.at(index).baseVertexIndex = targetVertex;
			vertices.at(targetVertex).directedEdgeIndex = index;
			removedPairs.insert(replaced);

			if (GetEdge(replaced).oppositeEdgeIndex != UINT_MAX)
			{
				replaced = VertexPair(vertex, collapsedVertex);
				index = edgeMap.at(replaced);
				edgeMap.insert(std::pair<VertexPair, uint32_t>(VertexPair(vertex, targetVertex), index));
				removedPairs.insert(replaced);
			}
		}

		for (VertexPair pair : removedPairs)
		{
			int numberErased = edgeMap.erase(pair);
		}

		edges.at(collapsedEdge).deleted = true;
		vertices.at(collapsedVertex).deleted = true;

		//if (edges.at(vertices.at(targetVertex).directedEdgeIndex).deleted)
		//	DebugBreak();

		if (edges.at(vertices.at(wingVertex1).directedEdgeIndex).deleted)
			DebugBreak();

		if (edges.at(vertices.at(wingVertex2).directedEdgeIndex).deleted)
			DebugBreak();
	}

	void DirectedEdgeMesh::CollectGarbage()
	{
		vector<DirectedEdgeVertex> newVertices;
		vector<uint32_t> deletedVertices;
		for (uint32_t i = 0; i < vertices.size(); i++)
		{
			DirectedEdgeVertex vertex = vertices.at(i);
			if (!vertex.deleted)
			{
				newVertices.push_back(vertex);
			}
			else
			{
				deletedVertices.push_back(i);
			}
		}
		vertices = newVertices;

		std::sort(deletedVertices.begin(), deletedVertices.end());

		vector<DirectedEdge> newEdges;
		for (DirectedEdge edge : edges)
		{
			if (edge.deleted)
				continue;

			uint32_t baseVertexDelta = 0;
			auto it = deletedVertices.begin();
			while (it != deletedVertices.end() && edge.baseVertexIndex >= *it)
			{
				baseVertexDelta++;
				it++;
			}

			edge.baseVertexIndex -= baseVertexDelta;
			newEdges.push_back(edge);
		}
		
		edges = newEdges;
	}

	void DirectedEdgeMesh::EstablishEdgeMap()
	{
		edgeMap.clear();
		for (uint32_t i = 0; i < edges.size(); i++)
		{
			uint32_t v1 = edges.at(i).baseVertexIndex;
			uint32_t v2 = edges.at(NextEdge(i)).baseVertexIndex;
			std::pair<uint32_t, uint32_t> edge(v1, v2);
			edgeMap.insert(std::pair<VertexPair, uint32_t>(edge, i));
		}
	}

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
		std::set_intersection(oneRing1.begin(), oneRing1.end(), oneRing2.begin(), oneRing2.end(), std::back_inserter(intersection));

		if (intersection.size() < 1)
		{
			return 0.0f;
		}

		vector<float> costCandidates;

		for (auto face = oneRing1.begin(); face < oneRing1.end(); face++)
		{
			Vector3 normal1 = GetFaceNormal(*face);
			Vector3 normal2 = GetFaceNormal(intersection.at(0));

			float min = 1.0f - normal1.Dot(normal2) * 0.5f;

			if (intersection.size() == 2)
			{
				normal2 = GetFaceNormal(intersection.at(1));
				float minCandidate = 1.0f - normal1.Dot(normal2) * 0.5f;

				if (minCandidate < min)
					min = minCandidate;
			}

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

	void DirectedEdgeMesh::RelinkVertices()
	{
		for (uint32_t i = 0; i < edges.size(); i++)
		{
			vertices.at(edges.at(i).baseVertexIndex).directedEdgeIndex = i;
		}
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

			currentEdge = opposite;
			opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;
		}

		return false;
	}

	std::vector<uint32_t> DirectedEdgeMesh::GetOneRing(uint32_t vertex)
	{
		std::vector<uint32_t> oneRing;

		uint32_t startEdge = vertices.at(vertex).directedEdgeIndex;
		uint32_t currentEdge = startEdge;

		if (edges.at(currentEdge).deleted)
			DebugBreak();
		if (vertices.at(vertex).deleted)
			DebugBreak();

		oneRing.push_back(edges.at(NextEdge(currentEdge)).baseVertexIndex);

		uint32_t opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;

		while (opposite != startEdge && opposite != UINT32_MAX)
		{
			currentEdge = opposite;
			oneRing.push_back(edges.at(NextEdge(currentEdge)).baseVertexIndex);
			opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;
		}

		if (opposite == UINT32_MAX)
		{
			opposite = edges.at(startEdge).oppositeEdgeIndex;

			while (opposite != startEdge && opposite != UINT32_MAX)
			{
				currentEdge = opposite;
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
			currentEdge = opposite;
			oneRing.push_back(Face(currentEdge));
			opposite = edges.at(PreviousEdge(currentEdge)).oppositeEdgeIndex;
		}

		if (opposite == UINT32_MAX)
		{
			opposite = edges.at(startEdge).oppositeEdgeIndex;

			while (opposite != startEdge && opposite != UINT32_MAX)
			{
				currentEdge = opposite;
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
		return Halfedge(Face(edgeId), (edgeId + 1) % 3);
	}

	uint32_t DirectedEdgeMesh::PreviousEdge(uint32_t edgeId) const
	{
		return Halfedge(Face(edgeId), (edgeId + 2) % 3);
	}

	DirectedEdge& DirectedEdgeMesh::GetEdge(VertexPair pair)
	{
		return edges.at(edgeMap.at(pair));
	}

	uint32_t DirectedEdgeMesh::FaceCount()
	{
		return edgeMap.size() / 3;
	}


	DirectedEdge::DirectedEdge(uint32_t vertexIndex) :
		oppositeEdgeIndex(UINT32_MAX),
		baseVertexIndex(vertexIndex),
		deleted(false)
	{
	}


	DirectedEdgeVertex::DirectedEdgeVertex(Vertex vertex) :
		vertex(vertex),
		directedEdgeIndex(UINT32_MAX),
		deleted(false)
	{
	}
}
