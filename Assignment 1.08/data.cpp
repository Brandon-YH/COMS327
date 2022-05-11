#include "data.h"
// #include <iostream>
// #include <iterator>
// #include <iostream>
// #include <sstream>
#include <fstream>

Pokemon_db pokemon_list[1093];
pokemon_move_db pokemon_moves_list[528239];
pokemon_stats_db stats_list[6553];
pokemon_species_db species_list[899];
type_names_db type_names_list[20];
experience_db experience_list[601];
moves_db move_list[845];
pokemon_type_db pokemon_type_list[1676];

experience_db::experience_db()
{
    this->growth_rate_id = -1;
    this->level = -1;
    this->exp = -1;
}
experience_db::experience_db(int &growth_rate_id, int &level, int &exp)
{
    this->growth_rate_id = growth_rate_id;
    this->level = level;
    this->exp = exp;
}

void experience_db::print()
{
    std::cout << "growth_rate_id:\t" << (this->growth_rate_id == -1 ? " " : std::to_string(this->growth_rate_id)) << std::endl;
    std::cout << "level:\t\t" << (this->level == -1 ? " " : std::to_string(this->level)) << std::endl;
    std::cout << "experience:\t" << (this->exp == -1 ? " " : std::to_string(this->exp)) << std::endl;
    std::cout << "" << std::endl;
}

moves_db::moves_db()
{
    this->id = -1;
    this->generation_id = -1;
    this->type_id = -1;
    this->power = -1;
    this->pp = -1;
    this->accuracy = -1;
    this->priority = -1;
    this->target_id = -1;
    this->damage_class_id = -1;
    this->effect_id = -1;
    this->effect_chance = -1;
    this->contest_type_id = -1;
    this->contest_effect_id = -1;
    this->super_contest_effect_id = -1;
    this->identifier = "-1";
}
moves_db::moves_db(int id, std::string &identifier, int &generation_id, int &type_id, int &power, int &pp, int &accuracy, int &priority, int &target_id, int &damage_class_id, int &effect_id, int &effect_chance, int &contest_type_id, int &contest_effect_id, int &super_contest_effect_id)
{
    this->id = id;
    this->identifier = identifier;
    this->generation_id = generation_id;
    this->type_id = type_id;
    this->power = power;
    this->pp = pp;
    this->accuracy = accuracy;
    this->priority = priority;
    this->target_id = target_id;
    this->damage_class_id = damage_class_id;
    this->effect_id = effect_id;
    this->effect_chance = effect_chance;
    this->contest_type_id = contest_type_id;
    this->contest_effect_id = contest_effect_id;
    this->super_contest_effect_id = super_contest_effect_id;
}

void moves_db::print()
{
    std::cout << "id:\t\t\t" << (this->id == -1 ? " " : std::to_string(this->id)) << std::endl;
    std::cout << "identifier:\t\t" << (this->identifier == "-1" ? " " : (this->identifier)) << std::endl;
    std::cout << "generation_id:\t\t" << (this->generation_id == -1 ? " " : std::to_string(this->generation_id)) << std::endl;
    std::cout << "type_id:\t\t" << (this->type_id == -1 ? " " : std::to_string(this->type_id)) << std::endl;
    std::cout << "power:\t\t\t" << (this->power == -1 ? " " : std::to_string(this->power)) << std::endl;
    std::cout << "pp:\t\t\t" << (this->pp == -1 ? " " : std::to_string(this->pp)) << std::endl;
    std::cout << "accuracy:\t\t" << (this->accuracy == -1 ? " " : std::to_string(this->accuracy)) << std::endl;
    std::cout << "priority:\t\t" << (this->priority == -1 ? " " : std::to_string(this->priority)) << std::endl;
    std::cout << "target_id:\t\t" << (this->target_id == -1 ? " " : std::to_string(this->target_id)) << std::endl;
    std::cout << "damage_class_id:\t" << (this->damage_class_id == -1 ? " " : std::to_string(this->damage_class_id)) << std::endl;
    std::cout << "effect_id:\t\t" << (this->effect_id == -1 ? " " : std::to_string(this->effect_id)) << std::endl;
    std::cout << "effect_chance:\t\t" << (this->effect_chance == -1 ? " " : std::to_string(this->effect_chance)) << std::endl;
    std::cout << "contest_type_id:\t" << (this->contest_type_id == -1 ? " " : std::to_string(this->contest_type_id)) << std::endl;
    std::cout << "contest_effect_id:\t" << (this->contest_effect_id == -1 ? " " : std::to_string(this->contest_effect_id)) << std::endl;
    std::cout << "super_contest_effect_id: " << (this->super_contest_effect_id == -1 ? " " : std::to_string(this->super_contest_effect_id)) << std::endl;
    std::cout << "" << std::endl;
}

