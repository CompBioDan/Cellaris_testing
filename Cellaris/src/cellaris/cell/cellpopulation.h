#ifndef CELLPOPULATION_H_
#define CELLPOPULATION_H_

#include "cell.h"
#include <list>

class Cell;

class CellPopulation {

protected:

	std::list<Cell*> all_cells;


public:

	CellPopulation(std::vector<Cell*>& all_cells);

	void InitialiseCells();

	std::list<Cell*>& get_cells();

	//virtual Cell* AddCell(Cell* pNewCell, Cell* pParentCell = Cell * ()) = 0;

	class Iterator;

	//virtual void Update(bool hasBirthsOrDeaths = true) = 0;

	unsigned get_number_all_cells();

	//virtual void OpenWritersFiles(OutputFileHandler& rOutputFileHandler);

	//void ClosWritersFiles();

	//virtual void WriteResultsToFiles(const std::string& rDirectory);


	class Iterator
	{
	public:

		inline Cell* operator*();

		inline Cell* operator->();

		inline bool operator!=(const typename CellPopulation::Iterator& rOther);

		inline Iterator& operator++();

		inline Iterator(CellPopulation& rCellPopulation, std::list<Cell*>::iterator cellIter);

		virtual ~Iterator()
		{}

	private:

		virtual inline bool IsRealCell();

		inline bool IsAtEnd();

		CellPopulation& mrCellPopulation;

		std::list<Cell*>::iterator mCellIter;


	};

	inline Iterator Begin();

	inline Iterator End();
};




Cell* CellPopulation::Iterator::operator*()
{
	return *mCellIter;
}


Cell* CellPopulation::Iterator::operator->()
{
	return *mCellIter;
}


bool CellPopulation::Iterator::operator!=(const typename CellPopulation::Iterator& rOther)
{
	return mCellIter != rOther.mCellIter;
}

typename CellPopulation::Iterator& CellPopulation::Iterator::operator++()
{
	do
	{
		++mCellIter;
	} while (!IsAtEnd() && !IsRealCell());

	return (*this);
}


bool CellPopulation::Iterator::IsRealCell()
{
	//return !(mrCellPopulation.IsCellAssociatedWithADeletedLocation(*mCellIter) || (*this)->IsDead());
	return 0;
}


bool CellPopulation::Iterator::IsAtEnd()
{
	return mCellIter == mrCellPopulation.get_cells().end();
}


CellPopulation::Iterator::Iterator(CellPopulation& rCellPopulation, std::list<Cell*>::iterator cellIter)
	: mrCellPopulation(rCellPopulation),
	mCellIter(cellIter)
{
	// The cell population can now return empty if it only has ghost nodes
	if (mrCellPopulation.get_cells().empty())
	{
		mCellIter = mrCellPopulation.get_cells().end();
	}
	else
	{
		// Make sure we start at a real cell
		if (mCellIter == mrCellPopulation.get_cells().begin() && !IsRealCell())
		{
			++(*this);
		}
	}
}


typename CellPopulation::Iterator CellPopulation::Begin()
{
	return Iterator(*this, this->all_cells.begin());
}


typename CellPopulation::Iterator CellPopulation::End()
{
	return Iterator(*this, this->all_cells.end());
}


#endif /* CELLPOPULATION_H_ */