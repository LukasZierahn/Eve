#ifndef __H__Cell
#define __H__Cell

class RenderClass;
class Model;

class Cell
{
private:
	Model* mod = nullptr;

public:
	Cell(RenderClass* rndCls);
	Cell(RenderClass* rndCls, float x, float y, float z);

	Model* GetModel() { return mod; };

	~Cell();
};

#endif