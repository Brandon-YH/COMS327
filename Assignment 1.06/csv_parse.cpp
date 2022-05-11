#include "csv_parse.h"

void parse_pokemon(std::ifstream *file)
{
    std::vector<Pokemon> list;
    int id, species_id, height, weight, base_experience, order, is_default;
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

        list.push_back(Pokemon(id, identifier, species_id, height, weight, base_experience, order, is_default));
    }

    for (int i = 0; i < (int)list.size(); i++)
    {
        list[i].print();
    }
}

void parse_pokemon_move(std::ifstream *file)
{
    std::vector<pokemon_move> list;
    int pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order;
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

        list.push_back(pokemon_move(pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order));
    }

    for (int i = 0; i < (int)list.size(); i++)
    {
        list[i].print();
    }
}

void parse_pokemon_species(std::ifstream *file)
{
    std::vector<pokemon_species> list;
    int id, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
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

        list.push_back(pokemon_species(id, identifier, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order));
    }

    for (int i = 0; i < (int)list.size(); i++)
    {
        list[i].print();
    }
}

void parse_type_names(std::ifstream *file)
{
 std::vector<type_names> list;
    int type_id, local_language_id;
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

        list.push_back(type_names(type_id, name, local_language_id));
    }

    for (int i = 0; i < (int)list.size(); i++)
    {
        list[i].print();
    }
}

void parse_experience(std::ifstream *file)
{
 std::vector<experience> list;
    int growth_rate_id, level, exp;
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

        list.push_back(experience(growth_rate_id, level, exp));
    }

    for (int i = 0; i < (int)list.size(); i++)
    {
        list[i].print();
    }
}

void parse_move(std::ifstream *file)
{
 std::vector<moves> list;
    int id, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id;
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

        list.push_back(moves(id, identifier, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id));
    }

    for (int i = 0; i < (int)list.size(); i++)
    {
        list[i].print();
    }
}

void parse(std::string *fname)
{
    std::string tempString;
    //tempString = "/share/cs327/" + *fname;
    tempString = "/share/cs327/pokedex/pokedex/data/csv/" + *fname;
    std::ifstream file;
    file.open(tempString, std::ios::in);
    if (!file)
    {
        file.close();
        std::cout << "File does not exist when attempting to access: " << tempString << std::endl;
        tempString = (std::string)getenv("HOME") + "/.poke327/pokedex/pokedex/data/csv/" + *fname;
        file.open(tempString, std::ios::in);
        if (!file){
            file.close();
            std::cout << "File does not exist when attempting to access: " << tempString << std::endl;
            tempString = "data/" + *fname;
            file.open(tempString, std::ios::in);
            if (!file){
                file.close();
                std::cout << "Could not open the file, after attempting all 3 methods. Exitting ...\n" << std::endl;
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
        else if (*fname == "type_names.csv")
            parse_type_names(&file);
        else if (*fname == "experience.csv")
            parse_experience(&file);
        else if (*fname == "moves.csv")
            parse_move(&file);
        else
            std::cout << tempString << " does not exists!" << std::endl;

        file.close();
}