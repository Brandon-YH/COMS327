#ifndef ITEMS_H
#define ITEMS_H

class Items
{
private:
    int pokeball;
    int revives;
    int potions;

public:
    Items();
    Items(int pokeball, int revives, int potions);
    // ~Items();

    int get_num_pokeball();
    void set_num_pokeball(int num);
    
    int get_num_revive();
    void set_num_revive(int num);
    
    int get_num_potions();
    void set_num_potions(int num);
    };

#endif