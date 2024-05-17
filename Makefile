all: exec entrada.txt
	exec < entrada.txt > codigo.txt

lex.yy.c: java.l
	flex java.l

y.tab.c: java.y
	bison -o y.tab.c java.y

exec: lex.yy.c y.tab.c
	g++ -o exec y.tab.c C:\GnuWin32\lib\liby.a -w

clean:
	del lex.yy.c y.tab.c exec.exe codigo.txt