#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>

#define NBUSES  5
#define NTICKETS_PER_THREAD 10
#define NTICKETS 50
pthread_mutex_t mutexes[NBUSES];
pthread_mutex_t profit_lock;
const char* filename = "profits.txt";

typedef struct {
    int bus_number;
} Bus;

typedef struct {
    int route_id;
    char route_name[50];
    char starting_point[50];
    char destination[50];
} Route;

typedef struct {
    int user_id;
    char username[50];
    char contact_number[12];
} User;

typedef struct {
    int ticket_id;
    float price;
} Ticket;

typedef struct {
    int id;
    int tickets;
} BusInfo;

typedef struct {
    int serialNumber;
    double profit;
    char dateTime[20]; 
} ProfitRecord;


Bus bus[10];
Route route[10];
User user[500];
Ticket ticket[500];
BusInfo bus_info[10];
char buffer[200];         //shared memory
float profit=0;            //shared vairable
int seats[NTICKETS];     



int generateRandomBusAndWriteToFile() 
{   
FILE *file = fopen("buses.txt", "a");
if (file == NULL) {
    perror("Error opening file");
    return 1; 
}

    Bus bus;
    bus.bus_number = rand() % 1000 + 1000; 
  
    fprintf(file, "%d\n", bus.bus_number);
    fclose(file); 
    return 0;
}


int generateRandomRouteAndWriteToFile() {
    FILE *file = fopen("routes.txt", "a"); 
if (file == NULL) {
    perror("Error opening file");
    return 1; 
}
    Route route;
    route.route_id = rand() % 1000 + 1; 
    char *route_names[] = {"shahrahefaisal", "carsaz", "highway", "saddar", "pechs"};
    char *start_points[] = {"malir", "numasih", "tariq road", "ayesha manzil", "bahadurabad"};
    char *destinations[] = {"airport", "cantt station", "clifton", "gulshan", "johar"};
    strcpy(route.route_name, route_names[rand() % 5]); 
    strcpy(route.starting_point, start_points[rand() % 5]); 
    strcpy(route.destination, destinations[rand() % 5]); 

    fprintf(file, "%d %s %s %s\n", route.route_id, route.route_name, route.starting_point, route.destination);
    fclose(file); 
    return 0;
}


int generateRandomUserAndWriteToFile() {
    FILE *file = fopen("users.txt", "a"); 
if (file == NULL) {
    perror("Error opening file");
    return 1; 
}
    User user;
    user.user_id = rand() % 1000 + 1; 
    char *usernames[] = {"ahan", "ansh", "noman","ayan", "ismail", "wajahat","ukasha", "abdullah", "talha","hasanain", "hussain"};
    strcpy(user.username, usernames[rand() % 11]); 
    strcpy(user.contact_number, "0319"); 
    for (int i = 4; i < 11; ++i) {
        user.contact_number[i] = rand() % 10 + '0'; 
    }
    user.contact_number[11] = '\0'; 
   
    fprintf(file, "%d %s %s\n", user.user_id, user.username, user.contact_number);
    fclose(file); 
    return 0;
}


int generateRandomTicketAndWriteToFile() {
    FILE *file = fopen("tickets.txt", "a"); 
if (file == NULL) {
    perror("Error opening file");
    return 1; 
}
    Ticket ticket;
    ticket.ticket_id = rand() % 1000 + 1; 
    ticket.price = (float)(rand() % 21 + 10)/2.0; 

    fprintf(file, "%d %.2f\n", ticket.ticket_id,ticket.price);
    fclose(file); 
    return 0;
}


void readBusDataFromFile() {
    FILE *file = fopen("buses.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int i = 0;
    while (fscanf(file, "%d",&bus[i].bus_number) != EOF){
        i++;
    }
    fclose(file);
}


void readRouteDataFromFile() {
    FILE *file = fopen("routes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    
    int i=0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%d %49s %49s %49s", &route[i].route_id, route[i].route_name, route[i].starting_point, route[i].destination) == 4) {
            i++;
        }
    }
    fclose(file);
}


