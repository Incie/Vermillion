#include"pch.h"
#include"opengl2_shader.h"
#include"GL\glew.h"
#include"log.h"
#include"file.h"


GLSLShader::GLSLShader()
	: shaderId(0), shaderType(ShaderType::SHADER_NOT_INITIALIZED)
{
}

GLSLShader::~GLSLShader()
{
}

bool GLSLShader::Load(const std::string & shaderFilePath)
{
	Log::Info("GLSLShader", fmt::format("Loading Shader {0}", shaderFilePath) );

	if (shaderFilePath.find(".vert") != std::string::npos ) {
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderFilePath.find(".frag") != std::string::npos) {
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else {
		Log::Error("GLSLShader", "Shadertype not supported");
		Log::Error("GLSLShader", shaderFilePath);
		return false;
	}

	const auto shaderContent = FileReader::ReadFileContent(shaderFilePath);
	const char* charContent = shaderContent.c_str();
	int shaderContentLength = (int)shaderContent.length();
	glShaderSource(shaderId, 1, &charContent, &shaderContentLength);
	glCompileShader(shaderId);

	return false;
}

void GLSLShader::Unload()
{
	if( shaderId != 0 )
		glDeleteShader(shaderId);
	shaderId = 0;
}

GLSLProgram::GLSLProgram()
	: programId(0)
{
}

GLSLProgram::~GLSLProgram()
{
}

bool GLSLProgram::LoadProgram(const std::string & shader)
{
	programId = glCreateProgram();

	if (!vertex.Load(fmt::format("{0}{1}", shader, ".vert"))) {
		Log::Error("GLSLProgram", fmt::format("Failed to load vertex shader {0}", shader).c_str() );
		UnloadProgram();
		return false;
	}


	if (!fragment.Load(fmt::format("{0}{1}", shader, ".frag"))) {
		Log::Error("GLSLProgram", fmt::format("Failed to load fragment shader {0}", shader).c_str());
		UnloadProgram();
		return false;
	}

	glAttachShader(programId, vertex.shaderId);
	glAttachShader(programId, fragment.shaderId);

	glLinkProgram(programId);
	glUseProgram(programId);

	NoProgram();

	return true;
}

void GLSLProgram::UnloadProgram()
{
	glDetachShader(programId, fragment.shaderId);
	glDetachShader(programId, vertex.shaderId);

	fragment.Unload();
	vertex.Unload();

	glDeleteProgram(programId);
}

void GLSLProgram::SetUniform(const std::string & uniformname, int value)
{
	auto uniformlocation = glGetUniformLocation(programId, uniformname.c_str());
	glUniform1i(uniformlocation, value);
}

void GLSLProgram::SetUniform(const std::string & uniformname, const glm::vec4 & color)
{
	auto uniformlocation = glGetUniformLocation(programId, uniformname.c_str());
	glUniform4f(uniformlocation, color.r, color.g, color.b, color.a);
}

void GLSLProgram::SetUniform(const std::string & uniformname, const glm::vec3& color)
{
	auto uniformlocation = glGetUniformLocation(programId, uniformname.c_str());
	glUniform4f(uniformlocation, color.r, color.g, color.b, 1.0f);
}

void GLSLProgram::Use()
{
	glUseProgram(programId);
}

void GLSLProgram::NoProgram()
{
	glUseProgram(0);
}


void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

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

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}