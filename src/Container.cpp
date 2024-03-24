#include "Container.h"
#include <cmath>

Container::Container(float upper, float lower, float left, float right, uint32_t num_blocks_x, uint32_t num_blocks_y)
	: upper(upper), lower(lower), left(left), right(right), num_blocks_x(num_blocks_x), num_blocks_y(num_blocks_y){
	this->block_size_x = (right - left) / num_blocks_x;
	this->block_size_y = (upper - lower) / num_blocks_y;
}

uint32_t
Container::getBlockId(float x, float y) const {
	uint32_t block_x = floor(x / block_size_x);
	uint32_t block_y = floor(y / block_size_y);
	return block_x + block_y * num_blocks_x;
}

uint32_t
Container::getBlockId(glm::vec2 pos) const {
	uint32_t block_x = floor(pos.x / block_size_x);
	uint32_t block_y = floor(pos.y / block_size_y);
	return block_x + block_y * num_blocks_x;
}

// May be written in ParticleSystem, add particles to blocks
void
Container::initContainer() {
	blocks.resize(num_blocks_x * num_blocks_y);
	for (int i = 0; i < num_blocks_x * num_blocks_y; i++) {
		blocks[i].clear();
	}
}