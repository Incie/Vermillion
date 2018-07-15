#include "opengl2_shader.h"
#include"GL\glew.h"
#include"log.h"
#include"file.h"
#include"fmt\format.h"

GLSLShader::GLSLShader()
{
}

GLSLShader::~GLSLShader()
{
}

bool GLSLShader::Load(const std::string & shader)
{
	if (shader.find(".vert") != std::string::npos ) {
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shader.find(".frag") != std::string::npos) {
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else {
		Log::Error("GLSLShader", "Shadertype not suppoerted");
		Log::Error("GLSLShader", shader);
	}

	const auto shaderContent = FileReader::ReadFileContent(shader);
	const char* charContent = shaderContent.c_str();
	int len = shaderContent.length();
	glShaderSource(shaderId, 1, &charContent, &len);
	glCompileShader(shaderId);

	return false;
}

void GLSLShader::Unload()
{
	if( shaderId != 0 )
		glDeleteShader(shaderId);
	shaderId = 0;
}

bool GLSLProgram::LoadProgram(const std::string & shader)
{
	programId = glCreateProgram();

	vertex.Load( fmt::format("{0}{1}", shader, ".vert") );
	fragment.Load(fmt::format("{0}{1}", shader, ".frag"));

	glAttachShader(programId, vertex.shaderId);
	glAttachShader(programId, fragment.shaderId);

	glLinkProgram(programId);
	glUseProgram(programId);	

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