void readUserDataFromFile() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
   int i = 0;
    while (fscanf(file, "%d %49s %11s", &user[i].user_id, user[i].username, user[i].contact_number) != EOF) {
        i++;
    }
    fclose(file);
}


void readTicketDataFromFile() {
    FILE *file = fopen("tickets.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    int i = 0;
    while (fscanf(file, "%d %f", &ticket[i].ticket_id,&ticket[i].price) != EOF) {
        i++;
    }
    fclose(file);
}


void read_Bus_txt() {
    FILE *file = fopen("buses.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int i = 0;
    while (fscanf(file, "%d",&bus[i].bus_number) != EOF){
    printf("Bus Number: %d\n",bus[i].bus_number);
        i++;
    }
    fclose(file);
}


void read_Route_txt() {
    FILE *file = fopen("routes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    int i=0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%d %49s %49s %49s", &route[i].route_id, route[i].route_name, route[i].starting_point, route[i].destination) == 4) {
            printf("Route ID: %d, Route Name: %s, Starting Point: %s, Destination: %s\n", route[i].route_id, route[i].route_name, route[i].starting_point, route[i].destination);
            i++;
        }
    }
    fclose(file);
}


void read_User_txt() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
   int i = 0;
    while (fscanf(file, "%d %49s %11s", &user[i].user_id, user[i].username, user[i].contact_number) != EOF) {
        printf("User ID: %d, Username: %s, Contact Number: %s\n", user[i].user_id, user[i].username, user[i].contact_number);
        i++;
    }
    fclose(file);
}


