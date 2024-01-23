#pragma once


struct sPoint
{
	float position[ 3 ];
};

class cPointCloud
{
public:
	 cPointCloud();
	~cPointCloud();

	void create( size_t _size );
	void update();

	sPoint* data() const { return m_points; }

private:
	unsigned int m_vao = 0;
	unsigned int m_vbo = 0;
	sPoint* m_points = nullptr;
	size_t m_size = 0;
};
