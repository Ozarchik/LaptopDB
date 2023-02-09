#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRINT_ALL_LAPTOPS   't'
#define MENU_ADD_LAPTOP     'r'
#define SORT                'c'
#define SEARCH              'w'
#define DELETE_LAPTOP       'd'
#define SAVE_ON_FILE        's'
#define READ_FILE           'b'
#define EXIT                'q'
#define MODEL               'm'
#define PRICE               'p'

#define NAME_SIZE           15

typedef struct Laptop{
    unsigned id;
    char model    [NAME_SIZE];
    char processor[NAME_SIZE];
    char videoCard[NAME_SIZE];
    unsigned int SSD;
    unsigned int RAM;
    double price;

    struct Laptop* next;
} Laptop;

static unsigned id = 1;
Laptop* root = NULL;

void print(Laptop* ltp);
void Laptop_delete(Laptop* ltp);
void save_DB(Laptop* root, const char* filename);
void read_DB(const char* filename);
void List_show(Laptop* root);
void show_header();

Laptop* List_push(Laptop* root, Laptop* newLaptop, int readFileMode);
Laptop* List_remove(Laptop* root, int ind);
Laptop* Laptop_Create(char* model, double price, char* processor, unsigned int RAM, char* videoCard,  unsigned int SSD);
Laptop* Menu_createLaptop(Laptop* root);
void writeDataOnFile(FILE* pFile, Laptop* ltp);
void searchLaptopByPrice(Laptop* root, double min_price, double max_price);
void searchLaptopByModel(Laptop* root, const char* inputWord);

void searchLaptopByModel(Laptop* root, const char* inputWord) {
    Laptop* current = root;
    show_header();
    while (current != NULL) {
        if (strstr(current->model, inputWord) != NULL) {
            print(current);
        }
        current = current->next;
    }
}

void searchLaptopByPrice(Laptop* root, double min_price, double max_price) {
    show_header();
    Laptop* current = root;
    while (current != NULL) {
        if (current->price >= min_price && current->price <= max_price) {
            print(current);
        }
        current = current->next;
    }
}



Laptop* sort_database(Laptop* root, char sym, int direction) {
    Laptop* new_root;
    if (sym == 'p' && direction == 1) {
        while (root != NULL){
            Laptop* node = root;
            root = root->next;

            if (new_root == NULL || node->price < new_root->price){
                node->next = new_root;
                new_root = node;
            }
            else{
                Laptop* current = new_root;
                while (current->next != NULL && !(node->price < current->next->price)){                   
                      current = current->next;
                }                
                node->next = current->next;
                current->next = node;
            }
        }    
    return new_root;

    }

        if (sym == 'p' && direction == 2){
        while (root != NULL){
            Laptop* node = root;
            root = root->next;

            if (new_root == NULL || node->price > new_root->price){
                node->next = new_root;
                new_root = node;
            }
            else{
                Laptop* current = new_root;
                while (current->next != NULL && !(node->price > current->next->price)){                   
                      current = current->next;
                }                
                node->next = current->next;
                current->next = node;
            }
        }
      return new_root;
      }

    if (sym == 'm' && direction == 1) {
        while (root != NULL){
            Laptop* node = root;
            root = root->next;

            if (new_root == NULL || (strcmp(node->model,new_root->model) < 0) ){
                node->next = new_root;
                new_root = node;
            }
            else{
                Laptop* current = new_root;
                while (current->next != NULL && !(strcmp(node->model,current->next->model) < 0)){                   
                      current = current->next;
                }                
                node->next = current->next;
                current->next = node;
            }
        }
        return new_root;
    }

    if (sym == 'm' && direction == 2) {
        while (root != NULL){
            Laptop* node = root;
            root = root->next;

            if (new_root == NULL || (strcmp(node->model,new_root->model) > 0) ){
                node->next = new_root;
                new_root = node;
            }
            else{
                Laptop* current = new_root;
                while (current->next != NULL && !(strcmp(node->model,current->next->model) > 0)){                   
                      current = current->next;
                }                
                node->next = current->next;
                current->next = node;
            }
        }
        return new_root;
    }
}

