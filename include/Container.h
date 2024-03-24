#ifndef CONTAINER_H
#define CONTAINER_H

#include <cstdint>
#include <vector>
#include "glm/glm.hpp"


class Container {
public:
	Container() = default;
	~Container() = default;
	explicit Container(float upper, float lower, float left, float right, uint32_t num_blocks_x, uint32_t num_blocks_y);

	void initContainer();

	uint32_t getBlockId(float x, float y) const;
	uint32_t getBlockId(glm::vec2 pos) const;

	float upper, lower, left, right, block_size_x, block_size_y;
	uint32_t num_blocks_x, num_blocks_y;
	std::vector<std::vector<uint32_t>> blocks;
};

#endif // CONTAINER_H