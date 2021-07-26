#ifndef CMODELX_H //�C���N���[�h�K�[�h
#define CMODELX_H

#define MODEL_FILE "sample.blend.x" //���̓t�@�C����

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a;a=0;}

#define ARRAY_SIZE(a)(sizeof(a)/sizeof(0))



#include <vector>        //vector�N���X�̃C���N���[�h
#include "CMatrix.h"     //�}�g���N�X�N���X�̃C���N���[�h
class CModelX;           //CModelX�N���X�̐錾


#include "CVector.h"

#include "CMaterial.h"

class CMaterial;


/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/

class CSkinWeights{
public:
	char *mpFrameName;  //�t���[����
	int mFrameIndex;   //�t���[���ԍ�
	int mIndexNum;     //���_�ԍ���
	int *mpIndex;      //���_�ԍ��z��
	float *mpWeight;   //���_�E�F�C�g�z��
	CMatrix mOffset;   //�I�t�Z�b�g�}�g���b�N�X

	CSkinWeights(CModelX *model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};

/*
CAnimationKey
�A�j���[�V�����L�[�N���X
*/

class CAnimationKey{
public:
	//����
	float mTime;
	//�s��
	CMatrix mMatrix;
};

/*
CAnimation
�A�j���[�V�����N���X
*/
class CAnimation{
public:
	char *mpFrameName; //�t���[����
	int mFrameIndex;   //�t���[���ԍ�
	int mKeyNum;       //�L�[��(���Ԑ�)
	CAnimationKey *mpKey;  //�L�[�̔z��

	CAnimation(CModelX*model);
	~CAnimation(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpKey);
	}
};

/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet{
public:
	//�A�j���[�V�����Z�b�g��
	char *mpName;
	//�A�j���[�V����
	std::vector<CAnimation*>mAnimation;
	float mTime;   //���ݎ���
	float mWeight; //�d��
	float mMaxTime;//�ő厞��

	CAnimationSet(CModelX*model);
	~CAnimationSet(){
		SAFE_DELETE_ARRAY(mpName);
		//�A�j���[�V�����v�f�̍폜
		for (int i = 0; i < mAnimation.size(); i++){
			delete mAnimation[i];
		}
	}
};

