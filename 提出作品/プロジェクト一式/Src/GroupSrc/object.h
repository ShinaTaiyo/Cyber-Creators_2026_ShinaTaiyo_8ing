//============================================================================
// 
// �I�u�W�F�N�g�Ǘ��A�w�b�_�t�@�C�� [object.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "timescale_info.h" // �^�C���X�P�[�����g�p����

//****************************************************
// �I�u�W�F�N�g�N���X
//****************************************************
class CObject
{
public:

	//****************************************************
	// �ÓI�萔���`
	//****************************************************
	static constexpr int MAX_OBJECT = 2048; // ���e����I�u�W�F�N�g�̍ő吔

	//****************************************************
	// �I�u�W�F�N�g�̃��C���[����
	//****************************************************
	enum class LAYER : WORD
	{
		BG = 0,		// �w�i
		DEFAULT,	// �ʏ�
		FRONT,		// �O��
		STENCIL,	// �X�e���V��
		UI,			// UI
		MAX
	};
	//****************************************************
    // �I�u�W�F�N�g���ގ���
    //****************************************************
	enum class OBJTYPE : WORD
	{
		NONE = 0,   // �Ȃ�
		TYPE2D,     // 2D(��������n�߂��Ȃ��̂Ő擪��"TYPE"�Ƃ��Ă���)
		TYPE3D,     // 3D(��������n�߂��Ȃ��̂Ő擪��"TYPE"�Ƃ��Ă���)
		X,          // X
		MAX,        // �ő�
	};

	//****************************************************
	// �I�u�W�F�N�g�̎�ގ���
	//****************************************************
	enum class TYPE : WORD
	{
		NONE = 0,	   // ����
		PLAYER,		   // �v���C���[
		VISIONBLOCKSPAWNER,// ����W�Q�������I�u�W�F�N�g
		BLOCK,		   // �u���b�N
		BGMODEL,       // �w�i���f��
		OBSTACLE,      // ��Q��
		HOLEOBSTACLE,  // ��������Q��
		HOLE,          // ��
		VISIONBLOCKER, // ����W�Q
		ZONE,          // �]�[��
		PARTICLE,      // �p�[�e�B�N��
		TEXTMESH,	   // �e�L�X�g���b�V��
		TEXTURE	,	   // �e�N�X�`��
		NUMBER,        // �i���o�[
		BUTTON,        // �{�^��
		STENCILMASK,   // �X�e���V���}�X�N
		TARGET,        // �^�[�Q�b�g
		TIMER,         // �^�C�}�[
		PARTICLE_2D,   // �p�[�e�B�N��_2D
		ONLYTEXT,	   // �e�L�X�g�̂�
		SPIRIT,        // �u�C�v�̒~��(�V�F�[�_�[)
		TONE,          // �F���ω�(�V�F�[�_�[)
		SUMMONMANAGER, // �����}�l�[�W���[
		NOALLRELEASE,  // �S����ŉ������Ȃ�
		MAX
	};

	// <special function>
	CObject();									// �R���X�g���N�^
	CObject(LAYER Priority = LAYER::DEFAULT);	// �`��D��x�w��R���X�g���N�^
	virtual ~CObject() = 0;						// �f�X�g���N�^

	// <function>
	void SetRelease();	// ����\��ݒ�
	void Release();		// �ʉ������

	// <getter>
	const bool& GetDeath() const; // ���S�t���O���擾

	// <virtual function>
	virtual HRESULT	Init() = 0;		// �����ݒ�
	virtual void	Uninit() = 0;	// �I������
	virtual void	Update() = 0;	// �X�V����
	virtual void	Draw() = 0;		// �`�揈��

	// <getter/setter>
	const TYPE& GetType() const;	   // �^�C�v���擾
	void SetType(TYPE Type);		   // �^�C�v��ݒ�

	// <getter/setter>
	const OBJTYPE& GetObjType() const; // �I�u�W�F�N�g�^�C�v�擾
	void SetObjType(OBJTYPE ObjType);  // �I�u�W�F�N�g���ރ^�C�v�ݒ�

	// <getter/setter>
	const bool& GetRelease() const; // ���S�t���O���擾

	// <getter/setter>
	const bool& GetPause() const; // �|�[�Y�t���O�擾
	void SetPause(bool bPause);   // �|�[�Y�t���O�ݒ�

    // <getter>
	CTimeScale_Info* GetTimeScaleInfo() const;  // �^�C���X�P�[�����擾

	// <getter>
	CObject* GetPrev();	// �O�̃I�u�W�F�N�g�̃|�C���^���擾
	CObject* GetNext();	// ���̃I�u�W�F�N�g�̃|�C���^���擾

	// <static function>
	static void ReleaseAll();		// �S�I�u�W�F�N�g�������
	static void AllSetRelease();    // �S�ẴI�u�W�F�N�g�̎��S�t���O�𔭓����鏈��
	static void UpdateAll();		// �S�I�u�W�F�N�g�X�V����
	static void LateUpdateAll();	// �S�I�u�W�F�N�g��X�V����
	static void DrawAll();			// �S�I�u�W�F�N�g�`�揈��

	// <static getter>
	static CObject* GetTopObject();					// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* GetTopObject(WORD wPriority);	// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* GetTopObject(LAYER Priority);	// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* FindSpecificObject(TYPE Type);	// ����^�C�v�̃I�u�W�F�N�g�T��
	static int		CountSpecificObject(TYPE Type);	// ����^�C�v�̃I�u�W�F�N�g�����擾

private:

	// <data>
	WORD	 m_wPriority;	 // �`��D��x
	CObject* m_pPrev;		 // �O�̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pNext;		 // ���̃I�u�W�F�N�g�̃|�C���^
	TYPE	 m_Type;		 // �^�C�v���� 
	OBJTYPE  m_ObjType;      // �I�u�W�F�N�g���ގ���
	bool	 m_bDeath;		 // ���S�t���O
	bool     m_bPause;       // �|�[�Y�t���O�i�I���Ȃ�X�V�������Ă΂Ȃ��j
	bool     m_bIsSummonManagerObj; // �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ���
	CTimeScale_Info* m_pTimeScaleInfo; // �^�C���X�P�[�����

	// <static data>
	static int		m_nNumAll;								// �I�u�W�F�N�g����
	static CObject* m_pTop[static_cast<WORD>(LAYER::MAX)];	// �擪�I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[static_cast<WORD>(LAYER::MAX)];	// �I�[�I�u�W�F�N�g�̃|�C���^
};

#endif // _OBJECT_H_