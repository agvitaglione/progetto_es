CFLAGS=`pkg-config --cflags --libs gtk+-3.0` -rdynamic -std=c++17 -g 
OBJ=PtMain.o PtMonitorControl.o PtMonitorView.o PtMonitorModel.o DataPlotQueue.o
LIBS=-lslope -lm -lrt
DEPS=PtMonitorView.h PtMonitorControl.h PtMonitorModel.h DataPlotQueue.h

%.o: %.cpp $(DEPS)
	@g++ -c $< $(CFLAGS)

PtMain: $(OBJ)
	@g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm *.o
	rm PtMain