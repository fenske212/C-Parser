output: parser.o inputbuf.o lexer.o
	g++ parser.o inputbuf.o lexer.o

parser.o: parser.cc
	g++ -c parser.cc

inputbuf.o: inputbuf.cc inputbuf.h
	g++ -c inputbuf.cc

lexer.o: lexer.cc lexer.h
	g++ -c lexer.cc

clean:
	rm *.o
	rm a.out