void read_Ticket_txt() {
    FILE *file = fopen("tickets.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    int i = 0;
    while (fscanf(file, "%d %f", &ticket[i].ticket_id,&ticket[i].price) != EOF) {
        printf("Ticket ID: %d, Price: %.2f\n", ticket[i].ticket_id,ticket[i].price);
        i++;
    }
    fclose(file);
}


void display_routes() {
    char *route_names[] = {"Shahrahe Faisal", "Carsaz", "Highway", "Saddar", "PECHS"};
    char *start_points[] = {"Malir", "Numaish", "Tariq Road", "Ayesha Manzil", "Bahadurabad"};
    char *destinations[] = {"Airport", "Cantt Station", "Clifton", "Gulshan", "Johar"};
    int num_routes = sizeof(route_names) / sizeof(route_names[0]);

    printf("\t\t\t\t\t\t\t\t\t***********************************************************\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t    Welcome to Our City\n");
    printf("\t\t\t\t\t\t\t\t\t***********************************************************\n");
    printf("\t\t\t\t\t\t\t\t\tRoute\t\t\tStart\t\t\tDestination\n");
    printf("\t\t\t\t\t\t\t\t\t-----------------------------------------------------------\n\n");
    for (int i = 0; i < num_routes; i++) {
        printf("\t\t\t\t\t\t\t\t\t%-15s\t\t%-15s\t\t%-15s\n\n", route_names[i], start_points[i], destinations[i]);
    }
    printf("\t\t\t\t\t\t\t\t\t-----------------------------------------------------------\n");
}


void display_schedule() {
    char *route_names[] = {"Shahrahe Faisal", "Carsaz", "Highway", "Saddar", "PECHS"};
    char *start_points[] = {"Malir", "Numaish", "Tariq Road", "Ayesha Manzil", "Bahadurabad"};
    char *destinations[] = {"Airport", "Cantt Station", "Clifton", "Gulshan", "Johar"};
    int num_routes = sizeof(route_names) / sizeof(route_names[0]);

    printf("\n\n\t\t\t\t\t\t\t\t************************************************************************************************\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tBus Schedule\n");
   printf("\t\t\t\t\t\t\t\t************************************************************************************************\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tDate: ");
    
  
    time_t now;
    struct tm *current_time;
    time(&now);
    current_time = localtime(&now);
    printf("%d-%02d-%02d\n", current_time->tm_year + 1900, current_time->tm_mon + 1, current_time->tm_mday);
    
    const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tDay: %s\n", days[current_time->tm_wday]);
    
    printf("\t\t\t\t\t\t\t\t************************************************************************************************\n");
    printf("\t\t\t\t\t\t\t\tBus ID\t\t  Route\t\t\tStart\t\tDestination\tDeparture\tArrival\n");
    printf("\t\t\t\t\t\t\t\t-------------------------------------------------------------------------------------------------\n\n");
    
  
    int departure_hour = 8;
    int departure_minute = 0;
    
    for (int i = 0; i < NBUSES; i++) {
       
        int route_index = i % num_routes;
        
        int rand_departure_hour = departure_hour + (rand() % 7) + 6; 
        int rand_departure_minute = rand() % 60; 
        
       
        int rand_arrival_hour = rand_departure_hour + (rand() % 5) + 1; 
        int rand_arrival_minute = rand() % 60; 
        
        printf("\t\t\t\t\t\t\t\t%-8d\t  %-15s\t%-15s\t%-15s\t%02d:%02d\t\t%02d:%02d\n\n",
               i + 1, route_names[route_index], start_points[route_index], destinations[route_index],
               rand_departure_hour, rand_departure_minute, rand_arrival_hour, rand_arrival_minute);
        
        departure_hour += 2;
        if (departure_hour >= 24) {
            departure_hour %= 24;
            current_time->tm_mday++; 
            mktime(current_time); 
        }
    }
    
    printf("\t\t\t\t\t\t\t\t-------------------------------------------------------------------------------------------------\n");
}


void initialize_shared_memory() {
    int shm_id;
    void *shared_memory;
    BusInfo *bus_data;

    shm_id = shmget((key_t)2345 + 101, sizeof(BusInfo) * NBUSES, 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    bus_data = (BusInfo *)shared_memory;
    for (int i = 0; i < NBUSES; ++i) {
        bus_data[i].id = i + 1; 
        bus_data[i].tickets = NTICKETS;
    }
    
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}


void print_available_tickets() {
    int shm_id;
    void *shared_memory;
    BusInfo *bus_data;

    shm_id = shmget((key_t)2345 + 101, sizeof(BusInfo) * NBUSES, 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    bus_data = (BusInfo *)shared_memory;
    printf("Available Tickets:\n");
    for (int i = 0; i < NBUSES; ++i) {
        if (bus_data[i].tickets == 0) {
            printf("Bus %d is unavailable\n", bus_data[i].id);
            
        } else {
            printf("Bus %d: %d seats\n", bus_data[i].id, bus_data[i].tickets);
            
        }
    }
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}


void check_available_tickets() {
    int shm_id;
    void *shared_memory;
    BusInfo *bus_data;

    shm_id = shmget((key_t)2345 + 101, sizeof(BusInfo) * NBUSES, 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    bus_data = (BusInfo *)shared_memory;
    for (int i = 0; i < NBUSES; ++i) {
        if (bus_data[i].tickets == 0) {
               for(int k=0;k <NTICKETS ;k++)
               { seats[k]=1;}
        } else {
            for(int j=0;j <NTICKETS ;j++)
                { seats[j]=0;}        
        }
    }
    
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}


void *book_tickets(void *args) {
    int *bus_id = (int *)args;
    int thread_num = bus_id[1];
    int bus_id_val = bus_id[0];

    int shm_id;
    void *shared_memory;
    BusInfo *bus_data;

    shm_id = shmget((key_t)2345 + 101, sizeof(BusInfo) * NBUSES, 0666);
    if (shm_id == -1) {
        perror("shmget");
        pthread_exit(NULL);
    }

    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        pthread_exit(NULL);
    }

    bus_data = (BusInfo *)shared_memory;

    for (int i = 0; i < NTICKETS_PER_THREAD; ++i) {
        pthread_mutex_lock(&mutexes[bus_id_val]);  
        if (bus_data[bus_id_val].tickets > 0) {
            int ticket_index = NTICKETS - bus_data[bus_id_val].tickets;
            printf("Thread %d for Bus %d booked ticket for seat %d\n", thread_num + 1, bus_id_val + 1, ticket_index + 1);
            bus_data[bus_id_val].tickets--;
            pthread_mutex_lock(&profit_lock);  
            profit+=ticket[i].price;
            pthread_mutex_unlock(&profit_lock);  
        } else {
            printf("Thread %d for Bus %d: No more tickets available\n", thread_num + 1, bus_id_val + 1);
            pthread_mutex_unlock(&mutexes[bus_id_val]);  
            break;
        }
        pthread_mutex_unlock(&mutexes[bus_id_val]);  
    }
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}


void *book_tickets_fancy(void *args) {
    int *bus_id = (int *)args;
    int thread_num = bus_id[1];
    int bus_id_val = bus_id[0];

    int shm_id;
    void *shared_memory;
    BusInfo *bus_data;

    shm_id = shmget((key_t)2345 + 101, sizeof(BusInfo) * NBUSES, 0666);
    if (shm_id == -1) {
        perror("shmget");
        pthread_exit(NULL);
    }

    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        pthread_exit(NULL);
    }

    bus_data = (BusInfo *)shared_memory;
    
    for (int i = 0; i < NTICKETS_PER_THREAD; ++i) {
        pthread_mutex_lock(&mutexes[bus_id_val]);  
        if (bus_data[bus_id_val].tickets > 0) {
            int ticket_index = NTICKETS - bus_data[bus_id_val].tickets;
            printf("\n\t\t\t\t\t\t********************************************************************************************\n\n");
           
            printf("\t\t\t\t\t\t\t\t\t\tBUS DETAILS:\n");
            printf("\t\t\t\t\t\t\t\t\t\tThread %d for Bus %d\n",thread_num + 1, bus_id_val + 1); 
            printf("\t\t\t\t\t\t\t\t\t\tTICKET FOR SEAT %d\n",ticket_index + 1);   
            printf("\t\t\t\t\t\t\t\t\t\tBUS NUMBER %d\n",bus[bus_id_val + 1].bus_number);
            printf("\n");
               
            printf("\t\t\t\t\t\t\t\t\t\tROUTE DETAILS:\n");
            printf("\t\t\t\t\t\t\t\t\t\tROUTE ID    :  %d\n",route[bus_id_val + 1].route_id);
            printf("\t\t\t\t\t\t\t\t\t\tROUTE NAME  :  %s\n",route[bus_id_val + 1].route_name);
            printf("\t\t\t\t\t\t\t\t\t\tSTARTING    :  %s\n",route[bus_id_val + 1].starting_point);
            printf("\t\t\t\t\t\t\t\t\t\tDESTINATION :  %s\n",route[bus_id_val + 1].destination);
            printf("\n");
            
            printf("\t\t\t\t\t\t\t\t\t\tPASSENGER DETAILS:\n");
            printf("\t\t\t\t\t\t\t\t\t\tUSER ID           :  %d\n",user[i].user_id);
            printf("\t\t\t\t\t\t\t\t\t\tUSER NAME         :  %s\n",user[i].username);
            printf("\t\t\t\t\t\t\t\t\t\tCONTACT NUMBER    :  %s\n",user[i].contact_number);
            printf("\n");
            
            pthread_mutex_lock(&profit_lock);  
            profit+=ticket[i].price;
            pthread_mutex_unlock(&profit_lock);  
            
            printf("\t\t\t\t\t\t\t\t\t\tTICKET DETAILS DETAILS:\n");
            printf("\t\t\t\t\t\t\t\t\t\tTICKET ID  :  %d\n",ticket[i].ticket_id);
            printf("\t\t\t\t\t\t\t\t\t\tPRICE      :  %.2f\n\n",ticket[i].price);
            
            printf("\n\n\t\t\t\t\t\t********************************************************************************************\n");
           
            bus_data[bus_id_val].tickets--;
        } else {
            printf("Thread %d for Bus %d: No more tickets available\n", thread_num + 1, bus_id_val + 1);
         
            pthread_mutex_unlock(&mutexes[bus_id_val]);  
            break;
        }
        pthread_mutex_unlock(&mutexes[bus_id_val]);  
    }
    
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}


void book_all_tickets_fancy() {
    pthread_t threads[NBUSES * 5];
    int bus_ids[NBUSES * 5][2]; 

    for (int i = 0; i < NBUSES; ++i) {
        if (pthread_mutex_init(&mutexes[i], NULL) != 0) {
            perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NBUSES * 5; ++i) {
        bus_ids[i][0] = i / 5; 
        bus_ids[i][1] = i % 5;
        if (pthread_create(&threads[i], NULL, book_tickets_fancy, &bus_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < NBUSES * 5; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NBUSES; ++i) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}


void book_all_tickets() {
    pthread_t threads[NBUSES * 5];
    int bus_ids[NBUSES * 5][2]; 

    for (int i = 0; i < NBUSES; ++i) {
        if (pthread_mutex_init(&mutexes[i], NULL) != 0) {
            perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NBUSES * 5; ++i) {
        bus_ids[i][0] = i / 5; 
        bus_ids[i][1] = i % 5; 
        if (pthread_create(&threads[i], NULL, book_tickets, &bus_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NBUSES * 5; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < NBUSES; ++i) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}


void readfiles()
{
    readBusDataFromFile();
    readRouteDataFromFile();
    readUserDataFromFile();
    readTicketDataFromFile();
}


int populatefiles()
{
    for(int i=0;i<10;i++)
    {
    srand(time(NULL)+i);
    if (generateRandomBusAndWriteToFile() != 0) return 1;
    if (generateRandomRouteAndWriteToFile() != 0) return 1;
    if (generateRandomUserAndWriteToFile() != 0) return 1;
    if (generateRandomTicketAndWriteToFile() != 0) return 1;
    }
}


void writeProfitToFile(const char* filename, double profit) {
    FILE* file = fopen(filename, "a"); 
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    time_t currentTime;
    time(&currentTime);
    struct tm* localTime = localtime(&currentTime);

    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    int serialNumber = fileSize / sizeof(ProfitRecord) + 1;

    ProfitRecord record;
    record.serialNumber = serialNumber;
    record.profit = profit;
    strftime(record.dateTime, sizeof(record.dateTime), "%Y-%m-%d %H:%M:%S", localTime);
    fprintf(file, "%d %.2f %d %s\n", record.serialNumber, record.profit,NBUSES, record.dateTime);

    fclose(file);
}


void read_profit(const char* filename) {
   
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Serial Number\tProfit\t\tBuses\t\tDate  &  Time\n");
    printf("--------------\t------------\t------------\t-------------------\n");
    int serialNumber;
    double profit;
    char dateTime[20];
    int busess;
    while (fscanf(file, "%d %lf %d %[^\n]", &serialNumber, &profit,&busess, dateTime) != EOF) {
        printf("%-14d\t$%-12.2f\t%d\t\t%s\n", serialNumber, profit,busess, dateTime);
    }
    fclose(file);
}


void visualizeBus(int seats[], int numSeats, int seatsPerRow) {
    printf("\n");
    printf("┌"); 
    for (int i = 0; i < seatsPerRow; i++) {
        printf("───────┬"); 
    }
    printf("\b┐\n"); 

    for (int i = 0; i < numSeats; i++) {
        if (i % seatsPerRow == 0 && i != 0) {
            printf("├"); 
            for (int j = 0; j < seatsPerRow; j++) {
                printf("───────┼"); 
            }
            printf("\b┤\n");
        }
        printf("│  %c%2d%c ", seats[i] == 0 ? ' ' : 'X', i + 1, seats[i] == 0 ? ' ' : ' ');
        if (i % seatsPerRow == seatsPerRow - 1 || i == numSeats - 1) {
            for (int j = i % seatsPerRow + 1; j < seatsPerRow; j++) {
                printf("       "); 
            }
            printf("│\n");
        }
    }

    printf("└"); 
    for (int i = 0; i < seatsPerRow; i++) {
        printf("───────┴"); 
    }
    printf("\b┘\n"); 
}


void read_data_files()
{
    printf("\n\nData from BUS file (Buses.txt):\n\n\n");
    read_Bus_txt(); 
    printf("\n\nData from ROUTES file (Routes.txt):\n\n\n");
    read_Route_txt();
    printf("\n\nData from USERS file (users.txt):\n\n\n");
    read_User_txt();
    printf("\n\nData from TICKET file (tickets.txt):\n\n\n");
    read_Ticket_txt();
}

  
int main() {
    int choice;
    readfiles();
    initialize_shared_memory();
    printf("\n\n\n");
    printf("\t\t\t\t\t\t\t\t\t***********************************************************\n\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t    BUS MANAGEMENT SYSTEM\n\n");
    printf("\t\t\t\t\t\t\t\t\t***********************************************************\n\n\n");
    printf("\t\t\t\t\t\t\t\t\t\t        ______________________________\n");
    printf("\t\t\t\t\t\t\t\t\t\t      __||__________________________||____    \n");
    printf("\t\t\t\t\t\t\t\t\t\t     |                                   /|   \n");
    printf("\t\t\t\t\t\t\t\t\t\t     |  [] [] [] [] [] [] [] [] [] []   | |    \n");
    printf("\t\t\t\t\t\t\t\t\t\t     |                                  |_|    \n");
    printf("\t\t\t\t\t\t\t\t\t\t    (|____________________________________|)   \n");
    printf("\t\t\t\t\t\t\t\t\t\t      \\___________________________________/  \n");
    printf("\t\t\t\t\t\t\t\t\t\t        ()()                       ()()        \n\n\n");
    
    do {
        printf("\n\n\t\t\t\t\t\t\t\t\t***********************************************************\n\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t     MENU\n\n");
        printf("\t\t\t\t\t\t\t\t\t***********************************************************\n\n\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t1.  Display Routes\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t2.  Display Schedule\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t3.  Print Available Tickets\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t4.  Book Tickets (Normal)\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t5.  Book Tickets (Fancy)\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t6.  Read Profit Data from File\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t7.  Visualize Bus Seats\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t8.  Populate Files\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t9.  Read data files\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t10. EXIT\n");
        
        
        printf("\t\t\t\t\t\t\t\t\t\t\tEnter your choice: ");
        scanf("\t\t\t\t\t\t\t\t\t\t\t%d", &choice);
         
        switch (choice) {
            case 1:
                display_routes();
                break;
            case 2:    
                display_schedule();
                break;
            case 3:
                print_available_tickets();
                break;
            case 4:
                 book_all_tickets();
                
                 break;
            case 5:
                
                 book_all_tickets_fancy();
               
                 break;
            case 6:
                    writeProfitToFile(filename, profit);
                    read_profit(filename);
                    break;
            case 7:
                check_available_tickets();
                for(int i=0;i<NBUSES;i++)
                {
                visualizeBus(seats, NTICKETS, 10); 
                }
                break;
            case 8:
                  if (populatefiles() != 0)
                    printf("\t\t\t\t\t\t\t\t\t\t\t\tError populating files.\n");
                    printf("\t\t\t\t\t\t\t\t\t\t\t\tFILES HAVE BEEN POPULATED WITH RANDOM DATA!\n");
                    break;
            case 9:
               read_data_files();
               break;
            case 10:
                printf("\t\t\t\t\t\t\t\t\t\t\t\t\n\nExiting program........\n");
                break;
            default:
                printf("\t\t\t\t\t\t\t\t\t\t\t\tInvalid choice. Please enter a number between 1 and 10.\n");
        }
    } while (choice != 10);

    return 0;    
}
