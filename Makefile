CC=g++
CFLAGS=-std=gnu++98 -g -I. 

obj/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

numerics: obj/main.o obj/Parsers.o obj/FunctionProperties.o obj/ParserUtilities.o obj/FunctionExpression.o obj/Makers.o obj/Utilities.o obj/Point.o obj/Integrator.o obj/Optimizer.o obj/CalculusUtilities.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f obj/*.o *~ numerics 
