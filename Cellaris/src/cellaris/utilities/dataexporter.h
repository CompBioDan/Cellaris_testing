#ifndef DATAEXPORTER_H_
#define DATAEXPORTER_H_

#include <iostream>
#include <iomanip>
#include <iosfwd>
#include <fstream>
#include "../cell/cell.h"
#include "../cell/cellpopulation.h"


//namespace Cellaris {

	//class Cell;

	void allCellDataWriter(Cell* cell, const char* outputFilename);

//}

#endif /* DATAEXPORTER_H_ */