void save_DB(Laptop* root, const char* filename){
        FILE* pFile = fopen(filename, "w");
        if(pFile == NULL){
            printf("Error saving to file %s", filename);
            return;
        }
        Laptop* current = root;
        fprintf(pFile, "%d\n", id);
        while (current != NULL){
            fprintf(pFile, "%u %s %s %s %u %u %lf", current->id, current->model, current->processor, current->videoCard, current->SSD, current->RAM, current->price);
            if (current->next != NULL) {fprintf(pFile, "\n");}
            current = current->next;
        }
        fclose(pFile);
}

void read_DB(const char* filename){
    FILE* pFile = fopen(filename, "r");
    if(pFile == NULL){
        printf("Error reading file %s", filename);
        return;
    }
    Laptop* newLaptop;
    fscanf(pFile, "%u\n", &id);
    while(!feof(pFile)){
        newLaptop = (Laptop*) malloc(sizeof(Laptop));
        fscanf(pFile, "%u %s %s %s %u %u %lf", &newLaptop->id, newLaptop->model, newLaptop->processor, newLaptop->videoCard, &newLaptop->SSD, &newLaptop->RAM, &newLaptop->price);
        newLaptop->next = NULL;
        int rfileMode = 1;
        root = List_push(root, newLaptop, rfileMode);
    }
    fclose(pFile);
}

void List_show(Laptop* root){
    if(root == NULL){
        printf("List is empty!\n");
    }
    show_header();
    while(root != NULL){
        print(root);
        root = root->next;
    }
}

void show_header(){
    printf("\n\n");
    printf("|----------------------------------------------------------------------------------------|\n");
    printf("|                                     [LAPTOP DATABASE]                                  |\n");
    printf("|----------------------------------------------------------------------------------------|\n");
    printf("| ID   | Model\t\t  | Price\t  | Processor\t| RAM\t| Videocard\t | SSD\t |\n");
}

void print(Laptop* current){
        printf("| %d    | %-16.15s | %-8.lf\t  | %-8.8s\t| %-4d\t| %-14.12s | %-4d\t |\n", \
        current->id, current->model, current->price, current->processor, current->RAM, current->videoCard, current->SSD);
}

Laptop* List_remove(Laptop* root, int ind){
    if(root == NULL){
        return root;
    }
    if(ind == 1){
        Laptop* next = root->next;
        Laptop_delete(root);
        return next;
    }
    Laptop* prev = root;
    Laptop* current = root;
    while(current != NULL){
        if(ind == current->id){
            prev->next = current->next;
            Laptop_delete(current);
            break;
        }
        prev = current;
        current = current->next;\
    }
    return root;
}

Laptop* List_push(Laptop* root, Laptop* newLaptop, int readFileMode){
    if(root == NULL){ return newLaptop; }
    Laptop* tmpLaptop = root;
    if (readFileMode != 1){
        while(tmpLaptop->next != NULL){
            tmpLaptop = tmpLaptop->next;
        }
        tmpLaptop->next = newLaptop;
        tmpLaptop->next->id = ++id;
    } else {
        while(tmpLaptop->next != NULL){
            tmpLaptop = tmpLaptop->next;
        }
        tmpLaptop->next = newLaptop;
    }
    return root;
}

Laptop* Menu_createLaptop(Laptop* root){
    Laptop tmp;
    printf("\nPlease, write follow fields of Laptop (without using space!):\n");
    printf(">> model(string): ");     scanf(" %s", &tmp.model);
    printf(">> price(number): ");     scanf(" %lf", &tmp.price);
    printf(">> processor(string): "); scanf(" %s", &tmp.processor);
    printf(">> RAM(number): ");       scanf(" %d", &tmp.RAM);
    printf(">> videoCard(string): "); scanf(" %s", &tmp.videoCard);
    printf(">> SSD(number): ");       scanf(" %d", &tmp.SSD);

    int rfileMode = 0;
    root = List_push(root, Laptop_Create(tmp.model, tmp.price, tmp.processor, tmp.RAM, tmp.videoCard, tmp.SSD), rfileMode);
    return root;
}