pokemon_move_db::pokemon_move_db()
{
    this->pokemon_id = -1;
    this->version_group_id = -1;
    this->move_id = -1;
    this->pokemon_move_method_id = -1;
    this->level = -1;
    this->order = -1;
}
pokemon_move_db::pokemon_move_db(int &pokemon_id, int &version_group_id, int &move_id, int &pokemon_move_method_id, int &level, int &order)
{
    this->pokemon_id = pokemon_id;
    this->version_group_id = version_group_id;
    this->move_id = move_id;
    this->pokemon_move_method_id = pokemon_move_method_id;
    this->level = level;
    this->order = order;
}

void pokemon_move_db::print()
{
    std::cout << "pokemon_id:\t\t" << (this->pokemon_id == -1 ? " " : std::to_string(this->pokemon_id)) << std::endl;
    std::cout << "version_group_id:\t" << (this->version_group_id == -1 ? " " : std::to_string(this->version_group_id)) << std::endl;
    std::cout << "move_id:\t\t" << (this->move_id == -1 ? " " : std::to_string(this->move_id)) << std::endl;
    std::cout << "pokemon_move_method_id:\t" << (this->pokemon_move_method_id == -1 ? " " : std::to_string(this->pokemon_move_method_id)) << std::endl;
    std::cout << "level:\t\t\t" << (this->level == -1 ? " " : std::to_string(this->level)) << std::endl;
    std::cout << "order:\t\t\t" << (this->order == -1 ? " " : std::to_string(this->order)) << std::endl;
    std::cout << "" << std::endl;
}

pokemon_species_db::pokemon_species_db()
{
    this->id = -1;
    this->identifier = -1;
    this->generation_id = -1;
    this->evolves_from_species_id = -1;
    this->evolution_chain_id = -1;
    this->color_id = -1;
    this->shape_id = -1;
    this->habitat_id = -1;
    this->gender_rate = -1;
    this->capture_rate = -1;
    this->base_happiness = -1;
    this->is_baby = -1;
    this->hatch_counter = -1;
    this->has_gender_differences = -1;
    this->growth_rate_id = -1;
    this->forms_switchable = -1;
    this->is_legendary = -1;
    this->is_mythical = -1;
    this->order = -1;
    this->conquest_order = -1;
}
pokemon_species_db::pokemon_species_db(int &id, std::string &identifier, int &generation_id, int &evolves_from_species_id, int &evolution_chain_id, int &color_id, int &shape_id, int &habitat_id, int &gender_rate, int &capture_rate, int &base_happiness, int &is_baby, int &hatch_counter, int &has_gender_differences, int &growth_rate_id, int &forms_switchable, int &is_legendary, int &is_mythical, int &order, int &conquest_order)
{
    this->id = id;
    this->identifier = identifier;
    this->generation_id = generation_id;
    this->evolves_from_species_id = evolves_from_species_id;
    this->evolution_chain_id = evolution_chain_id;
    this->color_id = color_id;
    this->shape_id = shape_id;
    this->habitat_id = habitat_id;
    this->gender_rate = gender_rate;
    this->capture_rate = capture_rate;
    this->base_happiness = base_happiness;
    this->is_baby = is_baby;
    this->hatch_counter = hatch_counter;
    this->has_gender_differences = has_gender_differences;
    this->growth_rate_id = growth_rate_id;
    this->forms_switchable = forms_switchable;
    this->is_legendary = is_legendary;
    this->is_mythical = is_mythical;
    this->order = order;
    this->conquest_order = conquest_order;
}

