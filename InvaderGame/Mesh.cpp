#include "Mesh.h"

#include <fbxsdk.h>
#include <iostream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& path)
{
	_vertices.clear();
	_indices.clear();

	// 1. FBX SDKの初期化
	FbxManager* fbxManager = FbxManager::Create();
	if (!fbxManager) return false;

	FbxIOSettings* fbxIOsetting = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxIOsetting);

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (!fbxImporter->Initialize(path.c_str(), -1, fbxManager->GetIOSettings())) {
		fbxManager->Destroy();
		return false;
	}

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "myScene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	// 座標系をDirectX（左手系、Y-Up）に変換
	FbxAxisSystem::DirectX.ConvertScene(fbxScene);
	// 単位をメートルに変換（必要に応じて）
	FbxSystemUnit::m.ConvertScene(fbxScene);

	// 2. ポリゴンを三角形に変換
	FbxGeometryConverter fbxGeomConverter(fbxManager);
	fbxGeomConverter.Triangulate(fbxScene, true);

	// 3. メッシュデータの抽出
	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	if (fbxRootNode) {
		// 再帰的にノードを探索してメッシュを見つける
		auto ProcessNode = [&](auto self, FbxNode* fbxNode) -> void {
			if (fbxNode->GetNodeAttribute() && fbxNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
				FbxMesh* fbxMesh = fbxNode->GetMesh();
				
				int polygonCount = fbxMesh->GetPolygonCount();
				FbxVector4* controlPoints = fbxMesh->GetControlPoints(); // 頂点の位置情報の配列

				for (int j = 0; j < polygonCount; j++) {
					int polygonSize = fbxMesh->GetPolygonSize(j); // ポリゴンの頂点数（3のはず）
					for (int k = 0; k < polygonSize; k++) {
						int controlPointIndex = fbxMesh->GetPolygonVertex(j, k); // メッシュの中の頂点番号

						VertexType3D vertex;
						// 座標
						vertex.Pos.x = (float)controlPoints[controlPointIndex][0];
						vertex.Pos.y = (float)controlPoints[controlPointIndex][1];
						vertex.Pos.z = (float)controlPoints[controlPointIndex][2];

						// 法線
						FbxVector4 normal;
						if (fbxMesh->GetPolygonVertexNormal(j, k, normal)) {
							vertex.Normal.x = (float)normal[0];
							vertex.Normal.y = (float)normal[1];
							vertex.Normal.z = (float)normal[2];
						}
						else {
							vertex.Normal = { 0, 0, 0 };
						}

						// UV
						vertex.UV = { 0, 0 };
						if (fbxMesh->GetElementUVCount() > 0) {
							FbxGeometryElementUV* fbxGeomElemUV = fbxMesh->GetElementUV(0);
							int uvIndex = 0;
							if (fbxGeomElemUV->GetReferenceMode() == FbxGeometryElement::eDirect) {
								uvIndex = controlPointIndex;
							}
							else if (fbxGeomElemUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
								uvIndex = fbxGeomElemUV->GetIndexArray().GetAt(fbxMesh->GetTextureUVIndex(j, k));
							}
							
							FbxVector2 uv = fbxGeomElemUV->GetDirectArray().GetAt(uvIndex);
							vertex.UV.x = (float)uv[0];
							vertex.UV.y = 1.0f - (float)uv[1]; // Vを反転
						}

						_indices.push_back((unsigned int)_vertices.size());
						_vertices.push_back(vertex);
					}
				}
			}

			for (int i = 0; i < fbxNode->GetChildCount(); i++) {
				self(self, fbxNode->GetChild(i));
			}
		};

		ProcessNode(ProcessNode, fbxRootNode);
	}

	fbxManager->Destroy();
	return true;
}