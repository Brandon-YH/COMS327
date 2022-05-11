#include "data.h"

experience::experience()
{
    this->growth_rate_id = -1;
    this->level = -1;
    this->exp = -1;
}
experience::experience(int &growth_rate_id, int &level, int &exp)
{
    this->growth_rate_id = growth_rate_id;
    this->level = level;
    this->exp = exp;
}

void experience::print()
{
    std::cout << "growth_rate_id:\t" << (this->growth_rate_id == -1 ? " " : std::to_string(this->growth_rate_id)) << std::endl;
    std::cout << "level:\t\t" << (this->level == -1 ? " " : std::to_string(this->level)) << std::endl;
    std::cout << "experience:\t" << (this->exp == -1 ? " " : std::to_string(this->exp)) << std::endl;
    std::cout << "" << std::endl;
}

moves::moves()
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
moves::moves(int id, std::string &identifier, int &generation_id, int &type_id, int &power, int &pp, int &accuracy, int &priority, int &target_id, int &damage_class_id, int &effect_id, int &effect_chance, int &contest_type_id, int &contest_effect_id, int &super_contest_effect_id)
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

void moves::print()
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

pokemon_move::pokemon_move()
{
    this->pokemon_id = -1;
    this->version_group_id = -1;
    this->move_id = -1;
    this->pokemon_move_method_id = -1;
    this->level = -1;
    this->order = -1;
}
pokemon_move::pokemon_move(int &pokemon_id, int &version_group_id, int &move_id, int &pokemon_move_method_id, int &level, int &order)
{
    this->pokemon_id = pokemon_id;
    this->version_group_id = version_group_id;
    this->move_id = move_id;
    this->pokemon_move_method_id = pokemon_move_method_id;
    this->level = level;
    this->order = order;
}

void pokemon_move::print()
{
    std::cout << "pokemon_id:\t\t" << (this->pokemon_id == -1 ? " " : std::to_string(this->pokemon_id)) << std::endl;
    std::cout << "version_group_id:\t" << (this->version_group_id == -1 ? " " : std::to_string(this->version_group_id)) << std::endl;
    std::cout << "move_id:\t\t" << (this->move_id == -1 ? " " : std::to_string(this->move_id)) << std::endl;
    std::cout << "pokemon_move_method_id:\t" << (this->pokemon_move_method_id == -1 ? " " : std::to_string(this->pokemon_move_method_id)) << std::endl;
    std::cout << "level:\t\t\t" << (this->level == -1 ? " " : std::to_string(this->level)) << std::endl;
    std::cout << "order:\t\t\t" << (this->order == -1 ? " " : std::to_string(this->order)) << std::endl;
    std::cout << "" << std::endl;
}

pokemon_species::pokemon_species()
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
pokemon_species::pokemon_species(int &id, std::string &identifier, int &generation_id, int &evolves_from_species_id, int &evolution_chain_id, int &color_id, int &shape_id, int &habitat_id, int &gender_rate, int &capture_rate, int &base_happiness, int &is_baby, int &hatch_counter, int &has_gender_differences, int &growth_rate_id, int &forms_switchable, int &is_legendary, int &is_mythical, int &order, int &conquest_order)
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

void pokemon_species::print()
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

Pokemon::Pokemon()
{
    this->id = -1;
    this->identifier = "-1";
    this->height = -1;
    this->weight = -1;
    this->base_experience = -1;
    this->order = -1;
    this->is_default = -1;
}
Pokemon::Pokemon(const int &id, const std::string &identifier, const int &s_id, const int &height, const int &weight, const int &base_exp, const int &order, const int &is_default)
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

void Pokemon::print()
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

type_names::type_names()
{
    this->type_id = -1;
    this->local_language_id = -1;
    this->name = "-1";
}
type_names::type_names(int &type_id, std::string &name, int &local_language_id)
{
    this->type_id = type_id;
    this->local_language_id = local_language_id;
    this->name = name;
}

void type_names::print()
{
    std::cout << "type_id:\t\t" << (this->type_id == -1 ? " " : std::to_string(this->type_id)) << std::endl;
    std::cout << "local_language_id:\t" << (this->local_language_id == -1 ? " " : std::to_string(this->local_language_id)) << std::endl;
    std::cout << "name:\t\t\t" << (this->name == "-1" ? " " : this->name) << std::endl;
    std::cout << "" << std::endl;
}