/*
animateFrame
�t���[���̕ϊ��s����A�j���[�V�����f�[�^�ōX�V����
*/
void CModelX::AnimateFrame(){
	//�A�j���[�V�����œK�p�����t���[����
	//�ϊ��s����[���N���A����
	for (int i = 0; i < mAnimationSet.size(); i++){
		CAnimationSet*anim = mAnimationSet[i];
		//�d�݂�0�͔�΂�
		if (anim->mWeight == 0)continue;
		//�t���[����(Animation��)�J��Ԃ�
		for (int j = 0; j < anim->mAnimation.size(); j++){
			CAnimatoin*animation = anim->mAnimation[j];
			//�Y������t���[���̕ϊ��s����[���N���A����
			memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix));
		}
	}
	//�A�j���[�V�����ɊY������t���[���̕ϊ��s���
	//�A�j���[�V�����̃f�[�^�Őݒ肷��
	for (int i = 0; i < mAnimationSet.size(); i++){
		CAnimationSet*anim = mAnimationSet[i];
		//�d�݂�0�͔�΂�
		if (anim->mWeight == 0)continue;
		//�t���[����(Animation��)�J��Ԃ�
		for (int j = 0; j < anim->mAnimation.size(); j++){
			//�t���[�����擾����
			CAnimation*animation = anim->mAnimation[j];
			CModelXFrame*frame = mFrame[animation->mFrameIndex];
			//�L�[�������ꍇ�͔�΂�
			if (animation->mpKey == 0)continue;
			//���Ԃ��擾
			float time = anim->mTime;
			//�ŏ��̎��Ԃ�菬�����ꍇ
			if (time < animation->mpKey[0].mTime){
				//�ϊ��s���0�R�}�ڂ̍s��ōX�V
				frame->mTransformMatrix += animation->mpKey[0].mMatrix*anim->mWeight;
			}
			//�Ō�̎��Ԃ��傫���ꍇ
			else if (time >= [animation->mpKey{ animation->mKeyNum - 1].mTime{
				//�ϊ��s����Ō�̃R�}�̍s��ōX�V
				frame->mTransformMAtrix += animation->mpKey[animation->mKeyNum - 1].mMatrix*anim->mWeight;
		}
			else{
				//���Ԃ̓r���̏ꍇ
				for (int k = 1; k < animation->mKeyNum; k++){
					//�ϊ��s���,���`��ԂɂčX�V
					if (time < animation->mpKey[k].mTime&&
						animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime){
						float r = (animation->mpKey[k].mTime - time) /
							(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);
						frame->mTransformMatrix +=
							(animation->mpKey[k - 1].mMatrix*r + animation->mpKey[k - 1].mTime);
						break;
					}
				}
			}
#ifdef _DEBUG
			for (inti = 0; i < mAnimationSet.size(); i++){
				CAnimationSet* anim = CAnimationSet[i];
				//�d�݂�0�͔�΂�
				if (anim->mWeight == 0)continue;
				//�t���[����(Animation��)�J��Ԃ�
				for (int j = 0; j < anim->mAnimation.size(); j++){
					//�t���[�����擾����
					CAnimation* Animation = anim->mAnimation[j];
					CModelXFrame*frame = mFrame[animation->mFrameIndex];
					printf("Frame:%s\n", frame->mpName);
					frame->mTransformMatrix.Print();
				}
			}
#endif

//CMesh�N���X�̒�`
class CMesh{
public:
	int mVertexNum;        //���_��
	CVector *mpVertex;     //���_�f�[�^
	int mFaceNum;          //�ʐ�
	int *mpVertexIndex;    //�ʂ��\�����钸�_�ԍ�
	int mNormalNum;        //�@����
	CVector *mpNormal;     //�@���f�[�^
	int mMaterialNum;      //�}�e���A����
	int mMaterialIndexNum; //�}�e���A���ԍ���(�ʐ�)
	int *mpMaterialIndex;  //�}�e���A���ԍ�
	std::vector <CMaterial*> mMaterial; //�}�e���A���f�[�^
	//�X�L���E�F�C�g
	std::vector<CSkinWeights*> mSkinWeights;
	//�R���X�g���N�^
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
	//�f�X�g���N�^
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		//�X�L���E�F�C�g�̍폜
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	//�ǂݍ��ݏ���
	void Init(CModelX *model);
	void Render();
};

//CModelXFrame�N���X�̒�`
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild; //�q�t���[���̔z��
	CMatrix mTransformMatrix;    //�ϊ��s��
	char* mpName;         //�t���[�����O
	int mIndex;           //�t���[���ԍ�
	CMesh mMesh;          //Mesh�f�[�^
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame(){
		//�q�t���[����S�ĉ������
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//���O�̃G���A���������
		SAFE_DELETE_ARRAY(mpName);
	}
	void Render();
};

/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/

class CModelX{
public: 
	//�����f�[�^�̎擾
	int GetIntToken();
	char* mpPointer;      //�ǂݍ��݈ʒu
	char mToken[1024];    //���o�����P��̗̈�
	std::vector<CModelXFrame*> mFrame; //�t���[���̔z��
	//���������_�f�[�^�̎擾
	float GetFloatToken();
	CModelX()
		: mpPointer(nullptr)
	{}

	~CModelX(){
		if (mFrame.size() > 0)
		{
			delete mFrame[0];
		}
		for (int i = 0; i < mAnimationSet.size(); i++){
			delete mAnimationSet[i];
		}
	}

	//�t�@�C���ǂݍ���
	void Load(char* file);
	//�P��̎��o��
	void GetToken();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
	void Render();
	//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CAnimationSet*>mAnimationSet;
	//�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	CModelXFrame* FindFrame(char* name);
};



#endif
