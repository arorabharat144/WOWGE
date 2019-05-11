#include <Graphics/Inc/Graphics.h>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>

#define VerifyOrThrow(condition, message)\
	if(!(condition))\
	{\
		throw std::exception(message);\
	}\

namespace
{
	using BoneIndexMap = std::map<std::string, uint32_t>;
	BoneIndexMap boneIndexMap;
}
struct Params
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
	const char* writeType = nullptr;
};

void PrintHelp()
{
	std::cout
		<< "== ModelImporter Help ==\n"
		<< "Flag -ascii is for ASCII\n"
		<< "Flag -binary is for Binary\n"
		<< "Flag -animation is for animation\n"
		<< "\n"
		<< "Usage:\n"
		<< "\n"
		<< "	ModelImporter.exe <InputFile> <OutputFile> <-a or -b> \n"
		<< "\n";
}

Params ParseArg(int argc, char* argv[])
{
	VerifyOrThrow(argc >= 4, "Invalid number of arguments");

	Params params;

	params.inputFileName = argv[argc - 3];
	params.outputFileName = argv[argc - 2];
	params.writeType = argv[argc - 1];

	return params;
}

void ImportModelAscii(const Params& params)
{
	//Create an instance of the Importer class
	Assimp::Importer importer;

	//And have it read the given file with some example postprocessing
	//Usually - if speed is not the most important aspect for you - you'll
	//probably tot request more postprocessing than we do in this example

	const uint32_t flags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(params.inputFileName, flags);

	VerifyOrThrow(scene != nullptr, importer.GetErrorString());

	std::ofstream outputFile;
	outputFile.open(params.outputFileName);

	//Read mesh data
	if (scene->HasMeshes())
	{
		std::cout << "Reading mesh data...\n";

		outputFile << "Number of meshes: " << scene->mNumMeshes << "\n";

		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];

			outputFile << "Number of vertices: " << inputMesh->mNumVertices << "\n";
			outputFile << "Number of indices: " << inputMesh->mNumFaces * 3 << "\n";

			auto vertices = std::make_unique<WOWGE::Graphics::Vertex[]>(inputMesh->mNumVertices);

			std::cout << "Reading positions...\n";
			outputFile << "Positions: \n";

			//Position
			for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
			{
				auto& vertex = vertices[i];
				vertex.position.x = inputMesh->mVertices[i].x;
				vertex.position.y = inputMesh->mVertices[i].y;
				vertex.position.z = inputMesh->mVertices[i].z;

				outputFile << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << "\n";
			}

			//Tangents
			if (inputMesh->HasTangentsAndBitangents())
			{
				std::cout << "Reading tangents...\n";
				outputFile << "Tangents: \n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];
					vertex.tangent.x = inputMesh->mTangents[i].x;
					vertex.tangent.y = inputMesh->mTangents[i].y;
					vertex.tangent.z = inputMesh->mTangents[i].z;

					if (isnan(vertex.tangent.x))
					{
						outputFile << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
					}
					else
					{
						outputFile << vertex.tangent.x << " " << vertex.tangent.y << " " << vertex.tangent.z << "\n";
					}
				}
			}
			else
			{
				outputFile << "Tangents:NO\n";
			}

			//UV
			if (inputMesh->HasTextureCoords(0))
			{
				std::cout << "Reading texture coordinates...\n";
				outputFile << "UVs: \n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];
					vertex.uv.x = inputMesh->mTextureCoords[0][i].x;
					vertex.uv.y = inputMesh->mTextureCoords[0][i].y;

					outputFile << vertex.uv.x << " " << vertex.uv.y << "\n";
				}
			}
			else
			{
				outputFile << "UVs:NO\n";
			}

			std::cout << "Reading indices...\n";
			outputFile << "Indices: \n";

			auto indices = std::make_unique<uint32_t[]>(inputMesh->mNumFaces * 3);

			for (uint32_t face = 0, index = 0; face < inputMesh->mNumFaces; ++face, index += 3)
			{
				indices[index] = inputMesh->mFaces[face].mIndices[0];
				indices[index + 1] = inputMesh->mFaces[face].mIndices[1];
				indices[index + 2] = inputMesh->mFaces[face].mIndices[2];

				outputFile << indices[index] << " " << indices[index + 1] << " " << indices[index + 2] << "\n";
			}

			//Normals
			if (inputMesh->HasNormals())
			{
				std::cout << "Reading normals...\n";
				outputFile << "Normals: \n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];
					vertex.normal.x = inputMesh->mNormals[i].x;
					vertex.normal.y = inputMesh->mNormals[i].y;
					vertex.normal.z = inputMesh->mNormals[i].z;

					outputFile << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << "\n";
				}
			}
			else
			{
				WOWGE::Graphics::GraphicsUtil::ComputeNormals(vertices.get(), inputMesh->mNumVertices, indices.get(), inputMesh->mNumFaces * 3);

				outputFile << "Normals: \n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];

					if (isnan(vertex.normal.x))
					{
						outputFile << 0.0f << " " << 0.0f << " " << 0.0f << "\n";
					}
					else
					{
						outputFile << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << "\n";
					}
				}
			}

			std::cout << "Reading material...\n";
			outputFile << "Material: \n";

			aiMaterial* inputMaterial = scene->mMaterials[inputMesh->mMaterialIndex];
			aiColor4D color;
			float power;

			outputFile << "Ambient: \n";
			inputMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			outputFile << color.r << " " << color.g << " " << color.b << " " << color.a << "\n";

			outputFile << "Diffuse: \n";
			inputMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			outputFile << color.r << " " << color.g << " " << color.b << " " << color.a << "\n";

			outputFile << "Specular: \n";
			inputMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			outputFile << color.r << " " << color.g << " " << color.b << " " << color.a << "\n";

			outputFile << "Emissive: \n";
			inputMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			outputFile << color.r << " " << color.g << " " << color.b << " " << color.a << "\n";

			outputFile << "Power: \n";
			inputMaterial->Get(AI_MATKEY_SHININESS, power);
			outputFile << power << "\n";
		}
		outputFile.close();
	}
}

