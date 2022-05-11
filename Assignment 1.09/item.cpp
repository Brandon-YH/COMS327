#include "item.h"

Items::Items()
{
    pokeball = 10;
    revives = 10;
    potions = 10;
}

Items::Items(int num_pokeball, int num_revives, int num_potions)
{
    pokeball = num_pokeball;
    revives = num_revives;
    potions = num_potions;
}

int Items::get_num_pokeball()
{
    return pokeball;
}
void Items::set_num_pokeball(int num)
{
    if (num <= 0)
    {
        pokeball = 0;
    }
    else
        pokeball = num;
}

int Items::get_num_revive()
{
    return revives;
}
void Items::set_num_revive(int num)
{
    if (num <= 0)
    {
        revives = 0;
    }
    else
        revives = num;
}

int Items::get_num_potions()
{
    return potions;
}
void Items::set_num_potions(int num)
{
    if (num <= 0)
    {
        potions = 0;
    }
    else
        potions = num;
}