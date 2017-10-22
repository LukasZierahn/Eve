#include "ModelLoader.h"
#include "RenderClass.h"
#include "Texture.h"
#include "Include.h"

ModelLoader::ModelLoader(RenderClass* rend)
{
	render = rend;

	render->GetDevice(&device);
	render->GetDeviceContext(&deviceContext);

	errorText = new Texture(render, "Error");
}

bool ModelLoader::LoadModel(string name, vector<Vertex> *outputVertex, vector<WORD> *outputIndex, bool forceRecreation)
{
	ifstream modelStream;
	char input;

	string fullFilename = "Data\\";
	fullFilename += name;
	fullFilename += ".cng";

	if (forceRecreation)
		CreateModel(name);

	//open the document
	modelStream.open(fullFilename.c_str(), ifstream::in);

	if (modelStream.fail())
	{
		modelStream.close();
		if (CreateModel(name))
		{
			modelStream.open(fullFilename.c_str(), ifstream::in);
			if (modelStream.fail())
			{
				modelStream.close();
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	//Load the Indexes and Vertexes
	Vertex newVertex;

	WORD newIndex;

	short i;

	while (!modelStream.eof())
	{
		//get the first char of the line
		modelStream.get(input);

		//Vertecies
		if (input == 'v')
		{
			modelStream.get(input);
			if (input == ' ')
			{
				modelStream >> newVertex.position.x >> newVertex.position.y >> newVertex.position.z >> newVertex.texturePos.x >> newVertex.texturePos.y;
				outputVertex->push_back(newVertex);

				modelStream.ignore(1000, '\n');
			}
		}
		//Indecies
		else if (input == 'f')
		{
			modelStream.get(input);
			if (input == ' ')
			{
				for (i = 0; i < 3; i++)
				{
					modelStream >> newIndex;
					outputIndex->push_back(newIndex);

					modelStream.get(input);
					while (input != ' ' && input != '\n')
					{
						modelStream.get(input);
					}
				}
			}
		}
		//skip to the next line
		else if (input != '\n')
		{
			modelStream.ignore(1000, '\n');

		}
	}

	outputVertex->shrink_to_fit();
	outputIndex->shrink_to_fit();

	return true;
}

//this function takes in wavefronts .obj files and translates them to .cng files
bool ModelLoader::CreateModel(string name)
{
	struct InstallVertex
	{
		int newIndex = 0;
		vector<WORD> texIndexes;
	};

	ifstream inputStream;
	ofstream outputStream;
	vector<Vertex>	inputvertexVector, outputvertexVector;
	vector<WORD>	inputindexVector, inputTexindexVector;
	vector<InstallVertex>	installvertexVector;
	vector<float>	texVertex1, texVertex2;
	Vertex	newVertex;
	WORD newIndex = 0, newTexIndex = 0;
	float texVex1, texVex2;
	short i;
	char input;

	string fullFilenameInput = "Source\\";
	fullFilenameInput += name;
	fullFilenameInput += ".obj";

	inputStream.open(fullFilenameInput.c_str(), ifstream::in);
	if (inputStream.fail())
		return false;

	while (!inputStream.eof())
	{
		inputStream.get(input);

		//Vertecies
		if (input == 'v')
		{
			inputStream.get(input);
			if (input == ' ')
			{
				inputStream >> newVertex.position.x >> newVertex.position.y >> newVertex.position.z;
				newVertex.position.z = newVertex.position.z * -1.0f;
				newVertex.texturePos.x = -1;
				newVertex.texturePos.y = -1;
				inputvertexVector.push_back(newVertex);

				inputStream.ignore(1000, '\n');
			}
			else if (input == 't')
			{
				inputStream.get(input);

				inputStream >> texVex1 >> texVex2;
				texVertex1.push_back(texVex1);
				texVertex2.push_back(1 - texVex2);

				inputStream.ignore(1000, '\n');
			}
		}
		else if (input == 'f') //Indecies
		{
			inputStream.get(input);
			if (input == ' ')
			{
				for (i = 0; i < 3; i++)
				{
					inputStream >> newIndex >> input >> newTexIndex;
					inputindexVector.push_back(newIndex - 1);
					inputTexindexVector.push_back(newTexIndex - 1);

					inputStream.get(input);
					while (input != ' ' && input != '\n')
					{
						inputStream.get(input);
					}
				}
			}
		}
		else if (input != '\n') //skip to the next line
		{
			inputStream.ignore(1000, '\n');

		}
	}

	inputStream.close();

	inputvertexVector.shrink_to_fit();
	inputindexVector.shrink_to_fit();
	inputTexindexVector.shrink_to_fit();
	texVertex1.shrink_to_fit();
	texVertex2.shrink_to_fit();

	string fullFilenameOutput = "Data\\";
	fullFilenameOutput += name;
	fullFilenameOutput += ".cng";

	outputStream.open(fullFilenameOutput.c_str());

	int Indexes = inputindexVector.size();
	int lastNewIndex = 0;


	if (outputStream.fail())
		return false;

	for (unsigned int i = 0; i < inputvertexVector.size(); i++)
	{
		InstallVertex newIndex;
		newIndex.newIndex = lastNewIndex;
		installvertexVector.push_back(newIndex);

		for (int f = 0; f < Indexes; f++)
		{
			if (inputindexVector[f] == i)
			{
				bool skip = false;

				for (unsigned int AP = 0; AP < installvertexVector[i].texIndexes.size(); AP++)
				{
					if (inputTexindexVector[f] == installvertexVector[i].texIndexes[AP])
					{
						skip = true;
					}
				}

				if (!skip)
				{
					outputStream << "v " << inputvertexVector[i].position.x << ' ' << inputvertexVector[i].position.y << ' ' << inputvertexVector[i].position.z
						<< ' ' << texVertex1[inputTexindexVector[f]] << ' ' << texVertex2[inputTexindexVector[f]] << endl;

					installvertexVector[i].texIndexes.push_back(inputTexindexVector[f]);
					lastNewIndex++;
				}
			}
		}
	}

	for (unsigned int f = 0; f < inputindexVector.size(); f += 3)
	{
		outputStream << 'f';
		for (int face = 0; face < 3; face++)
		{
			int indexToLookAt = inputindexVector[f + face];

			for (unsigned int i = 0; i < installvertexVector[indexToLookAt].texIndexes.size(); i++)
			{
				if (installvertexVector[indexToLookAt].texIndexes[i] == inputTexindexVector[f + face])
				{
					int Print = installvertexVector[indexToLookAt].newIndex + i;
					outputStream << ' ' << Print;
					continue;
				}
			}
		}
		outputStream << endl;
	}

	outputStream.close();

	return true;
}

void ModelLoader::GetModel(string name, ModelData** OutputData, bool recreate)
{
	if (modelDataMap[name] == nullptr)
		modelDataMap[name] = new ModelData();

	if (!modelDataMap[name]->initialized || recreate)
	{
		vector<Vertex>	vertexVector;
		vector<WORD>	indexVector;

		bool worked = LoadModel(name, &vertexVector, &indexVector, recreate);

		modelDataMap[name]->vertexCount = vertexVector.size();
		modelDataMap[name]->indexCount = indexVector.size();

		D3D11_SUBRESOURCE_DATA InitVertexData;
		ZeroMemory(&InitVertexData, sizeof(InitVertexData));
		InitVertexData.pSysMem = vertexVector.data();

		D3D11_SUBRESOURCE_DATA InitIndexData;
		ZeroMemory(&InitIndexData, sizeof(InitIndexData));
		InitIndexData.pSysMem = indexVector.data();

		D3D11_BUFFER_DESC VertexBufferDesc;
		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = sizeof(Vertex) * modelDataMap[name]->vertexCount;
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;

		D3D11_BUFFER_DESC IndexBufferDesc;
		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = sizeof(WORD) * modelDataMap[name]->indexCount;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;

		device->CreateBuffer(&VertexBufferDesc, &InitVertexData, modelDataMap[name]->vertexBuffer.GetAddressOf());
		device->CreateBuffer(&IndexBufferDesc, &InitIndexData, modelDataMap[name]->indexBuffer.GetAddressOf());
		modelDataMap[name]->initialized = true;

	}
	*OutputData = modelDataMap[name];
}

void ModelLoader::GetTexture(string name, Texture** tex)
{
	if (textureMap[name] == nullptr)
	{
		textureMap[name] = new Texture(render, name);


		if (!textureMap[name]->hasSucceded())
		{
			delete textureMap[name];
			textureMap[name] = nullptr;
			*tex = errorText;
			return;
		}
	}

	*tex = textureMap[name];
}

ModelLoader::~ModelLoader()
{
	delete errorText;
	for (auto const it : modelDataMap)
	{
		delete it.second;
	}

	for (auto const it : textureMap)
	{
		if (it.second != nullptr)
			delete it.second;
	}
}
