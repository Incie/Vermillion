#include"pch.h"
#include"opengl2_shader.h"
#include"GL\glew.h"
#include"log.h"
#include"file.h"

constexpr int INVALID_SHADER_ID = -1;

GLSLShader::GLSLShader()
	: shaderId(INVALID_SHADER_ID), shaderType(ShaderType::SHADER_NOT_INITIALIZED)
{
}

GLSLShader::~GLSLShader()
{
}

bool GLSLShader::Load(const FilePath& shaderFilePath)
{
	Log::Info("GLSLShader", fmt::format("Loading Shader {0}", shaderFilePath.tostring()) );

	if (shaderFilePath.tostring().find(".vert") != std::string::npos ) {
		shaderId = GLCHECK(glCreateShader(GL_VERTEX_SHADER))
	}
	else if (shaderFilePath.tostring().find(".frag") != std::string::npos) {
		shaderId = GLCHECK(glCreateShader(GL_FRAGMENT_SHADER))
	}
	else {
		Log::Error("GLSLShader", "Shadertype not supported");
		Log::Error("GLSLShader", shaderFilePath.tostring());
		return false;
	}

	const auto shaderContent = FileReader::ReadFileContent(shaderFilePath);
	const char* charContent = shaderContent.c_str();
	int shaderContentLength = (int)shaderContent.length();
	GLCHECK(glShaderSource(shaderId, 1, &charContent, &shaderContentLength))
	GLCHECK(glCompileShader(shaderId))

	return true;
}

void GLSLShader::Unload()
{
	if( shaderId != INVALID_SHADER_ID)
		GLCHECK(glDeleteShader(shaderId))
	shaderId = INVALID_SHADER_ID;
}

GLSLProgram::GLSLProgram()
	: programId(0)
{
}

GLSLProgram::~GLSLProgram()
{
}

bool GLSLProgram::LoadProgram(const std::string& shader)
{
	programId = glCreateProgram();

	FilePath vertexPath = { Paths::Shaders, shader + ".vert" };
	if (!vertex.Load(vertexPath)) {
		Log::Error("GLSLProgram", fmt::format("Failed to load vertex shader {0}", shader).c_str() );
		UnloadProgram();
		return false;
	}

	FilePath fragmentPath = { Paths::Shaders, shader + ".frag" };
	if (!fragment.Load(fragmentPath)) {
		Log::Error("GLSLProgram", fmt::format("Failed to load fragment shader {0}", shader).c_str());
		UnloadProgram();
		return false;
	}

	GLCHECK(glAttachShader(programId, vertex.shaderId))
	GLCHECK(glAttachShader(programId, fragment.shaderId))

	GLCHECK(glLinkProgram(programId))
	GLCHECK(glUseProgram(programId))

	NoProgram();

	return true;
}

void GLSLProgram::UnloadProgram()
{
	if(fragment.shaderId != INVALID_SHADER_ID) {
		GLCHECK(glDetachShader(programId, fragment.shaderId));
		fragment.Unload();
	}

	if(vertex.shaderId != INVALID_SHADER_ID) {
		GLCHECK(glDetachShader(programId, vertex.shaderId));
		vertex.Unload();
	}

	if(programId != INVALID_SHADER_ID) {
		GLCHECK(glDeleteProgram(programId));
		programId = INVALID_SHADER_ID;
	}
}

void GLSLProgram::SetUniform(const std::string& uniformname, int value) const
{
	auto uniformlocation = glGetUniformLocation(programId, uniformname.c_str());
	GLCHECK(glUniform1i(uniformlocation, value))
}

void GLSLProgram::SetUniform(const std::string& uniformname, const glm::vec4& color) const
{
	auto uniformlocation = glGetUniformLocation(programId, uniformname.c_str());
	GLCHECK(glUniform4f(uniformlocation, color.r, color.g, color.b, color.a))
}

void GLSLProgram::SetUniform(const std::string& uniformname, const glm::vec3& color) const
{
	auto uniformlocation = glGetUniformLocation(programId, uniformname.c_str());
	GLCHECK(glUniform4f(uniformlocation, color.r, color.g, color.b, 1.0f))
}

void GLSLProgram::Use() const
{
	GLCHECK(glUseProgram(programId))
}

void GLSLProgram::NoProgram() const
{
	GLCHECK(glUseProgram(0))
}


void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	GLCHECK(glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength))

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	GLCHECK(glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength))

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		GLCHECK(glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog))
		printf("%s\n", infoLog);
		free(infoLog);
	}
}