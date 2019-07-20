#pragma once

#include <DirectXMath.h>
#include <vector>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;

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
	uint32_t faceCount;

	void CreateFromMesh(Mesh basicMesh);

	// Links every edge with their opposite edge, provided one exists.
	void LinkEdges();

	/** Returns the index of a halfedge in the specified face.
		IndexInFace is supposed to be used with one of the values
		0, 1 or 2. */
	int32_t HalfedgeByContainingFace(uint32_t faceId, uint8_t indexInFace) const;

	/** Returns the index of the face containing the specified edge. */
	int32_t FaceByContainedEdge(uint32_t edgeId) const;

	/** Returns whether the specified edge is the first, second or third
		directed edge within its face. */
	int8_t HalfedgeIndexWithinFace(uint32_t edgeId) const;

	int32_t NextHalfEdgeInFace(uint32_t edgeId) const;

public:

	DirectedEdgeMesh(Mesh basicMesh);

	Mesh& ExtractBasicMesh();

	// Reset all edges before linking them to their opposite edge.
	void RelinkEdges();
};