WOWGE::Maths::Matrix44 Convert(aiMatrix4x4 transform)
{
	WOWGE::Maths::Matrix44 matrix;
	matrix.m11 = static_cast<float>(transform.a1); matrix.m12 = static_cast<float>(transform.a2); matrix.m13 = static_cast<float>(transform.a3); matrix.m14 = static_cast<float>(transform.a4);
	matrix.m21 = static_cast<float>(transform.b1); matrix.m22 = static_cast<float>(transform.b2); matrix.m23 = static_cast<float>(transform.b3); matrix.m24 = static_cast<float>(transform.b4);
	matrix.m31 = static_cast<float>(transform.c1); matrix.m32 = static_cast<float>(transform.c2); matrix.m33 = static_cast<float>(transform.c3); matrix.m34 = static_cast<float>(transform.c4);
	matrix.m41 = static_cast<float>(transform.d1); matrix.m42 = static_cast<float>(transform.d2); matrix.m43 = static_cast<float>(transform.d3); matrix.m44 = static_cast<float>(transform.d4);

	return WOWGE::Maths::Transpose(matrix);
}

uint32_t GetBoneIndex(aiBone* bone, WOWGE::Graphics::Model& model)
{
	auto& it{ boneIndexMap.find(bone->mName.C_Str()) };
	if (it != boneIndexMap.end())
	{
		return it->second;
	}
	WOWGE::Graphics::Bone* newBone{ new WOWGE::Graphics::Bone() };
	newBone->name = bone->mName.C_Str();
	newBone->index = model.GetBones().size();
	newBone->offsetTransform = Convert(bone->mOffsetMatrix);
	model.GetBones().emplace_back(newBone);
	boneIndexMap.insert(std::make_pair(newBone->name, newBone->index));
	return newBone->index;
}

