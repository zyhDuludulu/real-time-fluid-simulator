#ifndef KERNEL_FUNC_H
#define KERNEL_FUNC_H


#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <vector>
class KernelFunc {
public:
	KernelFunc() = delete;
	explicit KernelFunc(float h);
	~KernelFunc() = default;

	float value(float r) const;

	glm::vec2 gradient(glm::vec2 r) const;

private:
	float calculateValue(float r) const;

	glm::vec2 calculateGradient(glm::vec2 r) const;

	float m_h;
	float m_h2;
	float m_sigma;
	glm::uvec2 m_buffer_size;
	std::vector<std::vector<glm::vec2>> m_gradient_buffer;
	std::vector<float> m_value_buffer;
};

#endif // KERNEL_FUNC_H