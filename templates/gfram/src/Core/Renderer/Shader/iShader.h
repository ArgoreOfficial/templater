#pragma once

enum eShaderType
{
	None,
	Vertex,
	Fragment
};

class iShader
{
public:
    virtual ~iShader( void ) { }
	virtual void* getHandle() { return nullptr; }

protected:
    iShader( void ) { }

private:

};
