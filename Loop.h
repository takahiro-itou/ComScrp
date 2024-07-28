//////////////////////////////////////////////////////////////////////
//																	//
//		Loop.h			CLoop�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F���[�v�n�R�}���h�ŕK�v�ȃ��X�g�̊Ǘ�			//
//				Copyright Takahiro Ito, 2003/09/01 - 2003/09/01		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_LOOP_H__
#define __COMSCRP_INCLUDE_LOOP_H__

//
//	CLoop�N���X
//
class CLoop {
public:
	CLoop();
	virtual ~CLoop();

	//�@���X�g������������
	int InitWriteList();

	//�@���X�g�Ƀf�[�^�̏������݈ʒu��ǉ�����
	int AddBreakPointList(int *p);
	int AddContinuePointList(int *p);

	//�@�W�����v��̃A�h���X���m�肵����A���X�g�ɂ���ʒu�ɏ�������
	int WriteBreakPoint(int value);
	int WriteContinuePoint(int value);

protected:
	int 
	int *m_IfFalsePoint;			//�����s�������̃W�����v����������ވʒu
	int m_ExitPointCount;
	int *m_ExitPoint[MAX_ELSEIF];	//������������ELSE�u���b�N�̃X�L�b�v����������ވʒu

};

#endif
