goto label3
#label1
mes �ʏ�́A���̂悤�ȃX�N���v�g�������Ă͂����܂���B
goto label5
#label2
mes �e�X�g�̂��߁A���s�������ł���߂ɕς��Ă��܂��B
goto label4
#label3
mes ����́Agoto���߂̃e�X�g�ł��B
goto label2
#label4
mes ���̂悤�ȃX�N���v�g�͔��ɓǂ݂ɂ����Ȃ�܂�
goto label1
#label5
mes �ł́A���ɁAgosub/return�̐����ł��B
goto label7
#label6
mes Here is Subroutine.
return
#label7
gosub label6
gosub label6
gosub label6
goto label11
#label8
int a
if ( a == 5 ) then
mes a�̒l��5�ł��B
else
mes a�̒l��5�ł͂���܂���B
endif
return
#label11
a = 1
gosub label8
a = 3
gosub label8
a = 5
gosub label8
a = 10
gosub label8

