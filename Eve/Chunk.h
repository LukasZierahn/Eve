#ifndef __H__Chunk
#define __H__Chunk

class Chunk
{
private:
	int x, y, z;
	int volume;
	int size;

public:
	Chunk(int x, int y, int z);
	~Chunk();
};

#endif