#ifndef DATA_H
#define DATA_H

#include <string>
#include <iostream>

class experience
{
private:
    int growth_rate_id, level, exp;

public:
    experience();
    experience(int &growth_rate_id, int &level, int &exp);
    // experience(const experience &p);
    //~experience();

    void print();
};

class moves
{
private:
    int id, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id;
    std::string identifier;

public:
    moves();
    moves(int id, std::string &identifier, int &generation_id, int &type_id, int &power, int &pp, int &accuracy, int &priority, int &target_id, int &damage_class_id, int &effect_id, int &effect_chance, int &contest_type_id, int &contest_effect_id, int &super_contest_effect_id);
    // moves(const moves &p);
    //~moves();

    void print();
};

class pokemon_move
{
private:
    int pokemon_id,version_group_id, move_id, pokemon_move_method_id, level, order;

public:
    pokemon_move();
    pokemon_move(int &pokemon_id, int &version_group_id, int &move_id, int &pokemon_move_method_id, int &level, int &order);
    // pokemon_move(const pokemon_move &p);
    //~pokemon_move();

    void print();
};

class pokemon_species
{
private:
    int id, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
    std::string identifier;

public:
    pokemon_species();
    pokemon_species(int &id, std::string &identifier, int &generation_id, int &evolves_from_species_id, int &evolution_chain_id, int &color_id, int &shape_id, int &habitat_id, int &gender_rate, int &capture_rate, int &base_happiness, int &is_baby, int &hatch_counter, int &has_gender_differences, int &growth_rate_id, int &forms_switchable, int &is_legendary, int &is_mythical, int &order, int &conquest_order);
    // pokemon_species(const pokemon_species &p);
    //~pokemon_species();

    void print();
};

class Pokemon
{
private:
    int id, species_id, height, weight, base_experience, order, is_default;;
    std::string identifier;

public:
    Pokemon();
    Pokemon(const int &id, const std::string &identifier, const int &species_id, const int &height, const int &weight, const int &base_exp, const int &order, const int &is_default);
    // Pokemon(const Pokemon &p);
    //~Pokemon();

    void print();
};

class type_names
{
private:
    int type_id,local_language_id;
    std::string name;

public:
    type_names();
    type_names(int &type_id,std::string &name, int &local_language_id);
    // type_names(const type_names &p);
    //~type_names();

    void print();
};

#endif