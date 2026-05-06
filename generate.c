#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <uuid/uuid.h>   // ✅ add this

#define NUM_RECORDS 100000
#define MAX_UUID_STR 37

const char* first_names[] = {
    "Somchai","Anan","Suda","Narin","Kanya","Preecha","Mali","Wichai","Orn","Chaiya",
    "Thanawat","Krit","Somsak","Nattapong","Supaporn","Warunee","Chutima","Siriporn",
    "Apinya","Sirikarn","Teerapat","Pongpat","Tanawat","Sakda","Phuwanat","Kittipong",
    "Ratchanee","Busaba","Lalita","Jirawan","Pimchanok","Kamonwan","Chalerm","Niran"
};

const char* last_names[] = {
    "Sukjai","Chaiyo","Pornchai","Srisuk","Wattanakul","Thongdee","Boonsri","Kasem",
    "Chansiri","Saelim","Prasert","Kongkaew","Rattanakorn","Sawang","Chokchai","Manop",
    "Kanjana","Tantipong","Suwan","Phothong","Sukhum","Siriwat","Wongchai","Pongsiri",
    "Intarakul","Chantarasak","Nimman","Sutham","Pattara","Thipwong","Lertsiri"
};

int main() {
    FILE* file = fopen("data.csv", "w");
    if (!file) {
        perror("Error creating file");
        return 1;
    }

    srand(time(NULL));

    fprintf(file, "name,uuid,orders,purchase\n");

    int fcount = sizeof(first_names)/sizeof(first_names[0]);
    int lcount = sizeof(last_names)/sizeof(last_names[0]);

    for (int i = 0; i < NUM_RECORDS; i++) {

        const char* fname = first_names[rand() % fcount];
        const char* lname = last_names[rand() % lcount];

        int orders = rand() % 20 + 1;

        float purchase;
        if (rand() % 10 == 0)
            purchase = (rand() % 20000) + 5000;
        else
            purchase = (rand() % 5000) + 50;

        // 🔥 REAL UUID
        uuid_t uuid;
        uuid_generate(uuid);

        char uuid_str[MAX_UUID_STR];
        uuid_unparse_lower(uuid, uuid_str);

        fprintf(file, "%s_%s,%s,%d,%.2f\n",
                fname, lname,
                uuid_str, orders, purchase);
    }

    fclose(file);

    printf("Generated dataset with REAL UUID!\n");
    return 0;
}