#ifndef CMODELX_H //インクルードガード
#define CMODELX_H

#define MODEL_FILE "sample.blend.x" //入力ファイル名

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a;a=0;}

#define ARRAY_SIZE(a)(sizeof(a)/sizeof(0))



#include <vector>        //vectorクラスのインクルード
#include "CMatrix.h"     //マトリクスクラスのインクルード
class CModelX;           //CModelXクラスの宣言


#include "CVector.h"

#include "CMaterial.h"

class CMaterial;


/*
CSkinWeights
スキンウェイトクラス
*/

class CSkinWeights{
public:
	char *mpFrameName;  //フレーム名
	int mFrameIndex;   //フレーム番号
	int mIndexNum;     //頂点番号数
	int *mpIndex;      //頂点番号配列
	float *mpWeight;   //頂点ウェイト配列
	CMatrix mOffset;   //オフセットマトリックス

	CSkinWeights(CModelX *model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};

//CMeshクラスの定義
class CMesh{
public:
	int mVertexNum;        //頂点数
	CVector *mpVertex;     //頂点データ
	int mFaceNum;          //面数
	int *mpVertexIndex;    //面を構成する頂点番号
	int mNormalNum;        //法線数
	CVector *mpNormal;     //法線データ
	int mMaterialNum;      //マテリアル数
	int mMaterialIndexNum; //マテリアル番号数(面数)
	int *mpMaterialIndex;  //マテリアル番号
	std::vector <CMaterial*> mMaterial; //マテリアルデータ
	//スキンウェイト
	std::vector<CSkinWeights*> mSkinWeights;
	//コンストラクタ
	CMesh()
		: mVertexNum(0)
		, mpVertex(0)
		, mFaceNum(0)
		, mpVertexIndex(nullptr)
		, mNormalNum(0)
		, mpNormal(nullptr)
		, mMaterialNum(0)
		, mMaterialIndexNum(0)
		, mpMaterialIndex(nullptr)
	{}
	//デストラクタ
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		//スキンウェイトの削除
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	//読み込み処理
	void Init(CModelX *model);
	void Render();
};

//CModelXFrameクラスの定義
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild; //子フレームの配列
	CMatrix mTransformMatrix;    //変換行列
	char* mpName;         //フレーム名前
	int mIndex;           //フレーム番号
	CMesh mMesh;          //Meshデータ
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame(){
		//子フレームを全て解放する
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//名前のエリアを解放する
		SAFE_DELETE_ARRAY(mpName);
	}
	void Render();
};

/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/

class CModelX{
public: 
	//整数データの取得
	int GetIntToken();
	char* mpPointer;      //読み込み位置
	char mToken[1024];    //取り出した単語の領域
	std::vector<CModelXFrame*> mFrame; //フレームの配列
	//浮動小数点データの取得
	float GetFloatToken();
	CModelX()
		: mpPointer(nullptr)
	{}

	~CModelX(){
		if (mFrame.size() > 0)
		{
			delete mFrame[0];
		}
	}

	//ファイル読み込み
	void Load(char* file);
	//単語の取り出し
	void GetToken();
	//ノードの読み飛ばし
	void SkipNode();
	void Render();
};




#endif