void pokemon_species_db::print()
{
    std::cout << "id:\t\t\t" << (this->id == -1 ? " " : std::to_string(this->id)) << std::endl;
    std::cout << "identifier:\t\t" << (this->identifier == "-1" ? " " : this->identifier) << std::endl;
    std::cout << "generation_id:\t\t" << (this->generation_id == -1 ? " " : std::to_string(this->generation_id)) << std::endl;
    std::cout << "evolves_from_species_id:" << (this->evolves_from_species_id == -1 ? " " : std::to_string(this->evolves_from_species_id)) << std::endl;
    std::cout << "evolution_chain_id:\t" << (this->evolution_chain_id == -1 ? " " : std::to_string(this->evolution_chain_id)) << std::endl;
    std::cout << "color_id:\t\t" << (this->color_id == -1 ? " " : std::to_string(this->color_id)) << std::endl;
    std::cout << "shape_id:\t\t" << (this->shape_id == -1 ? " " : std::to_string(this->shape_id)) << std::endl;
    std::cout << "habitat_id:\t\t" << (this->habitat_id == -1 ? " " : std::to_string(this->habitat_id)) << std::endl;
    std::cout << "gender_rate:\t\t" << (this->gender_rate == -1 ? " " : std::to_string(this->gender_rate)) << std::endl;
    std::cout << "capture_rate:\t\t" << (this->capture_rate == -1 ? " " : std::to_string(this->capture_rate)) << std::endl;
    std::cout << "base_happiness:\t\t" << (this->base_happiness == -1 ? " " : std::to_string(this->base_happiness)) << std::endl;
    std::cout << "is_baby:\t\t" << (this->is_baby == -1 ? " " : std::to_string(this->is_baby)) << std::endl;
    std::cout << "hatch_counter:\t\t" << (this->hatch_counter == -1 ? " " : std::to_string(this->hatch_counter)) << std::endl;
    std::cout << "has_gender_differences:\t" << (this->has_gender_differences == -1 ? " " : std::to_string(this->has_gender_differences)) << std::endl;
    std::cout << "growth_rate_id:\t\t" << (this->growth_rate_id == -1 ? " " : std::to_string(this->growth_rate_id)) << std::endl;
    std::cout << "forms_switchable:\t" << (this->forms_switchable == -1 ? " " : std::to_string(this->forms_switchable)) << std::endl;
    std::cout << "is_legendary:\t\t" << (this->is_legendary == -1 ? " " : std::to_string(this->is_legendary)) << std::endl;
    std::cout << "is_mythical:\t\t" << (this->is_mythical == -1 ? " " : std::to_string(this->is_mythical)) << std::endl;
    std::cout << "order:\t\t\t" << (this->order == -1 ? " " : std::to_string(this->order)) << std::endl;
    std::cout << "conquest_order:\t\t" << (this->conquest_order == -1 ? " " : std::to_string(this->conquest_order)) << std::endl;
    std::cout << "" << std::endl;
}

pokemon_stats_db::pokemon_stats_db()
{
    this->pokemon_id = -1;
    this->stat_id = -1;
    this->base_stat = -1;
    this->effort = -1;
}
pokemon_stats_db::pokemon_stats_db(int &pokemon_id, int &stat_id, int &base_stat, int &effort)
{
    this->pokemon_id = pokemon_id;
    this->stat_id = stat_id;
    this->base_stat = base_stat;
    this->effort = effort;
}

