#ifndef SHADER_H

#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	unsigned int ID;



	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode   = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		int success;
		char infoLog[512];
		unsigned int vs, fs;

		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs,1,&vShaderCode,NULL);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(vs, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs,1,&fShaderCode,NULL);
		glCompileShader(fs);

		success = 0;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fs, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		ID = glCreateProgram();
			glAttachShader(ID,vs);
			glAttachShader(ID, fs);
			glLinkProgram(ID);
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if(!success) {
			    glGetProgramInfoLog(ID, 512, NULL, infoLog);
			    std::cout << "ERROR::Program::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			glDeleteShader(vs);
			glDeleteShader(fs);

	}

	void use()
	{
		glUseProgram(ID);
	}
	void deleteProgram()
	{
		 glDeleteProgram(ID);
	}

	void setBool(const std::string &name, bool value) const
	{
	    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
	    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
	    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}




};

#endif
