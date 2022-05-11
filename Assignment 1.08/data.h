#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>
#include <vector>

class experience_db
{
public:
    int growth_rate_id, level, exp;

    experience_db();
    experience_db(int &growth_rate_id, int &level, int &exp);
    // experience(const experience &p);
    //~experience();

    void print();
};

class moves_db
{
public:
    int id, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id;
    std::string identifier;

    moves_db();
    moves_db(int id, std::string &identifier, int &generation_id, int &type_id, int &power, int &pp, int &accuracy, int &priority, int &target_id, int &damage_class_id, int &effect_id, int &effect_chance, int &contest_type_id, int &contest_effect_id, int &super_contest_effect_id);
    // moves(const moves &p);
    //~moves();

    void print();
};

class pokemon_move_db
{
public:
    int pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order;

    pokemon_move_db();
    pokemon_move_db(int &pokemon_id, int &version_group_id, int &move_id, int &pokemon_move_method_id, int &level, int &order);
    // pokemon_move(const pokemon_move &p);
    //~pokemon_move();

    void print();
};

class pokemon_species_db
{
public:
    int id, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
    std::string identifier;

    pokemon_species_db();
    pokemon_species_db(int &id, std::string &identifier, int &generation_id, int &evolves_from_species_id, int &evolution_chain_id, int &color_id, int &shape_id, int &habitat_id, int &gender_rate, int &capture_rate, int &base_happiness, int &is_baby, int &hatch_counter, int &has_gender_differences, int &growth_rate_id, int &forms_switchable, int &is_legendary, int &is_mythical, int &order, int &conquest_order);
    // pokemon_species(const pokemon_species &p);
    //~pokemon_species();

    void print();
};

class pokemon_stats_db
{
public:
    int pokemon_id, stat_id, base_stat, effort;

    pokemon_stats_db();
    pokemon_stats_db(int &pokemon_id, int &stat_id, int &base_stat, int &effort);
    // pokemon_stats(const pokemon_stats &p);
    //~pokemon_stats();

    void print();
};

class Pokemon_db
{
public:
    int id, species_id, height, weight, base_experience, order, is_default;
    std::string identifier;

    Pokemon_db();
    Pokemon_db(const int &id, const std::string &identifier, const int &species_id, const int &height, const int &weight, const int &base_exp, const int &order, const int &is_default);
    // Pokemon(const Pokemon &p);
    //~Pokemon();

    void print();
};

class type_names_db
{
public:
    int type_id, local_language_id;
    std::string name;

    type_names_db();
    type_names_db(int &type_id, std::string &name, int &local_language_id);
    // type_names(const type_names &p);
    //~type_names();

    void print();
};

class pokemon_type_db
{
public:
    int pokemon_id, type_id, slot;

    pokemon_type_db();
    pokemon_type_db(int &pokemon_id, int &type_id, int &slot);
    // pokemon_type_db(const pokemon_type_db &p);
    //~pokemon_type_db();

    void print();
};

extern Pokemon_db pokemon_list[1093];
extern pokemon_move_db pokemon_moves_list[528239];
extern pokemon_stats_db stats_list[6553];
extern pokemon_species_db species_list[899];
extern type_names_db type_names_list[20];
extern experience_db experience_list[601];
extern moves_db move_list[845];
extern pokemon_type_db pokemon_type_list[1676];

void parse(std::string *fileName);
void parseAll();

#endif
// pokemon_move_db pokemon_moves[528239];
// pokemon_db pokemon[1093];
// char *types[19];
// move_db moves[845];
// pokemon_species_db species[899];
// experience_db experience[601];