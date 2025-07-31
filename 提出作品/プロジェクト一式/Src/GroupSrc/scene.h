//============================================================================
// 
// �V�[���Ǘ��A�w�b�_�t�@�C�� [scene.h]
// Author : ���c����
// 
//============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �V�[���N���X
//****************************************************
class CScene
{
public:

	// <virtual function>
	virtual void Update();		// �X�V����
	virtual void Draw();		// �`�揈��
	virtual	void To_Next() = 0;	// ���̃V�[����

	// <function>
	void Release();	// ���

protected:

	// <special function>
	CScene();			// �R���X�g���N�^
	virtual ~CScene();	// �f�X�g���N�^

	// <virtual function>
	virtual HRESULT Init();		// �����ݒ�
	virtual void	Uninit();	// �I������
};

//****************************************************
// �V�[���Ǘ��N���X
//****************************************************
class CScene_Manager
{
public:

	// <special function>
	CScene_Manager(const CScene_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CScene_Manager& operator=(const CScene_Manager&) = delete;	// �R�s�[������Z�q
	CScene_Manager(CScene_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CScene_Manager& operator=(CScene_Manager&&) = delete;		// ���[�u������Z�q

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <getter>
	CScene* GetScene();	// �V�[�����擾

	// <static getter>
	static CScene_Manager* GetInstance();	// �V�[���}�l�[�W���[���擾

	// <static setter>
	static void ChangeScene(CScene* pScene);	// �V�[���ύX
		
private:

	// <special function>
	CScene_Manager();	// �R���X�g���N�^
	~CScene_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT	Init();		// �����ݒ�
	void	Uninit();	// �I������

	// <data>
	CScene* m_pScene;	// �V�[��

	// <static data>
	static CScene_Manager* m_pSceneManager;	// �V�[���}�l�[�W���[�̖{��
};

#endif // _SCENE_H_