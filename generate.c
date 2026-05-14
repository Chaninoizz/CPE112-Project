#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_RECORDS 100000
#define UUID_STR_LEN 50

const char* first_names[] = {
    "Somchai","Anan","Suda","Narin","Kanya","Preecha","Mali","Wichai","Orn","Chaiya",
    "Thanawat","Krit","Somsak","Nattapong","Supaporn","Warunee","Chutima","Siriporn",
    "Apinya","Sirikarn","Teerapat","Pongpat","Tanawat","Sakda","Phuwanat","Kittipong",
    "Ratchanee","Busaba","Lalita","Jirawan","Pimchanok","Kamonwan","Chalerm","Niran",
    "Thanakorn","Pakorn","Natthapon","Kittisak","Arthit","Panuwat","Surasak","Aekkachai",
    "Pimrada","Napatsorn","Chanida","Daranee","Kornkanok","Thidarat","Supansa","Panadda","Chadathong"
};

const char* last_names[] = {
    "Sukjai","Chaiyo","Pornchai","Srisuk","Wattanakul","Thongdee","Boonsri","Kasem",
    "Chansiri","Saelim","Prasert","Kongkaew","Rattanakorn","Sawang","Chokchai","Manop",
    "Kanjana","Tantipong","Suwan","Phothong","Sukhum","Siriwat","Wongchai","Pongsiri",
    "Intarakul","Chantarasak","Nimman","Sutham","Pattara","Thipwong","Lertsiri",
    "Jaroensuk","Boonprasert","Kiatkul","Rojanapong","Tirakul","Yingcharoen","Sombat",
    "Chalermchai","Techasiri","Phupong","Ariyawong","Sirisomboon","Wisetkul","Porchaidai"
};

// Cross-platform UUID-like generator
void generate_uuid(char* uuid_str) {
    sprintf(uuid_str,
            "%08x-%04x-%04x-%04x-%04x%04x%04x",
            rand(),
            rand() % 65536,
            rand() % 65536,
            rand() % 65536,
            rand() % 65536,
            rand() % 65536,
            rand() % 65536);
}

void generate_dataset(int num_records) {

    FILE* file = fopen("data.csv", "w");

    if (!file) {
        perror("Error creating file");
        return;
    }

    fprintf(file, "name,uuid,orders,purchase\n");

    int first_count = sizeof(first_names) / sizeof(first_names[0]);
    int last_count  = sizeof(last_names) / sizeof(last_names[0]);

    for (int i = 0; i < num_records; i++) {

        const char* fname = first_names[rand() % first_count];
        const char* lname = last_names[rand() % last_count];

        int orders = rand() % 20 + 1;

        float purchase;

        // Simulate VIP customers
        if (rand() % 10 == 0) {
            purchase = (rand() % 20000) + 5000;
        }
        else {
            purchase = (rand() % 5000) + 50;
        }

        char uuid_str[UUID_STR_LEN];
        generate_uuid(uuid_str);

        fprintf(file,
                "%s_%s,%s,%d,%.2f\n",
                fname,
                lname,
                uuid_str,
                orders,
                purchase);
    }

    fclose(file);

    printf("Generated %d records successfully!\n", num_records);

}

#ifdef GENERATOR_MAIN
int main() {

    generate_dataset(DEFAULT_RECORDS);

    return 0;
}
#endif