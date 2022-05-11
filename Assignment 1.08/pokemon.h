#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include "data.h"

enum pokemon_stat
{
    stat_currHp,
    stat_hp,
    stat_atk,
    stat_def,
    stat_spatk,
    stat_spdef,
    stat_speed,
    numstats
};

enum pokemon_gender
{
    gender_female,
    gender_male
};

class Pokemon
{
private:
    int level;
    int pokemon_index;
    Pokemon_db pokemon;
    int move_index[4];
    int pokemon_species_index;
    int IV;
    int effective_stat[numstats];
    bool shiny;
    pokemon_gender gender;

public:
    Pokemon(int level);
    Pokemon_db get_pokemon() const;
    int get_pokemon_index() const;
    const char *get_species() const;
    int get_level() const;
    int get_currHP() const;
    int get_maxHP() const;
    int get_atk() const;
    int get_def() const;
    int get_spatk() const;
    int get_spdef() const;
    int get_speed() const;
    const char *get_gender_string() const;
    bool is_shiny() const;
    bool is_same_type(const moves_db *move) const;
    const moves_db *get_move(int i) const;
    int get_num_moves() const;


    void set_currHP(int newHP);
};

#endif