void pokemon_stats_db::print()
{
    std::cout << "pokemon_id:\t\t" << (this->pokemon_id == -1 ? " " : std::to_string(this->pokemon_id)) << std::endl;
    std::cout << "stat_id:\t\t" << (this->stat_id == -1 ? " " : std::to_string(this->stat_id)) << std::endl;
    std::cout << "base_stat:\t\t" << (this->base_stat == -1 ? " " : std::to_string(this->base_stat)) << std::endl;
    std::cout << "effort:\t\t" << (this->effort == -1 ? " " : std::to_string(this->effort)) << std::endl;
    std::cout << "" << std::endl;
}

Pokemon_db::Pokemon_db()
{
    this->id = -1;
    this->identifier = "-1";
    this->height = -1;
    this->weight = -1;
    this->base_experience = -1;
    this->order = -1;
    this->is_default = -1;
}
Pokemon_db::Pokemon_db(const int &id, const std::string &identifier, const int &s_id, const int &height, const int &weight, const int &base_exp, const int &order, const int &is_default)
{
    this->id = id;
    this->identifier = identifier;
    this->species_id = s_id;
    this->height = height;
    this->weight = weight;
    this->base_experience = base_exp;
    this->order = order;
    this->is_default = is_default;
}

void Pokemon_db::print()
{
    std::cout << "id:\t\t" << (this->id == -1 ? " " : std::to_string(this->id)) << std::endl;
    std::cout << "identifier:\t" << (this->identifier == "-1" ? " " : this->identifier) << std::endl;
    std::cout << "species_id:\t" << (this->species_id == -1 ? " " : std::to_string(this->species_id)) << std::endl;
    std::cout << "height:\t\t" << (this->height == -1 ? " " : std::to_string(this->height)) << std::endl;
    std::cout << "weight:\t\t" << (this->weight == -1 ? " " : std::to_string(this->weight)) << std::endl;
    std::cout << "base_experience:" << (this->base_experience == -1 ? " " : std::to_string(this->base_experience)) << std::endl;
    std::cout << "order:\t\t" << (this->order == -1 ? " " : std::to_string(this->order)) << std::endl;
    std::cout << "is_default:\t" << (this->is_default == -1 ? " " : std::to_string(this->is_default)) << std::endl;
    std::cout << "" << std::endl;
}

type_names_db::type_names_db()
{
    this->type_id = -1;
    this->local_language_id = -1;
    this->name = "-1";
}
type_names_db::type_names_db(int &type_id, std::string &name, int &local_language_id)
{
    this->type_id = type_id;
    this->local_language_id = local_language_id;
    this->name = name;
}

void type_names_db::print()
{
    std::cout << "type_id:\t\t" << (this->type_id == -1 ? " " : std::to_string(this->type_id)) << std::endl;
    std::cout << "local_language_id:\t" << (this->local_language_id == -1 ? " " : std::to_string(this->local_language_id)) << std::endl;
    std::cout << "name:\t\t\t" << (this->name == "-1" ? " " : this->name) << std::endl;
    std::cout << "" << std::endl;
}

pokemon_type_db::pokemon_type_db()
{
    this->pokemon_id = -1;
    this->type_id = -1;
    this->slot = -1;
}
pokemon_type_db::pokemon_type_db(int &pokemon_id, int &type_id, int &slot)
{
    this->pokemon_id = pokemon_id;
    this->type_id = type_id;
    this->slot = slot;
}

void pokemon_type_db::print()
{
    std::cout << "pokemon_id:\t\t" << (this->pokemon_id == -1 ? " " : std::to_string(this->pokemon_id)) << std::endl;
    std::cout << "type_id:\t\t" << (this->type_id == -1 ? " " : std::to_string(this->type_id)) << std::endl;
    std::cout << "slot:\t\t" << (this->slot == -1 ? " " : std::to_string(this->slot)) << std::endl;
}

