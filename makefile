# Batch Number 83
# Suyash Kumar 2014A7PS053P
# Siddharth Agarwal 2014A7PS025P
output: driver.o
	gcc -g  -o stage1exe  driver.o -lm
# lexer.o: lexer.c
# 	gcc -c -g lexer.c
# tablepop.o: tablepop.c
# 	gcc -c -g tablepop.c
# firstfollow.o: firstfollow.c
# 	gcc -c -g firstfollow.c
# parser.o: parser.c
# 	gcc -c -g parser.c
# stack.o: stack.c
# 	gcc -c -g stack.c
# AST.o: AST.c
# 	gcc -c -g AST.c
# actualParsing.o: actualParsing.c
# 	gcc -c -g actualParsing.c
driver.o: driver.c
	gcc -c -g driver.c -lm 
clean:
	rm *.o
	rm driver