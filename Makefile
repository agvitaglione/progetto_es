CFLAGS=`pkg-config --cflags --libs gtk+-3.0` -rdynamic -std=c++17 -fpermissive -g
OBJ=PtMain.o PtMonitorControl.o PtMonitorView.o
LIBS=-lslope -lm
DEPS=PtMonitorView.h PtMonitorControl.h

%.o: %.cpp $(DEPS)
	g++ -c $< $(CFLAGS)

PtMain: $(OBJ)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm *.o
	rm PtMain