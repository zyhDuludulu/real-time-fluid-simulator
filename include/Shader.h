#ifndef SHADER_H
#define SHADER_H


class Shader
{
public:
	Shader();
	~Shader() = default;
	void loadVFShader(const char* vertexPath, const char* fragmentPath) const;
	void loadVGFShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) const;
	void use() const;

	void setInt(const char* name, int value) const;

	unsigned int id;
};


#endif 