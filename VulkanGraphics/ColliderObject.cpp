#include "ColliderObject.h"

ColliderObject::ColliderObject()
{
}

ColliderObject::ColliderObject(const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indicesdata, glm::vec3 position, CollidorType type)
{
	VertexList = vertexList;
	Indicesdata = indicesdata;

	const glm::vec3 BottomLeftVertex = position + VertexList[0].Position;
	const glm::vec3 BottomRightVertex = position + VertexList[1].Position;
	const glm::vec3 TopRightVertex = position + VertexList[2].Position;
	const glm::vec3 TopLeftVertex = position + VertexList[3].Position;

	Position = position;
	Collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
	Type = type;
}

ColliderObject::~ColliderObject()
{
}

void ColliderObject::Update(glm::vec2 position)
{
}

void ColliderObject::Update(glm::vec3 position)
{
	Position = position;
	const glm::vec3 BottomLeftVertex = position + VertexList[0].Position;
	const glm::vec3 BottomRightVertex = position + VertexList[1].Position;
	const glm::vec3 TopRightVertex = position + VertexList[2].Position;
	const glm::vec3 TopLeftVertex = position + VertexList[3].Position;
	Collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}