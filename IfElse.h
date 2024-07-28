//////////////////////////////////////////////////////////////////////
//																	//
//		IfElse.h		CIfElse�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�FIF,ELSE�R�}���h�ŕK�v�ȃ��X�g�̊Ǘ�			//
//				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_IFELSE_H__
#define __COMSCRP_INCLUDE_IFELSE_H__

//IF-ELSEIF-ENDIF��ELSEIF�̋��e��
#define MAX_ELSEIF		64

class CIfElse {
public:
	CIfElse();
	~CIfElse();

	void SetFalsePoint(int *p);
	int WriteFalsePoint(int value);

	void InitExitPointList();
	int AddExitPoint(int *p);
	int WriteExitPoint(int value);

private:
	int *m_IfFalsePoint;			//�����s�������̃W�����v����������ވʒu
	int m_ExitPointCount;
	int *m_ExitPoint[MAX_ELSEIF];	//������������ELSE�u���b�N�̃X�L�b�v����������ވʒu
};

//�@�G���[�ʒm�p
extern int gIfElseErrorInfo;

#define IFELSE_ERR_NOTFINALIZE	1

#endif
