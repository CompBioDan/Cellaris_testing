#ifndef DATAEXPORTER_H_
#define DATAEXPORTER_H_

#include <iostream>
#include <iomanip>
#include <iosfwd>
#include <fstream>
#include "../cell/cell.h"
#include "../cell/bacteria.h"
#include "../cell/cellpopulation.h"


//namespace Cellaris {

	//class Cell;

	void basicCellDataWriter(Cell* cell, const char* outputFilename);

	void bacteriaCellDataWriter(Bacteria* bac, const char* outputFilename);

//}

#endif /* DATAEXPORTER_H_ */