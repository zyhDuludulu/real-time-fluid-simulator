#include "KernelFunc.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cmath>
#include <cstdlib>
#include <glm/gtc/constants.hpp>

KernelFunc::KernelFunc(float h) {
	m_h = h;
	m_h2 = h * h;
	m_sigma = 40.0f / (7.0f * glm::pi<float>() * m_h2);

	m_buffer_size = glm::uvec2(128, 128);
	m_gradient_buffer = std::vector<std::vector<glm::vec2>>(m_buffer_size.x, std::vector<glm::vec2>(m_buffer_size.y));
	m_value_buffer = std::vector<float>(m_buffer_size.x);

	for (int i = 0; i < m_buffer_size.x; i++) {
		for (int j = 0; j < m_buffer_size.y; j++) {
			glm::vec2 r = glm::vec2((static_cast<float>(i) + 0.5f) * m_h / m_buffer_size.x, 
									(static_cast<float>(j) + 0.5f) * m_h / m_buffer_size.y);
			m_gradient_buffer[i][j] = calculateGradient(r);
		}
	}

	for (int i = 0; i < m_buffer_size.x; i++) {
		float r = (static_cast<float>(i) + 0.5f) * m_h / m_buffer_size.x;
		m_value_buffer[i] = calculateValue(r);
	}
}


float
KernelFunc::value(float r) const {
	float i = std::abs(r) / m_h * m_buffer_size.x;
	if (i + 1 >= m_buffer_size.x) {
		return 0.0f;
	}
	// interpolate
	int floor = i;
	float t = (r / m_h * m_buffer_size.x - i);
	return (1.0f - t) * m_value_buffer[floor] + t * m_value_buffer[floor + 1];
}

glm::vec2
KernelFunc::gradient(glm::vec2 r) const {
	glm::vec2 res(0.f, 0.f);

	int i = (std::abs(r.x) * m_buffer_size.x / m_h);
	int j = (std::abs(r.y) * m_buffer_size.x / m_h);

	if (i >= m_buffer_size.x || j >= m_buffer_size.y) { return res; }

	res = m_gradient_buffer[i][j];

	if (r.x < 0) { res.x = -res.x; }
	if (r.y < 0) { res.y = -res.y; }

	return res;
}

float
KernelFunc::calculateValue(float r) const {
	r = std::abs(r);
	float q = r / m_h;
	float q2 = q * q;
	float q3 = q * q2;
	float res = 0.f;
	if (q < 0.5f) {
		res = 6.0f * (q3 - q2) + 1.0f;
		return res * m_sigma;
	} else if (q >= 0.5f && q < 1.f) {
		res = 1.0f - q;
		res = std::pow(res, 3) * 2.f;
		return res * m_sigma;
	}
	return res;
}

glm::vec2
KernelFunc::calculateGradient(glm::vec2 r) const {
	float distance = glm::length(r);
	if (distance < 1e-5) { return glm::vec2(0.f, 0.f); }
	float q = distance / m_h;
	glm::vec2 q_grad = r / (m_h * distance);

	if (q < 0.5f) {
		return 6.0f * (3.f * q * q - 2.f * q) * m_sigma * q_grad;
	} else if (q >= 0.5f && q < 1.0f) {
		return -6.f * powf(1.0f - q, 2) * m_sigma * q_grad;
	}
	return glm::vec2(0.f, 0.f);
}