WOWGE::Graphics::Bone* BuildSkeleton(aiNode& node, WOWGE::Graphics::Bone* parent, WOWGE::Graphics::Model& model)
{
	WOWGE::Graphics::Bone* bone{ nullptr };

	auto iter = boneIndexMap.find(node.mName.C_Str());
	if (iter == boneIndexMap.end())
	{
		const uint32_t boneIndex{ model.GetBones().size() };

		//We need to add a new bone
		bone = new WOWGE::Graphics::Bone();
		bone->index = boneIndex;
		bone->offsetTransform = WOWGE::Maths::Matrix44::Identity();

		if (node.mName.length > 0)
		{
			bone->name = node.mName.C_Str();
		}
		else
		{
			char buffer[128];
			sprintf_s(buffer, 128, "unnamed_%u", boneIndex);
			std::cout << "Warning: Bone" << boneIndex << " has no name, renamed to" << buffer;
			bone->name = buffer;
		}

		model.GetBones().emplace_back(bone);
		boneIndexMap.try_emplace(bone->name, bone->index);
	}
	else
	{
		bone = model.GetBones()[iter->second];
	}

	bone->transform = Convert(node.mTransformation);
	bone->parent = parent;
	if (bone->parent)
	{
		bone->parentIndex = parent->index;
	}
	else
	{
		bone->parentIndex = -1;
	}

	for (uint32_t i{ 0 }; i < node.mNumChildren; ++i)
	{
		WOWGE::Graphics::Bone* child{ BuildSkeleton(*(node.mChildren[i]), bone, model) };
		bone->children.emplace_back(child);
		bone->childrenIndex.emplace_back(child->index);
	}

	return bone;
}

