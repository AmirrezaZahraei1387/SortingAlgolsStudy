
EXE_NAME = assign3mzx776

.PHONY:
check: $(EXE_NAME)
	./$(EXE_NAME)

.PHONY:
all: $(EXE_NAME)

.PHONY:
clean:
	rm SortingAlgolStudy.o $(EXE_NAME)

SortingAlgolStudy.o: SortingAlgolStudy.cpp

$(EXE_NAME): SortingAlgolStudy.o
	g++ SortingAlgolStudy.o -o $(EXE_NAME)