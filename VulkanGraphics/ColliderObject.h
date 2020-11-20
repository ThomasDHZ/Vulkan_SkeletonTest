#pragma once
#include "BoxCollider.h"
#include "Mesh2D.h"
enum CollidorType
{
	LevelCollider,
	EnemyCollider
};

class ColliderObject
{
private:
	std::vector<Vertex> VertexList;
	std::vector<uint16_t> Indicesdata;
	Mesh2D ColliderMesh;
	glm::vec3 Position = glm::vec3(0.0f);
	BoxCollider Collider;
	CollidorType Type;
public:
	ColliderObject();
	ColliderObject(const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indicesdata, glm::vec3 position, CollidorType type);
	~ColliderObject();

	void Update(glm::vec2 position);
	void Update(glm::vec3 position);

	BoxCollider GetCollider() { return Collider; }
	CollidorType GetColliderType() { return Type; }
};