void parse_pokemon(std::ifstream *file)
{
    int id, species_id, height, weight, base_experience, order, is_default, i = 0;
    std::string identifier;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        identifier = word;

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        species_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        height = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        weight = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        base_experience = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        order = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        is_default = stoi(word);

        pokemon_list[i++] = Pokemon_db(id, identifier, species_id, height, weight, base_experience, order, is_default);
    }

    // for (int i = 0; i < (int)pokemon_list.size(); i++)
    // {
    //     pokemon_list[i].print();
    // }
}

void parse_pokemon_move(std::ifstream *file)
{
    int pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order, i = 0;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        pokemon_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        version_group_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        move_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        pokemon_move_method_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        level = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        order = stoi(word);

        pokemon_moves_list[i++] = (pokemon_move_db(pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order));
    }

    // for (int i = 0; i < (int)pokemon_moves_list.size(); i++)
    // {
    //     pokemon_moves_list[i].print();
    // }
}

void parse_pokemon_species(std::ifstream *file)
{
    int id, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order, i = 0;
    std::string identifier;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        identifier = word;

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        generation_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        evolves_from_species_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        evolution_chain_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        color_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        shape_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        habitat_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        gender_rate = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        capture_rate = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        base_happiness = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        is_baby = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        hatch_counter = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        has_gender_differences = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        growth_rate_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        forms_switchable = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        is_legendary = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        is_mythical = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        order = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        conquest_order = stoi(word);

        species_list[i++] = (pokemon_species_db(id, identifier, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order));
    }

    // for (int i = 0; i < (int)species_list.size(); i++)
    // {
    //     species_list[i].print();
    // }
}

void parse_pokemon_stats(std::ifstream *file)
{
    int pokemon_id, stat_id, base_stat, effort, i = 0;
    std::string name;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        pokemon_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        stat_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        base_stat = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        effort = stoi(word);

        stats_list[i++] = (pokemon_stats_db(pokemon_id, stat_id, base_stat, effort));
    }

    // for (int i = 0; i < (int)stats_list.size(); i++)
    // {
    //     stats_list[i].print();
    // }
}

void parse_type_names(std::ifstream *file)
{
    int type_id, local_language_id, i = 0;
    std::string name;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        type_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        if (stoi(word) == 9)
            local_language_id = stoi(word);
        else
            continue;

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        name = word;

        type_names_list[i++] = (type_names_db(type_id, name, local_language_id));
    }

    // for (int i = 0; i < (int)type_names_list.size(); i++)
    // {
    //     type_names_list[i].print();
    // }
}

void parse_pokemon_type(std::ifstream *file)
{
    int pokemon_id, type_id, slot, i = 0;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        pokemon_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        type_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = -1;
        slot = stoi(word);

        pokemon_type_list[i++] = (pokemon_type_db(pokemon_id, type_id, slot));
    }

    // for (int i = 0; i < (int)type_names_list.size(); i++)
    // {
    //     type_names_list[i].print();
    // }
}

void parse_experience(std::ifstream *file)
{
    int growth_rate_id, level, exp, i = 0;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        growth_rate_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        level = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        exp = stoi(word);

        experience_list[i++] = (experience_db(growth_rate_id, level, exp));
    }

    // for (int i = 0; i < (int)experience_list.size(); i++)
    // {
    //     experience_list[i].print();
    // }
}

void parse_move(std::ifstream *file)
{
    int id, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id, i = 0;
    std::string identifier;
    std::string line, word;

    getline(*file, line);
    while (getline(*file, line))
    {
        std::stringstream str(line);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        identifier = word;

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        generation_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        type_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        power = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        pp = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        accuracy = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        priority = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        target_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        damage_class_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        effect_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        effect_chance = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        contest_type_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        contest_effect_id = stoi(word);

        getline(str, word, ',');
        if (word.empty())
            word = "-1";
        super_contest_effect_id = stoi(word);

        move_list[i++] = (moves_db(id, identifier, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id));
    }

    // for (int i = 0; i < (int)move_list.size(); i++)
    // {
    //     move_list[i].print();
    // }
}

