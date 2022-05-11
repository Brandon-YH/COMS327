#include <cstdlib>
#include <algorithm>

#include "pokemon.h"

Pokemon::Pokemon(int level) : level(level)
{
    int i = 0;
    std::vector<moves_db> move_set;

    pokemon = pokemon_list[rand() % (sizeof(pokemon_list) / sizeof(pokemon_list[0]))];
    pokemon_index = pokemon.id;
    pokemon_species_index = pokemon.species_id;
    shiny = ((rand() % 8192) ? false : true);
    gender = ((rand() % 2) ? gender_female : gender_male);
    // dis = abs(Bworld.cur_idx[dim_y] - center_y) + abs(Bworld.cur_idx[dim_x] - center_x);
    IV = (rand() % 31) - 15;

    while (i < (int)(sizeof(pokemon_moves_list) / sizeof(pokemon_moves_list[0])) && pokemon_moves_list[i].pokemon_id <= pokemon.species_id)
    {
        if (pokemon_moves_list[i].pokemon_id == pokemon.species_id && pokemon_moves_list[i].level >= level / 2 && pokemon_moves_list[i].level <= level && pokemon_moves_list[i].pokemon_move_method_id == 1
            // && pokemon_moves_list[i].version_group_id == 19
        )
        {
            int temp = 0, j, max;
            while (pokemon_moves_list[i].pokemon_id == pokemon.species_id && pokemon_moves_list[i].move_id != move_list[temp].id)
                temp++;

            if ((int)move_set.size() > 0)
            {
                max = move_set.size();
                for (j = 0; j < max; j++)
                {
                    if (move_set[j].identifier == move_list[temp].identifier)
                        break;

                    if (j == (int)move_set.size() - 1 && move_set[j].identifier != move_list[temp].identifier)
                        move_set.push_back(move_list[temp]);
                }
            }
            else
                move_set.push_back(move_list[temp]);
        }
        i++;
    }
    if (move_set.size() <= 1)
    {
        i = 0;
        while (i < (int)(sizeof(pokemon_moves_list) / sizeof(pokemon_moves_list[0])) && pokemon_moves_list[i].pokemon_id <= pokemon.species_id)
        {
            if (pokemon_moves_list[i].pokemon_id == pokemon.species_id && pokemon_moves_list[i].level <= level && pokemon_moves_list[i].pokemon_move_method_id == 1
                // && pokemon_moves_list[i].version_group_id == 19
            )
            {
                int temp = 0, j, max;
                while (pokemon_moves_list[i].pokemon_id == pokemon.species_id && pokemon_moves_list[i].move_id != move_list[temp].id)
                    temp++;

                if ((int)move_set.size() > 0)
                {
                    max = move_set.size();
                    for (j = 0; j < max; j++)
                    {
                        if (move_set[j].identifier == move_list[temp].identifier)
                            break;

                        if (j == (int)move_set.size() - 1 && move_set[j].identifier != move_list[temp].identifier && move_list[temp].id != -1)
                            move_set.push_back(move_list[temp]);
                    }
                }
                else
                    move_set.push_back(move_list[temp]);
            }
            i++;
        }
    }

    while (move_set.size() > 2)
    {
        move_set.erase(move_set.begin() + (rand() % move_set.size()));
    }
    if (rand() % (101 - level) > level && move_set.size() > 1)
        move_set.erase(move_set.begin() + 1);

    move_index[0] = move_index[1] = move_index[2] = move_index[3] = -1;

    for (i = 0; i < (int)move_set.size(); i++)
    {
        move_index[i] = move_set[i].id;
    }
    // complete move selection

    i = 0;
    while (i < (int)(sizeof(stats_list) / sizeof(stats_list[0])) && stats_list[i].pokemon_id <= pokemon.species_id)
    {
        if (stats_list[i].pokemon_id == pokemon.species_id)
        {
            switch (stats_list[i].stat_id)
            {
            case 1:
                effective_stat[stat_currHp] = effective_stat[stat_hp] = (int)((stats_list[i].base_stat + IV) * 2 * level) / 100.0 + level + 10;
                break;
            case 2:
                effective_stat[stat_atk] = (int)((stats_list[i].base_stat + IV) * 2 * level) / 100.0 + 5;
                break;
            case 3:
                effective_stat[stat_def] = (int)((stats_list[i].base_stat + IV) * 2 * level) / 100.0 + 5;
                break;
            case 4:
                effective_stat[stat_spatk] = (int)((stats_list[i].base_stat + IV) * 2 * level) / 100.0 + 5;
                break;
            case 5:
                effective_stat[stat_spdef] = (int)((stats_list[i].base_stat + IV) * 2 * level) / 100.0 + 5;
                break;
            case 6:
                effective_stat[stat_speed] = (int)((stats_list[i].base_stat + IV) * 2 * level) / 100.0 + 5;
                break;
            // case 7:
            //     accuracy = ((stats_list[i].base_stat + IV) * 2 * level) / 100 + 5;
            //     break;
            // case 8:
            //     evasion = ((stats_list[i].base_stat + IV) * 2 * level) / 100 + 5;
            //     break;
            default:
                break;
            }
        }

        i++;
    }
}

Pokemon_db Pokemon::get_pokemon() const
{
    return pokemon;
}

int Pokemon::get_pokemon_index() const
{
    return pokemon_index;
}

const char *Pokemon::get_species() const
{
    return species_list[pokemon_species_index].identifier.data();
}

int Pokemon::get_level() const
{
    return level;
}

int Pokemon::get_currHP() const
{
    return effective_stat[stat_currHp];
}

int Pokemon::get_maxHP() const
{
    return effective_stat[stat_hp];
}

int Pokemon::get_atk() const
{
    return effective_stat[stat_atk];
}

int Pokemon::get_def() const
{
    return effective_stat[stat_def];
}

int Pokemon::get_spatk() const
{
    return effective_stat[stat_spatk];
}

int Pokemon::get_spdef() const
{
    return effective_stat[stat_spdef];
}

int Pokemon::get_speed() const
{
    return effective_stat[stat_speed];
}

const char *Pokemon::get_gender_string() const
{
    return gender == gender_female ? "female" : "male";
}

bool Pokemon::is_shiny() const
{
    return shiny;
}

bool Pokemon::is_same_type(const moves_db *move) const
{
    int i = 0;
    while (pokemon_type_list[i].pokemon_id != pokemon_index)
        i++;

    while (pokemon_type_list[i].pokemon_id == pokemon_index){
        if (pokemon_type_list[i].type_id == move->type_id)
            return true;
        i++;
    }
    return false;
}

const moves_db *Pokemon::get_move(int i) const
{
    if (i < 4 && move_index[i] != -1)
    {
        return &move_list[move_index[i]];
    }
    else
    {
        return NULL;
    }
}

int Pokemon::get_num_moves() const
{
    int i = 0;
    while (get_move(i) != NULL)
        i++;
    return i;
}

void Pokemon::set_currHP(int newHP)
{
    if (newHP <= 0)
        effective_stat[stat_currHp] = 0;
    else if (newHP >= effective_stat[stat_hp])
        effective_stat[stat_currHp] = effective_stat[stat_hp];
    else
        effective_stat[stat_currHp] = newHP;
}
