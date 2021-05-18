#ifndef SYSOPY_NAMES_H
#define SYSOPY_NAMES_H

enum Reindeer_names{Dasher, Dancer, Prancer, Vixen, Comet, Cupid, Donner, Blitzen, Rudolph, ALL_REINDEER};
enum Elves_names {Bushy_Evergreen, Shinny_Upatree, Wunorse_Openslae, Pepper_Minstix, Sugarplum_Mary,
    Alabaster_Snowball, Pixie, Candy, Garland, Jingle, ALL_ELVES};

char* reindeer_name(int reindeer){
    switch (reindeer) {
        case Dasher:
            return (char*) "Dasher";
        case Dancer:
            return (char*) "Dancer";
        case Prancer:
            return (char*) "Prancer";
        case Vixen:
            return (char*) "Vixen";
        case Comet:
            return (char*) "Comet";
        case Cupid:
            return (char*) "Cupid";
        case Donner:
            return (char*) "Donner";
        case Blitzen:
            return (char*) "Blitzen";
        case Rudolph:
            return (char*) "Rudolph";
        default:
            return (char*) dunno;
    }
}

char* elf_name(int elf){
    switch (elf) {
        case Bushy_Evergreen:
            return (char*) "Bushy Evergreen";
        case Shinny_Upatree:
            return (char*) "Shinny Upatree";
        case Wunorse_Openslae:
            return (char*) "Wunorse Openslae";
        case Pepper_Minstix:
            return (char*) "Pepper Minstix";
        case Sugarplum_Mary:
            return (char*) "Sugarplum Mary";
        case Alabaster_Snowball:
            return (char*) "Alabaster Snowball";
        case Pixie:
            return (char*) "Pixie";
        case Candy:
            return (char*) "Candy";
        case Garland:
            return (char*) "Garland";
        case Jingle:
            return (char*) "Jingle";
        default:
            return (char*) dunno;
    }
}

#endif //SYSOPY_NAMES_H
