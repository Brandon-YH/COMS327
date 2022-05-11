#ifndef CSV_PARSE_H
#define CSV_PARSE_H

#include "data.h"

#include <iterator>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<Pokemon> pokemon_list;
std::vector<pokemon_move> pokemon_moves_list;
std::vector<pokemon_stats> stats_list;
std::vector<pokemon_species> species_list;
std::vector<type_names> type_names_list;
std::vector<experience> experience_list;
std::vector<moves> move_list;

void parse(std::string *fileName);
void parseAll();
#endif