Laptop* Laptop_Create(char* model, double price, char* processor, unsigned int RAM, char* videoCard,  unsigned int SSD){
    Laptop* ltp = malloc(sizeof(Laptop));
    strcpy(ltp->model, model);
    strcpy(ltp->videoCard, videoCard);
    strcpy(ltp->processor, processor);
    ltp->id = id;
    ltp->price = price;
    ltp->RAM = RAM;
    ltp->SSD = SSD;
    ltp->next = NULL;
    return ltp;
}

void Show_menu(){
    printf("MENU:\n");
    printf("%c - Print all laptops\n",   PRINT_ALL_LAPTOPS);
    printf("%c - Add new laptop\n",      MENU_ADD_LAPTOP);
    printf("%c - Sort laptops\n",        SORT);
    printf("%c - Search laptops\n",      SEARCH);
    printf("%c - Delete laptops\n",      DELETE_LAPTOP);
    printf("%c - Save DB\n",             SAVE_ON_FILE);
    printf("%c - Read DB\n",             READ_FILE);
    printf("%c - Exit\n",                EXIT);

}

void Laptop_delete(Laptop* ltp){
    printf("ltp %s has been deleted (had size %d)", ltp, sizeof(ltp));
    free(ltp);
}

int main(){
    char input;
    while(1){
        Show_menu();
        printf("Input action key: ");
        scanf(" %c", &input);
        if (input == PRINT_ALL_LAPTOPS){
            List_show(root);
        }
        else if (input ==  MENU_ADD_LAPTOP){
            root = Menu_createLaptop(root);
        }
        else if (input == SORT){
            char sort;
            int direction;
            printf("\nSort by model or price? (m - model, p - price) ");
            scanf(" %c", &sort);
            printf("\nSort by ascending (1) or descending(2)? ");
            scanf(" %d", &direction);
            if (sort == MODEL || sort == PRICE){
                root = sort_database(root, sort, direction);
                List_show(root);
            } else {
                printf("Нужно ввести 'm' или 'p'. Хотите дальше продолжить? 'y' - continue, other - break\n");
                char answ;
                scanf("%s ", &answ);
                if (answ == 'y'){
                    root = sort_database(root, sort, direction);
                    List_show(root);
                }
                else if (answ != 'y'){
                    root = sort_database(root, sort, direction);
                    List_show(root);
                }
            }
        }
        else if (input == SEARCH){
            char search;
            printf("sort by model(%c) or price(%c)? ", MODEL, PRICE);
            scanf(" %c", &search);
            if (search == PRICE){
                float min_price, max_price;
                printf("Please, write minimum price for search: ");
                scanf(" %f", &min_price);
                printf("Please, write maximum price for search: ");
                scanf(" %f", &max_price);
                searchLaptopByPrice(root, min_price, max_price);
            }
            if (search == MODEL){
                char* in_model = malloc(NAME_SIZE);
                printf("Please, write prefix or full name of model laptop for search: ");
                scanf(" %s", in_model);
                searchLaptopByModel(root, in_model);
                free(in_model);
            }
        }
        else if (input == DELETE_LAPTOP){
            printf("Enter ltp index: ");
            int index = 0;
            scanf(" %d", &index);
            root = List_remove(root, index);
        }
        else if (input ==  SAVE_ON_FILE){
            save_DB(root, "./DB.txt");
        }
        else if (input == READ_FILE){
            read_DB("./DB.txt");
        }
        else if (input == EXIT){
            printf("Good Bye!");
            return 0;
        }
        else {
            printf("unknown command");
        }
        printf("\n");
        if (!input) {return 0;}
    }
}