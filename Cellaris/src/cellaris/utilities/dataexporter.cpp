#include "dataexporter.h"


//using namespace Cellaris;

/** Output all cell held data. Input: Pointer to cell & output filename */
void basicCellDataWriter(Cell* cell, const std::string outputFilename)
{
	//const char* g_outputFilename = outputFilename;
	const std::string g_outputFilename = outputFilename;
	std::ofstream g_outputFile;

	g_outputFile.open(g_outputFilename, std::ofstream::out | std::ofstream::app);
	g_outputFile << std::fixed << std::setprecision(6);

	SceneTime* p_scene_time = SceneTime::instance();
	g_outputFile << "Time: " << p_scene_time->get_time_steps_elapsed() << std::endl;
	g_outputFile << "CellID: " << cell->get_cell_id() << std::endl;
	g_outputFile << "Cellage: " << cell->get_cell_age() << std::endl;
	g_outputFile << "CellBirthTime: " << cell->get_birth_time() << std::endl;
	/*g_outputFile << "Cell x pos: " << cell->get_cell_position().pos.x << std::endl;
	g_outputFile << "Cell y pos: " << cell->get_cell_position().pos.y << std::endl;
	g_outputFile << "Cell z pos: " << cell->get_cell_position().pos.z << std::endl;*/

	g_outputFile.close();

}

/** Output all cell held data. Input: Pointer to cell & output filename */
void bacteriaCellDataWriter(Bacteria* bac, const char* outputFilename)
{
	const char* g_outputFilename = outputFilename;
	std::ofstream g_outputFile;

	g_outputFile.open(g_outputFilename, std::ofstream::out | std::ofstream::app);
	g_outputFile << std::fixed << std::setprecision(6);

	SceneTime* p_scene_time = SceneTime::instance();
	g_outputFile << "Time: " << p_scene_time->get_time() << std::endl;
	g_outputFile << "CellID: " << bac->get_cell_id() << std::endl;
	g_outputFile << "Cellage: " << bac->get_cell_age() << std::endl;
	g_outputFile << "ParticleCount: " << bac->get_number_particles() << std::endl;
	g_outputFile << "ParticleOffset: " << bac->get_particle_offset() << std::endl;
	g_outputFile << "SpringOffset: " << bac->get_spring_offset() << std::endl;
	g_outputFile << "SpringCount: " << bac->get_number_springs() << std::endl;
	g_outputFile << "CellBirthTime: " << bac->get_birth_time() << std::endl;
	//g_outputFile << "CellPosition: " << bac->get_cell_position().pos.x << std::endl;

	g_outputFile.close();

}

/** Output cell age data.  Input: Pointer to cell & output filename **/
void cellAgeWriter(Cell* cell, const char* outputFilename)
{
	const char* g_outputFilename = outputFilename;
	std::ofstream g_outputFile;

	g_outputFile.open(g_outputFilename, std::ofstream::out | std::ofstream::app);
	g_outputFile << std::fixed << std::setprecision(6);

	SceneTime* p_scene_time = SceneTime::instance();
	g_outputFile << "Time: " << p_scene_time->get_time() << std::endl;
	g_outputFile << "CellID: " << cell->get_cell_id() << std::endl;
	g_outputFile << "Cellage: " << cell->get_cell_age() << std::endl;


	g_outputFile.close();

}