void parse(std::string *fname)
{
    std::string tempString;
    // tempString = "/share/cs327/" + *fname;
    tempString = "/share/cs327/pokedex/pokedex/data/csv/" + *fname;
    std::ifstream file;
    file.open(tempString, std::ios::in);
    if (!file)
    {
        file.close();
        std::cout << "File does not exist when attempting to access: " << tempString << std::endl;
        tempString = (std::string)getenv("HOME") + "/.poke327/pokedex/pokedex/data/csv/" + *fname;
        file.open(tempString, std::ios::in);
        if (!file)
        {
            file.close();
            std::cout << "File does not exist when attempting to access: " << tempString << std::endl;
            tempString = "data/" + *fname;
            file.open(tempString, std::ios::in);
            if (!file)
            {
                file.close();
                std::cout << "Could not open the file, after attempting all 3 methods. Exitting ...\n"
                          << std::endl;
                return;
            }
        }
    }

    if (*fname == "pokemon.csv")
        parse_pokemon(&file);
    else if (*fname == "pokemon_moves.csv")
        parse_pokemon_move(&file);
    else if (*fname == "pokemon_species.csv")
        parse_pokemon_species(&file);
    else if (*fname == "pokemon_stats.csv")
        parse_pokemon_stats(&file);
    else if (*fname == "type_names.csv")
        parse_type_names(&file);
    else if (*fname == "experience.csv")
        parse_experience(&file);
    else if (*fname == "moves.csv")
        parse_move(&file);
    else if (*fname == "pokemon_types.csv")
        parse_pokemon_type(&file);
    else
        std::cout << tempString << " does not exists!" << std::endl;

    file.close();
}

void parseAll()
{
    std::string pathString = "/share/cs327/pokedex/pokedex/data/csv/";
    std::string tempString = pathString + "pokemon.csv";
    std::vector<std::string> csvNames = {"pokemon.csv", "pokemon_moves.csv", "pokemon_species.csv", "pokemon_stats.csv", "type_names.csv", "experience.csv", "moves.csv", "pokemon_types.csv"};
    std::ifstream file;
    file.open(tempString, std::ios::in);
    if (!file)
    {
        file.close();
        std::cout << "CSV Files does not exist when attempting to access: " << pathString << std::endl;
        pathString = (std::string)getenv("HOME") + "/.poke327/pokedex/pokedex/data/csv/";
        tempString = pathString + "pokemon.csv";
        file.open(tempString, std::ios::in);
        if (!file)
        {
            file.close();
            std::cout << "CSV File does not exist when attempting to access: " << pathString << std::endl;
            pathString = "data/";
            tempString = pathString + "pokemon.csv";
            file.open(tempString, std::ios::in);
            if (!file)
            {
                file.close();
                std::cout << "Could not open the file, after attempting all 3 methods. Exitting ...\n"
                          << std::endl;
                return;
            }
        }
    }

    file.close();
    for (int i = 0; i < (int)csvNames.size(); i++)
    {
        tempString = pathString + csvNames[i];
        file.open(tempString, std::ios::in);
        if (csvNames[i] == "pokemon.csv")
            parse_pokemon(&file);
        else if (csvNames[i] == "pokemon_moves.csv")
            parse_pokemon_move(&file);
        else if (csvNames[i] == "pokemon_species.csv")
            parse_pokemon_species(&file);
        else if (csvNames[i] == "pokemon_stats.csv")
            parse_pokemon_stats(&file);
        else if (csvNames[i] == "type_names.csv")
            parse_type_names(&file);
        else if (csvNames[i] == "experience.csv")
            parse_experience(&file);
        else if (csvNames[i] == "moves.csv")
            parse_move(&file);
        else if (csvNames[i] == "pokemon_types.csv")
            parse_pokemon_type(&file);
        else
            std::cout << tempString << " does not exists!" << std::endl;

        file.close();
    }
}