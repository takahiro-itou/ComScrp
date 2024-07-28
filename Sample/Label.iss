goto label3
#label1
mes 通常は、このようなスクリプトを書いてはいけません。
goto label5
#label2
mes テストのため、実行順序をでたらめに変えています。
goto label4
#label3
mes これは、goto命令のテストです。
goto label2
#label4
mes このようなスクリプトは非常に読みにくくなります
goto label1
#label5
mes では、次に、gosub/returnの説明です。
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
mes aの値が5です。
else
mes aの値は5ではありません。
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

