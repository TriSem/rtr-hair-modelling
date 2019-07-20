#include "GeometryDataStructs.h"

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
		uint32_t nextVertex = edges.at(NextHalfEdgeInFace(i)).baseVertexIndex;

		if (edge.oppositeEdgeIndex == UINT32_MAX)
		{
			for (uint32_t j = 0; j < edges.size(); j++)
			{
				DirectedEdge& opposite = edges.at(j);
				uint32_t oppositeNextVertex = edges.at(NextHalfEdgeInFace(j)).baseVertexIndex;
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

int32_t DirectedEdgeMesh::HalfedgeByContainingFace(uint32_t faceId, uint8_t indexInFace) const
{
	return 3 * faceId + indexInFace;
}

int32_t DirectedEdgeMesh::FaceByContainedEdge(uint32_t edgeId) const
{
	return edgeId / 3;
}

int8_t DirectedEdgeMesh::HalfedgeIndexWithinFace(uint32_t edgeId) const
{
	return edgeId % 3;
}

int32_t DirectedEdgeMesh::NextHalfEdgeInFace(uint32_t edgeId) const
{
	return (edgeId + 1) % 3;
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