void ImportModelBinary(const Params& params)
{
	//Create an instance of the Importer class
	Assimp::Importer importer;

	//And have it read the given file with some example postprocessing
	//Usually - if speed is not the most important aspect for you - you'll
	//probably tot request more postprocessing than we do in this example

	const uint32_t flags =
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(params.inputFileName, flags);

	VerifyOrThrow(scene != nullptr, importer.GetErrorString());

	WOWGE::Graphics::Model model;
	std::vector<WOWGE::Graphics::AnimationClip> animationClips;

	//Read mesh data
	if (scene->HasMeshes())
	{
		std::cout << "Reading mesh data...\n";

		model.GetMeshes() = new WOWGE::Graphics::SkinnedMesh[scene->mNumMeshes];
		model.GetNumberOfMeshes() = scene->mNumMeshes;

		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];

			model.GetMeshes()[meshIndex].Allocate(inputMesh->mNumVertices, inputMesh->mNumFaces * 3);

			auto vertices = std::make_unique<WOWGE::Graphics::Vertex[]>(inputMesh->mNumVertices);

			std::cout << "Reading positions...\n";

			//Position
			for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
			{
				auto& vertex = vertices[i];
				vertex.position.x = inputMesh->mVertices[i].x;
				vertex.position.y = inputMesh->mVertices[i].y;
				vertex.position.z = inputMesh->mVertices[i].z;

				model.GetMeshes()[meshIndex].GetVertex(i).position = vertex.position;
			}

			//Tangents
			if (inputMesh->HasTangentsAndBitangents())
			{
				std::cout << "Reading tangents...\n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];
					vertex.tangent.x = inputMesh->mTangents[i].x;
					vertex.tangent.y = inputMesh->mTangents[i].y;
					vertex.tangent.z = inputMesh->mTangents[i].z;

					model.GetMeshes()[meshIndex].GetVertex(i).tangent = vertex.tangent;
				}
			}

			//UV
			if (inputMesh->HasTextureCoords(0))
			{
				std::cout << "Reading texture coordinates...\n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];
					vertex.uv.x = inputMesh->mTextureCoords[0][i].x;
					vertex.uv.y = inputMesh->mTextureCoords[0][i].y;

					model.GetMeshes()[meshIndex].GetVertex(i).uv = vertex.uv;
				}
			}

			std::cout << "Reading indices...\n";

			auto indices = std::make_unique<uint32_t[]>(inputMesh->mNumFaces * 3);

			for (uint32_t face = 0, index = 0; face < inputMesh->mNumFaces; ++face, index += 3)
			{
				indices[index] = inputMesh->mFaces[face].mIndices[0];
				indices[index + 1] = inputMesh->mFaces[face].mIndices[1];
				indices[index + 2] = inputMesh->mFaces[face].mIndices[2];

				model.GetMeshes()[meshIndex].GetIndex(index) = indices[index];
				model.GetMeshes()[meshIndex].GetIndex(index + 1) = indices[index + 1];
				model.GetMeshes()[meshIndex].GetIndex(index + 2) = indices[index + 2];
			}

			//Normals
			if (inputMesh->HasNormals())
			{
				std::cout << "Reading normals...\n";

				for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
				{
					auto& vertex = vertices[i];
					vertex.normal.x = inputMesh->mNormals[i].x;
					vertex.normal.y = inputMesh->mNormals[i].y;
					vertex.normal.z = inputMesh->mNormals[i].z;

					model.GetMeshes()[meshIndex].GetVertex(i).normal = vertex.normal;
				}
			}

			model.GetMaterialIndices().emplace_back(inputMesh->mMaterialIndex);

			//Bones
			if (inputMesh->HasBones())
			{
				std::vector<uint32_t> numWeightsPerVertex;
				numWeightsPerVertex.resize(inputMesh->mNumVertices, 0);

				for (uint32_t b{ 0 }; b < inputMesh->mNumBones; ++b)
				{
					aiBone* bone{ inputMesh->mBones[b] };
					uint32_t boneIndex{ GetBoneIndex(bone, model) };
					for (uint32_t w{ 0 }; w < bone->mNumWeights; ++w)
					{
						auto& vertexWeight{ bone->mWeights[w] };
						uint32_t boneVertexID{ vertexWeight.mVertexId };

						model.GetMeshes()[meshIndex].GetVertex(boneVertexID).boneWeight[numWeightsPerVertex[boneVertexID]] = vertexWeight.mWeight;
						model.GetMeshes()[meshIndex].GetVertex(boneVertexID).boneIndex[numWeightsPerVertex[boneVertexID]] = boneIndex;

						numWeightsPerVertex[boneVertexID]++;
					}
				}
			}

			//int textureCount = inputMaterial->GetTextureCount(aiTextureType_NORMALS);
			//if (inputMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
			//{
			//	aiString texturePath;
			//	if (inputMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
			//	{
			//		std::string path = texturePath.data;
			//		uint32_t found = path.find_last_of("\\");
			//		found = path.find_last_of("/");
			//		path = path.substr(found + 1);
			//
			//		normalMaps.push_back({ meshIndex, path });
			//	}
			//}
			//
			//int textureCount = inputMaterial->GetTextureCount(aiTextureType_SPECULAR);
			//if (inputMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
			//{
			//	aiString texturePath;
			//	if (inputMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
			//	{
			//		std::string path = texturePath.data;
			//		uint32_t found = path.find_last_of("\\");
			//		found = path.find_last_of("/");
			//		path = path.substr(found + 1);
			//
			//		specularMaps.push_back({ meshIndex, path });
			//	}
			//}
		}
	}

	//Read material data
	if (strcmp(params.writeType, "-animation") != 0)
	{
		if (scene->HasMaterials())
		{
			std::cout << "Reading material...\n";

			model.GetMaterials() = new WOWGE::Graphics::Material[scene->mNumMaterials];
			model.GetNumberOfMaterials() = scene->mNumMaterials;

			uint32_t diffuseTextureIndex{ 0 };
			uint32_t normalTextureIndex{ 0 };
			uint32_t specularTextureIndex{ 0 };
			uint32_t displacementTextureIndex{ 0 };

			for (uint32_t materialIndex{ 0 }; materialIndex < scene->mNumMaterials; ++materialIndex)
			{
				aiMaterial* inputMaterial = scene->mMaterials[materialIndex];
				aiColor4D color{ 0.0f, 0.0f, 0.0f, 0.0f };
				float power{ 0.0f };

				inputMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
				model.GetMaterials()[materialIndex].ambient = { color.r, color.g, color.b, color.a };

				inputMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				model.GetMaterials()[materialIndex].diffuse = { color.r, color.g, color.b, color.a };

				inputMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
				model.GetMaterials()[materialIndex].specular = { color.r, color.g, color.b, color.a };

				inputMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
				model.GetMaterials()[materialIndex].emissive = { color.r, color.g, color.b, color.a };

				inputMaterial->Get(AI_MATKEY_SHININESS, power);
				model.GetMaterials()[materialIndex].power = power;

				std::cout << "Reading Texture...\n";
				aiString texturePath;
				if (inputMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
				{
					if (texturePath.C_Str()[0] == '*')
					{
						std::string fileName = params.inputFileName;
						uint32_t found = fileName.find_last_of("\\");
						fileName = fileName.substr(found + 1);

						found = fileName.find_last_of("/");
						fileName = fileName.substr(found + 1);

						fileName.erase(fileName.length() - 4);
						fileName += "_diffuse_";
						fileName += std::to_string(diffuseTextureIndex++);
						//fileName += texturePath.C_Str()[1];
						//TODO: Create a counter to count the index instrad of parsing

						ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

						int textureIndex{ atoi(texturePath.C_Str() + 1) };
						ASSERT(textureIndex < static_cast<int>(scene->mNumTextures), "Error: Invalid Texture index!");

						const aiTexture* texture{ scene->mTextures[textureIndex] };
						ASSERT(texture->mHeight == 0, "Error: Uncompressed texture found");

						fileName += ".";
						fileName += texture->achFormatHint;

						std::string saveFileLocation{ params.inputFileName };
						found = saveFileLocation.find_last_of("\\");
						saveFileLocation = saveFileLocation.substr(0, found + 1);

						found = saveFileLocation.find_last_of("/");
						if (found < saveFileLocation.size())
						{
							saveFileLocation = saveFileLocation.substr(0, found + 1);
						}
						saveFileLocation += fileName;

						FILE* file{ nullptr };

						fopen_s(&file, saveFileLocation.c_str(), "wb");
						size_t written{ fwrite(texture->pcData, 1, texture->mWidth, file) };
						ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
						fclose(file);

						model.GetDiffuseTexturePaths()[materialIndex] = fileName;
					}
					else
					{
						std::string path = texturePath.data;

						uint32_t found = path.find_last_of("\\");
						path = path.substr(found + 1);

						found = path.find_last_of("/");
						path = path.substr(found + 1);

						model.GetDiffuseTexturePaths()[materialIndex] = path;
					}
				}

				if (inputMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
				{
					if (texturePath.C_Str()[0] == '*')
					{
						std::string fileName = params.inputFileName;
						uint32_t found = fileName.find_last_of("\\");
						fileName = fileName.substr(found + 1);

						found = fileName.find_last_of("/");
						fileName = fileName.substr(found + 1);

						fileName.erase(fileName.length() - 4);
						fileName += "_normal_";
						fileName += std::to_string(normalTextureIndex++);
						//fileName += texturePath.C_Str()[1];

						ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

						int textureIndex{ atoi(texturePath.C_Str() + 1) };
						ASSERT(textureIndex < static_cast<int>(scene->mNumTextures), "Error: Invalid Texture index!");

						const aiTexture* texture{ scene->mTextures[textureIndex] };
						ASSERT(texture->mHeight == 0, "Error: Uncompressed texture found");

						fileName += ".";
						fileName += texture->achFormatHint;

						std::string saveFileLocation{ params.inputFileName };
						found = saveFileLocation.find_last_of("\\");
						saveFileLocation = saveFileLocation.substr(0, found + 1);

						found = saveFileLocation.find_last_of("/");
						if (found < saveFileLocation.size())
						{
							saveFileLocation = saveFileLocation.substr(0, found + 1);
						}
						saveFileLocation += fileName;

						FILE* file{ nullptr };

						fopen_s(&file, saveFileLocation.c_str(), "wb");
						size_t written{ fwrite(texture->pcData, 1, texture->mWidth, file) };
						ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
						fclose(file);

						model.GetNormalTexturePaths()[materialIndex] = fileName;
					}
					else
					{
						std::string path = texturePath.data;

						uint32_t found = path.find_last_of("\\");
						path = path.substr(found + 1);

						found = path.find_last_of("/");
						path = path.substr(found + 1);

						model.GetNormalTexturePaths()[materialIndex] = path;
					}
				}

				if (inputMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
				{
					if (texturePath.C_Str()[0] == '*')
					{
						std::string fileName = params.inputFileName;
						uint32_t found = fileName.find_last_of("\\");
						fileName = fileName.substr(found + 1);

						found = fileName.find_last_of("/");
						fileName = fileName.substr(found + 1);

						fileName.erase(fileName.length() - 4);
						fileName += "_specular_";
						fileName += std::to_string(specularTextureIndex++);
						//fileName += texturePath.C_Str()[1];

						ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

						int textureIndex{ atoi(texturePath.C_Str() + 1) };
						ASSERT(textureIndex < static_cast<int>(scene->mNumTextures), "Error: Invalid Texture index!");

						const aiTexture* texture{ scene->mTextures[textureIndex] };
						ASSERT(texture->mHeight == 0, "Error: Uncompressed texture found");

						fileName += ".";
						fileName += texture->achFormatHint;

						std::string saveFileLocation{ params.inputFileName };
						found = saveFileLocation.find_last_of("\\");
						saveFileLocation = saveFileLocation.substr(0, found + 1);

						found = saveFileLocation.find_last_of("/");
						if (found < saveFileLocation.size())
						{
							saveFileLocation = saveFileLocation.substr(0, found + 1);
						}
						saveFileLocation += fileName;

						FILE* file{ nullptr };

						fopen_s(&file, saveFileLocation.c_str(), "wb");
						size_t written{ fwrite(texture->pcData, 1, texture->mWidth, file) };
						ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
						fclose(file);

						model.GetSpecularTexturePaths()[materialIndex] = fileName;
					}
					else
					{
						std::string path = texturePath.data;

						uint32_t found = path.find_last_of("\\");
						path = path.substr(found + 1);

						found = path.find_last_of("/");
						path = path.substr(found + 1);

						model.GetSpecularTexturePaths()[materialIndex] = path;
					}
				}

				if (inputMaterial->GetTexture(aiTextureType_DISPLACEMENT, 0, &texturePath) == AI_SUCCESS)
				{
					if (texturePath.C_Str()[0] == '*')
					{
						std::string fileName = params.inputFileName;
						uint32_t found = fileName.find_last_of("\\");
						fileName = fileName.substr(found + 1);

						found = fileName.find_last_of("/");
						fileName = fileName.substr(found + 1);

						fileName.erase(fileName.length() - 4);
						fileName += "_displacement_";
						fileName += std::to_string(displacementTextureIndex++);
						//fileName += texturePath.C_Str()[1];

						ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

						int textureIndex{ atoi(texturePath.C_Str() + 1) };
						ASSERT(textureIndex < static_cast<int>(scene->mNumTextures), "Error: Invalid Texture index!");

						const aiTexture* texture{ scene->mTextures[textureIndex] };
						ASSERT(texture->mHeight == 0, "Error: Uncompressed texture found");

						fileName += ".";
						fileName += texture->achFormatHint;

						std::string saveFileLocation{ params.inputFileName };
						found = saveFileLocation.find_last_of("\\");
						saveFileLocation = saveFileLocation.substr(0, found + 1);

						found = saveFileLocation.find_last_of("/");
						if (found < saveFileLocation.size())
						{
							saveFileLocation = saveFileLocation.substr(0, found + 1);
						}
						saveFileLocation += fileName;

						FILE* file{ nullptr };

						fopen_s(&file, saveFileLocation.c_str(), "wb");
						size_t written{ fwrite(texture->pcData, 1, texture->mWidth, file) };
						ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
						fclose(file);

						model.GetDisplacementTexturePaths()[materialIndex] = fileName;
					}
					else
					{
						std::string path = texturePath.data;

						uint32_t found = path.find_last_of("\\");
						path = path.substr(found + 1);

						found = path.find_last_of("/");
						path = path.substr(found + 1);

						model.GetDisplacementTexturePaths()[materialIndex] = path;
					}
				}
			}
		}
	}

	model.GetRootBone() = BuildSkeleton(*scene->mRootNode, nullptr, model);

	if (scene->HasAnimations())
	{
		aiAnimation** animations{ scene->mAnimations };
		animationClips.resize(scene->mNumAnimations);


		for (uint32_t animationIndex{ 0 }; animationIndex < scene->mNumAnimations; ++animationIndex)
		{
			animationClips[animationIndex].boneAnimations.resize(boneIndexMap.size(), nullptr);

			animationClips[animationIndex].name = animations[animationIndex]->mName.C_Str();
			animationClips[animationIndex].duration = static_cast<float>(animations[animationIndex]->mDuration);
			animationClips[animationIndex].ticksPerSecond = static_cast<float>(animations[animationIndex]->mTicksPerSecond);

			animationClips[animationIndex].totalNumberOfBones = boneIndexMap.size();
			animationClips[animationIndex].numberOfBonesWithAnimations = animations[animationIndex]->mNumChannels;

			for (uint32_t channelIndex{ 0 }; channelIndex < animations[animationIndex]->mNumChannels; ++channelIndex)
			{
				aiNodeAnim** channel{ animations[animationIndex]->mChannels };
				auto it{ boneIndexMap.find(channel[channelIndex]->mNodeName.C_Str()) };

				WOWGE::Graphics::BoneAnimation* boneAnimation{ new WOWGE::Graphics::BoneAnimation() };
				boneAnimation->boneIndex = it->second;

				for (uint32_t i{ 0 }; i < channel[channelIndex]->mNumPositionKeys; ++i)
				{
					WOWGE::Graphics::Keyframe<WOWGE::Maths::Vector3> positionKeyFrame;

					positionKeyFrame.key = { channel[channelIndex]->mPositionKeys[i].mValue.x, channel[channelIndex]->mPositionKeys[i].mValue.y, channel[channelIndex]->mPositionKeys[i].mValue.z };
					positionKeyFrame.time = static_cast<float>(channel[channelIndex]->mPositionKeys[i].mTime);

					boneAnimation->AddPositionKeyFrame(positionKeyFrame);
				}

				for (uint32_t i{ 0 }; i < channel[channelIndex]->mNumRotationKeys; ++i)
				{
					WOWGE::Graphics::Keyframe<WOWGE::Maths::Quaternion> rotationKeyFrame;

					rotationKeyFrame.key = { channel[channelIndex]->mRotationKeys[i].mValue.x, channel[channelIndex]->mRotationKeys[i].mValue.y, channel[channelIndex]->mRotationKeys[i].mValue.z , channel[channelIndex]->mRotationKeys[i].mValue.w };
					rotationKeyFrame.time = static_cast<float>(channel[channelIndex]->mRotationKeys[i].mTime);

					boneAnimation->AddRotationKeyFrame(rotationKeyFrame);
				}

				for (uint32_t i{ 0 }; i < channel[channelIndex]->mNumScalingKeys; ++i)
				{
					WOWGE::Graphics::Keyframe<WOWGE::Maths::Vector3> scaleKeyFrame;

					scaleKeyFrame.key = { channel[channelIndex]->mScalingKeys[i].mValue.x, channel[channelIndex]->mScalingKeys[i].mValue.y, channel[channelIndex]->mScalingKeys[i].mValue.z };
					scaleKeyFrame.time = static_cast<float>(channel[channelIndex]->mScalingKeys[i].mTime);

					boneAnimation->AddScaleKeyFrame(scaleKeyFrame);
				}

				animationClips[animationIndex].boneAnimations[boneAnimation->boneIndex] = boneAnimation;
			}
		}
	}

	model.GetNumberOfDiffuseTextures() = model.GetDiffuseTexturePaths().size();
	model.GetNumberOfNormalTextures() = model.GetNormalTexturePaths().size();
	model.GetNumberOfSpecularTextures() = model.GetSpecularTexturePaths().size();
	model.GetNumberOfDisplacementTextures() = model.GetDisplacementTexturePaths().size();

	model.GetNumberOfBones() = static_cast<uint32_t>(model.GetBones().size());

	if (strcmp(params.writeType, "-binary") == 0)
	{
		std::ofstream outputFile;
		outputFile.open(params.outputFileName, std::ios::out | std::ios::binary);

		model.Serialize(outputFile);

		outputFile.close();
	}
	else if (strcmp(params.writeType, "-animation") == 0)
	{
		std::string saveFileLocation{ params.inputFileName };
		uint32_t found{ saveFileLocation.find_last_of("\\") };

		saveFileLocation = saveFileLocation.substr(0, found + 1);

		found = saveFileLocation.find_last_of("/");
		if (found < saveFileLocation.size())
		{
			saveFileLocation = saveFileLocation.substr(0, found + 1);
		}

		for (uint32_t i{ 0 }; i < animationClips.size(); ++i)
		{
			saveFileLocation += animationClips[i].name;
			saveFileLocation += ".wowanim";
			std::ofstream outputFile;
			outputFile.open(saveFileLocation, std::ios::out | std::ios::binary);

			animationClips[i].Serialize(outputFile, i);

			outputFile.close();
		}
	}


	for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
	{
		model.GetMeshes()[i].Destroy();
	}

	SafeDeleteArray(model.GetMeshes());
	SafeDeleteArray(model.GetMaterials());
}

int main(int argc, char* argv[])
{
	try
	{
		const Params params = ParseArg(argc, argv);

		if (strcmp(params.writeType, "-ascii") == 0)
		{
			ImportModelAscii(params);
		}
		else if (strcmp(params.writeType, "-binary") == 0 || strcmp(params.writeType, "-animation") == 0)
		{
			ImportModelBinary(params);
		}
	}
	catch (std::exception e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}

	system("pause");
	return 0;
}