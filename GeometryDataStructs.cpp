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

	///* Search through the edges and try to find another edge
	//	that is connecting the same vertices in the opposite direction. */
	//void DirectedEdgeMesh::LinkEdges()
	//{
	//	for (int i = 0; i < edges.size(); i++)
	//	{
	//		DirectedEdge& edge = edges.at(i);

	//		if (edge.oppositeEdgeIndex == UINT32_MAX)
	//		{
	//			uint32_t nextVertex = edges.at(NextEdge(i)).baseVertexIndex;

	//			for (uint32_t j = i+1; j < edges.size(); j++)
	//			{
	//				DirectedEdge& opposite = edges.at(j);

	//				if (opposite.baseVertexIndex != nextVertex)
	//					continue;

	//				uint32_t oppositeNextVertex = edges.at(NextEdge(j)).baseVertexIndex;

	//				if (edge.baseVertexIndex == oppositeNextVertex)
	//				{
	//					edge.oppositeEdgeIndex = j;
	//					opposite.oppositeEdgeIndex = i;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

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

			std::uniform_int_distribution<uint32_t> distribution(0, edgeMap.size() - 1);
			vector<uint32_t> randomCandidates;
			randomCandidates.reserve(candidatesPerDecimation);

			for (int i = 0; i < candidatesPerDecimation; i++)
			{
				uint32_t randomNumber = distribution(generator);
				while (std::find(randomCandidates.begin(), randomCandidates.end(), randomNumber) != randomCandidates.end())
				{
					randomNumber = distribution(generator);
				}

				randomCandidates.push_back(randomNumber);
			}

			for (auto it = randomCandidates.begin(); it != randomCandidates.end(); it++)
			{
				auto randomIt = edgeMap.begin();
				std::advance(randomIt, *it % edgeMap.size());
				VertexPair edge = randomIt->first;
				uint32_t v1 = edge.first;
				uint32_t v2 = edge.second;

				std::vector<uint32_t> oneRing1 = GetOneRing(v1);
				std::vector<uint32_t> oneRing2 = GetOneRing(v2);

				auto opposite = edgeMap.find(edge);

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
				if (intersection.size() > 2)
					continue;

				queue.push(pair<float, uint32_t>(ErrorCost(v1, v2), *it));
			}

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

		vertices.at(collapsedVertex).deleted = true;

		vector<uint32_t> oneRing = GetOneRing(collapsedVertex);

		std::set<VertexPair> removedPairs;

		removedPairs.insert(VertexPair(collapsedVertex, targetVertex));
		edges.at(collapsedEdge).deleted = true;
		edges.at(PreviousEdge(collapsedEdge)).deleted = true;
		edges.at(NextEdge(collapsedEdge)).deleted = true;

		if (collapsedOpposite != UINT_MAX)
		{
			removedPairs.insert(VertexPair(targetVertex, collapsedVertex));
			edges.at(collapsedOpposite).deleted = true;
			edges.at(PreviousEdge(collapsedOpposite)).deleted = true;
			edges.at(NextEdge(collapsedOpposite)).deleted = true;
		}

		uint32_t wingVertex1 = edges.at(PreviousEdge(collapsedEdge)).baseVertexIndex;

		// w - wing vertex, c - collapsed vertex, t - target Vertex.
		VertexPair wc = VertexPair(wingVertex1, edges.at(collapsedEdge).baseVertexIndex);
		VertexPair cw = VertexPair(wc.second, wc.first);
		VertexPair tw = VertexPair(edges.at(NextEdge(collapsedEdge)).baseVertexIndex, wingVertex1);
		VertexPair wt = VertexPair(tw.second, tw.first);

		edgeMap.at(tw) = edgeMap.at(cw);
		vertices.at(targetVertex).directedEdgeIndex = edgeMap.at(tw);
		GetEdge(tw).baseVertexIndex = targetVertex;
		GetEdge(tw).oppositeEdgeIndex = edgeMap.at(wt);
		GetEdge(wt).oppositeEdgeIndex = edgeMap.at(tw);
		vertices.at(wingVertex1).directedEdgeIndex = edgeMap.at(wt);
		removedPairs.insert(wc);
		removedPairs.insert(cw);

		// delete after debugging
		if (vertices.at(GetEdge(tw).baseVertexIndex).deleted || vertices.at(GetEdge(wt).baseVertexIndex).deleted)
			DebugBreak();

		uint32_t wingVertex2 = UINT_MAX;

		if (collapsedOpposite != UINT_MAX)
		{
			wingVertex2 = edges.at(PreviousEdge(collapsedOpposite)).baseVertexIndex;

			wc = VertexPair(wingVertex2, edges.at(NextEdge(collapsedOpposite)).baseVertexIndex);
			cw = VertexPair(wc.second, wc.first);
			tw = VertexPair(edges.at(collapsedOpposite).baseVertexIndex, wingVertex2);
			wt = VertexPair(tw.second, tw.first);

			edgeMap.at(wt) = edgeMap.at(wc);
			GetEdge(wt).baseVertexIndex = wingVertex2;
			GetEdge(wt).oppositeEdgeIndex = edgeMap.at(tw);
			GetEdge(tw).oppositeEdgeIndex = edgeMap.at(wt);
			vertices.at(wingVertex2).directedEdgeIndex = edgeMap.at(wt);
			removedPairs.insert(cw);
			removedPairs.insert(wc);

			// delete after debugging
			bool twDel = vertices.at(GetEdge(tw).baseVertexIndex).deleted;
			bool wtDel = vertices.at(GetEdge(wt).baseVertexIndex).deleted;
			if (twDel || wtDel)
				DebugBreak();
		}

		for (uint32_t vertex : oneRing)
		{
			if (vertex == targetVertex || vertex == wingVertex1 || vertex == wingVertex2)
				continue;

			VertexPair replaced = VertexPair(collapsedVertex, vertex);
			uint32_t index = edgeMap.at(replaced);
			edgeMap.insert(std::pair<VertexPair, uint32_t>(VertexPair(targetVertex, vertex), index));
			edges.at(index).baseVertexIndex = targetVertex;
			removedPairs.insert(replaced);

			if (GetEdge(replaced).oppositeEdgeIndex != UINT_MAX)
			{
				replaced = VertexPair(vertex, collapsedVertex);
				edgeMap.insert(std::pair<VertexPair, uint32_t>(VertexPair(vertex, targetVertex), edgeMap.at(replaced)));
				removedPairs.insert(replaced);
			}
		}

		for (VertexPair pair : removedPairs)
		{
			int numberErased = edgeMap.erase(pair);
			// delete after debugging
			if (numberErased < 1)
				DebugBreak();
		}
	}

	void DirectedEdgeMesh::CollectGarbage()
	{
		vector<DirectedEdgeVertex> newVertices;
		std::list<uint32_t> deletedVertices;
		for (uint32_t i = 0; i < vertices.size(); i++)
		{
			DirectedEdgeVertex vertex = vertices.at(i);
			if (!vertex.deleted)
			{
				vertex.deleted = false;
				newVertices.push_back(vertex);
			}
			else
			{
				deletedVertices.push_back(i);
			}
		}
		vertices = newVertices;

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
			}

			edge.baseVertexIndex -= baseVertexDelta;
			newEdges.push_back(baseVertexDelta);
		}
		
		edges = newEdges;
	}

	void DirectedEdgeMesh::EstablishEdgeMap()
	{
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

		// delete after debugging
		if (edges.at(NextEdge(currentEdge)).deleted)
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
		auto pointer = edgeMap.find(pair);
		if (pointer == edgeMap.end())
		{
			DebugBreak();
		}
		return edges.at(edgeMap.at(pair));
	}

	uint32_t DirectedEdgeMesh::FaceCount()
	{
		return edges.size() / 3;
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
