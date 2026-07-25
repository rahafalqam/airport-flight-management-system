//Rahaf Alqam 1232633 Sec. 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//definition for LinkedList:
typedef struct passenger* PassengerNode; // for make it simple when define a pointer to node
struct passenger { //node
    char name[50];      // Name
    char passport[20];  // passport number
    PassengerNode next; // next passenger in the list
};
typedef PassengerNode PassengerList;


//definition for linkedList
typedef struct flight* FlightNode;
    struct flight{
    char flightID[20];
    char state[20];        // Arrival / Departure / Emergency
    char date[20];
    char time[10];
    PassengerList passengers;  // Linked List of passengers (header node)
     FlightNode next;
};
typedef FlightNode FlightList;


//Passenger list functions
PassengerList makePassengerList();
PassengerNode createPassenger(char name[], char passport[]);
int isEmpty(PassengerList L);
void insertPassengerLast(PassengerList L, PassengerNode P);
void deletePassenger(PassengerList L, char passport[]);
void printPassengers(PassengerList L);
void printPassengers(PassengerList L);
int passengerCount(PassengerList L);


//definition for queue
 struct FlightQueue {
    FlightNode front; //first
    FlightNode rear; // last
    int size;
};
typedef struct FlightQueue*Queue;


// Queue function
FlightList makeFlightList(); //
Queue createQueue();
int isEmptyQueue(Queue q);
FlightNode dequeue(Queue q);
void enqueue(Queue q, FlightNode f);
FlightNode frontFlight(Queue q);



// MAIN FUNCTION :- (prototype)
void viewMenu();
void loadPassengers(char filename[], FlightNode flightList);
FlightNode loadFlights(char filename[]);
void buildQueuesFromFlights(FlightNode flights, Queue arrival, Queue departure, Queue emergency);
void printFlightDetails(FlightNode f);
FlightNode findFlightByID(FlightNode head, char id[]);
FlightNode addFlight(FlightNode flights, Queue arrival, Queue departure, Queue emergency);
void landFlight(Queue emergency, Queue arrival);
void departFlight(Queue departure);
void PrintFlightsFile(char filename[]);
void removeFlightFromQueue(Queue q, FlightNode f);
void cancelFlight(FlightNode flights, Queue arrival, Queue departure, Queue emergency);
void displayFlightStatus(FlightNode flights, Queue arrival, Queue departure, Queue emergency);
void managePassengers(FlightNode flights);

//case 10
void freePassengerList(PassengerList L);
void freeFlights(FlightNode flights);
void freeQueue(Queue q);
void exitProgram(FlightNode flights, Queue arrival, Queue departure, Queue emergency);




int main() {
    FlightNode flights = NULL;

    // queues for flights (currently unused, but created)
    Queue arrival   = createQueue();
    Queue departure = createQueue();
    Queue emergency = createQueue();

    // removed/cancelled flights list (not used yet)
    FlightList removedList = NULL;

    char choice[10];

    do {
        viewMenu();
        printf("\nPLEASE ,Enter your choice\n");
        scanf("%9s", choice);

        while (atoi(choice) < 1 || atoi(choice) > 10) {
            printf("Invalid input , Enter a valid one\n");
            scanf("%9s", choice);
        }

        switch (atoi(choice)) {
        case 1: //work
            flights = loadFlights("flight.txt");
            if (flights == NULL) {
                printf("SORRY ,File not found.\n");
                break;
            }

            loadPassengers("passenger.txt", flights);
            buildQueuesFromFlights(flights, arrival, departure, emergency); // save data in Queue

            PrintFlightsFile("flight.txt");
            PrintFlightsFile("passenger.txt");

            printf("\nData loaded succeffully !! \n");

            break;

        case 2: // work
            PrintFlightsFile("flight.txt");
            break;

        case 3:
            if (flights == NULL) {
                printf("No flights loaded yet. Choose option 1 first.\n");
                break;
            }

            char searchID[20];
            printf("\nEnter the flight number: ");
            scanf("%19s", searchID);

            FlightNode f = findFlightByID(flights, searchID);

            if (f == NULL) {
                printf("Flight not found.\n");
            } else {
                printFlightDetails(f);
            }
            break;

        case 4:
            flights = addFlight(flights, arrival, departure, emergency);
            break;

        case 5:
            landFlight(emergency, arrival);
            break;

        case 6:
            departFlight(departure);
            break;

        case 7:
           cancelFlight(flights, arrival, departure, emergency);
           break;

       case 8:
           displayFlightStatus(flights, arrival, departure, emergency);
           break;

        case 9:
           managePassengers(flights);
           break;

        case 10:
           exitProgram(flights, arrival, departure, emergency);
           break;

        default:
            printf("\nInvalid choice. Please try again.\n");
            break;
        }
    } while (atoi(choice) != 10);

    return 0;
}


void viewMenu() {
    printf("\n");
    printf("=======|Main Menu|=======\n");
    printf("1. Load File\n"); //(flights & passengers)
    printf("2. Print all flights\n");
    printf("3. Print flight details\n");
    printf("4. Add a new flight\n");
    printf("5. Land a flight\n");
    printf("6. Depart a plane\n");
    printf("7. Cancel a flight\n");
    printf("8. Display flight status\n");
    printf("9. Manage passengers\n");
    printf("10. Exit\n\n");
}


// first option
void loadPassengers(char filename[], FlightNode flightList) {
    FILE* file = fopen(filename, "r"); // open file
    if (file == NULL) { //check
        printf("Cannot open passengers file!\n");
        return;
    }
    char p[200];
    while (fgets(p, sizeof(p), file)) {
            // in order
        char name[50];
        char passport[20];
        char flightID[20];
        char *token; // to split the line
        token = strtok(p, ";");
        if (token == NULL) {
            continue;
        }
        strcpy(name, token); // save the name in token
        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        strcpy(passport, token);
        token = strtok(NULL, ";");
        if (token == NULL) {
            continue;
        }
        strcpy(flightID, token);
        int x = strlen(flightID); // to remove /n ...
        if (x > 0 && (flightID[x - 1] == '\n' )) {
            flightID[x - 1] = '\0';
        }
        FlightNode current = flightList; //find the right flight
        while (current != NULL) {
            if (strcmp(current->flightID, flightID) == 0) {
                if (current->passengers == NULL) {
                    current->passengers = makePassengerList(); //header
                }
                PassengerNode newP = createPassenger(name, passport);// new passenger
                insertPassengerLast(current->passengers, newP);
                break;
            }
            current = current->next;
        }
    }
    fclose(file);
    printf("Passengers loaded successfully!\n");
}

// continue first option
FlightNode loadFlights(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open flights file!\n");
        return NULL;
    }
    char y[128];
    FlightNode head = NULL;
    FlightNode tail = NULL;
    while (fgets(y, sizeof(y), file)) {
        // skip empty lines
        if (y[0] == '\n' )
            continue;
        FlightNode current = (FlightNode)malloc(sizeof(struct flight));
        if (current == NULL) {
            printf("Out of memory while reading flights.\n");
            fclose(file);
            return head;
        }
        current->passengers = makePassengerList(); // header list
        current->next       = NULL;
        // FlightID;State;Date;Time
        char *token;
        // 1) FlightID
        token = strtok(y, ";");
        if (token == NULL) {
            free(current);
            continue;
        }
        strcpy(current->flightID, token);
        // 2) State
        token = strtok(NULL, ";");
        if (token == NULL) {
            free(current);
            continue;
        }
        strcpy(current->state, token);
        // 3) Date
        token = strtok(NULL, ";");
        if (token == NULL) {
            free(current);
            continue;
        }
        strcpy(current->date, token);
        // 4) Time
        token = strtok(NULL, ";");
        if (token == NULL) {
            free(current);
            continue;
        }
        strcpy(current->time, token);
        int n = strlen(current->time);
        if (n > 0 && (current->time[n - 1] == '\n' || current->time[n - 1] == '\r')) {
            current->time[n - 1] = '\0';
        }
        if (head == NULL)
            head = tail = current;
        else {
            tail->next = current;
            tail       = current;
        }
    }
    fclose(file);
    printf("Flights loaded successfully!\n");
    return head;
}

// helper
 void buildQueuesFromFlights(FlightNode flights, Queue arrival, Queue departure, Queue emergency) {
    FlightNode current = flights; // pointer to check the list (start with flights )

    while (current != NULL) {

        FlightNode copy = (FlightNode)malloc(sizeof(struct flight)); //new node
        if (copy == NULL) {
            printf("SORRY, An error while building queues.\n");
            return;
        }

        strcpy(copy->flightID, current->flightID); // copy data to new node
        strcpy(copy->state,current->state);
        strcpy(copy->date,current->date);
        strcpy(copy->time,current->time);

        copy->passengers = current->passengers; // copy passenger

        copy->next = NULL;
                   // fill the flight acording to state in Q.
        if (strcmp(copy->state, "Emergency") == 0) {
            enqueue(emergency, copy);
        }
        else if (strcmp(copy->state, "Arrival") == 0) {
            enqueue(arrival, copy);
        }
        else if (strcmp(copy->state, "Departure") == 0) {
            enqueue(departure, copy);
        }

        current = current->next; // end of list
    }
}


// second option
void PrintFlightsFile(char filename[]) {
    FILE* file = fopen(filename, "r"); //read

    if (file == NULL) { //check
        printf("Cannot open file: %s\n", filename);
        return;
    }

    printf("\nOpening %s:\n", filename);

    char line[200];
    while (fgets(line, sizeof(line), file)) { // to read line by line
        printf("%s", line);
    }

    fclose(file);
}

//third option
void printFlightDetails(FlightNode f) {
    if (f == NULL) {
        printf("Flight not found.\n");
        return;
    }

    printf("\nFlight details:\n");
    printf("Flight ID : %s\n", f->flightID);
    printf("State : %s\n", f->state);
    printf("Date : %s\n", f->date);
    printf("Time : %s\n", f->time);

    printf("\nPassengers for these flight :\n");

    if (f->passengers == NULL || f->passengers->next == NULL) {
        printf("No passengers for this flight.\n");
        return;
    }

    PassengerNode p = f->passengers /* this the header */ ->next; // first real passenger

    int i = 1;
    while (p != NULL) {
        printf("%d) Name %s , PassportID %s\n", i, p->name, p->passport);
        p = p->next;
        i++;
    }
}

//helper for 3
FlightNode findFlightByID(FlightNode head, char id[]) {
    FlightNode p = head; //pointer p share to first node

    while (p != NULL) {
        if (strcmp(p->flightID, id) == 0) {  // found the flight
            return p;
        }
        p = p->next;  // move on if we donot found then another loop
    }

    return NULL; // not found
}


// fourth option
FlightNode addFlight(FlightNode flights, Queue arrival, Queue departure, Queue emergency) {
    char id[20];
    char state[20];
    char date[20];
    char time[10];  // variable for flight

    printf("Enter Flight ID: ");
    scanf("%s", id);

    if (findFlightByID(flights, id) != NULL) { // to check if we have it
        printf("SORRY ,There is a Flight with this ID \n");
        return flights;
    }

    int typeChoice;
    printf("Select flight type: ( number of ur choice) \n");
    printf("1. Arrival\n");
    printf("2. Departure\n");
    printf("3. Emergency\n");
    printf("Enter your choice: ");
    scanf("%d", &typeChoice);

    switch (typeChoice) {
        case 1:
            strcpy(state, "Arrival"); //save in state
            break;
        case 2:
            strcpy(state, "Departure");
            break;
        case 3:
            strcpy(state, "Emergency");
            break;
        default:
            printf("Invalid type. Flight not added.\n");
            return flights;
    }

    printf("PLEASE, Enter Flight Date (dd-mm-yyyy): ");
    scanf("%s", date);

    printf("PLEASE, Enter Flight Time (hh:mm): ");
    scanf("%s", time);

    FlightNode newF = (FlightNode)malloc(sizeof(struct flight)); //made new node
    if (newF == NULL) {
        printf("SORRY, Flight not added.\n");
        return flights;
    }

    strcpy(newF->flightID, id); // id in newF->flightID
    strcpy(newF->state, state);
    strcpy(newF->date, date);
    strcpy(newF->time, time);

    newF->passengers = makePassengerList();  // header of passenger
    newF->next = NULL;


    if (flights == NULL) {
        flights = newF;  // first flight in list
    } else {
        FlightNode last = flights; // pointer named last
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newF; // new node the last
    }

    // to save in queue
    if (strcmp(state, "Emergency") == 0) {
        enqueue(emergency, newF);
    } else if (strcmp(state, "Arrival") == 0) {
        enqueue(arrival, newF);
    } else { // Departure
        enqueue(departure, newF);
    }

    printf("Flight added successfully.\n");

    return flights;
}


// 5th option
void landFlight(Queue emergency, Queue arrival) {
    FlightNode f = NULL;

    printf("\nLand a Flight:\n");

    if (!isEmptyQueue(emergency)) { // emergency first
        f = dequeue(emergency);
        printf("Landing EMERGENCY flight:\n");
    }
    else if (!isEmptyQueue(arrival)) { // arrival
        f = dequeue(arrival);
        printf("Landing ARRIVAL flight:\n");
    }
    else {
        printf("No flights to land.\n");
        return;
    }
    printFlightDetails(f);
}


//6th option
void departFlight(Queue departure) {
    FlightNode f = NULL; // varible save flight will depart

    printf("\n Depart a Flight :\n");

    if (isEmptyQueue(departure)) {
        printf("No flights to depart\n");
        return;
    }

    f = dequeue(departure);

    printf("Departing flight:\n");

    printFlightDetails(f);
}


// helper for 7
void removeFlightFromQueue(Queue q, FlightNode f) {
    if (q == NULL || q->front == NULL || f == NULL)
        return;

    FlightNode prev = NULL; // prev. node pointer
    FlightNode curr = q->front; // curr. pointer

    while (curr != NULL && curr != f) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) { // means f flight not found
        return;
    }
    // remove from front
    if (prev == NULL) { // curr. the first in Q.
        q->front = curr->next;
        if (q->front == NULL) { // Q. empety
            q->rear = NULL;
        }
    } else { // in mid. or last
        prev->next = curr->next;
        if (q->rear == curr) {
            q->rear = prev;
        }
    }
    curr->next = NULL; // نفصلها
    q->size--;
}

// 7th option
void cancelFlight(FlightNode flights, Queue arrival, Queue departure, Queue emergency) {
    if (flights == NULL) {
        printf("No flights to cancel.\n");
        return;
    }

    char id[20];
    printf("Enter Flight ID to cancel: ");
    scanf("%19s", id);

    FlightNode f = findFlightByID(flights, id); // to find this flight

    if (f == NULL) { //check
        printf("Flight with this ID not found.\n");
        return;
    }

    if (strcmp(f->state, "Cancelled") == 0 ) {
        printf("Flight is already cancelled.\n");
        return;
    }

    removeFlightFromQueue(arrival,f);
    removeFlightFromQueue(departure,f);
    removeFlightFromQueue(emergency,f);

    strcpy(f->state, "Cancelled"); // change the state

    printf("\nFlight %s cancelled successfully.\n", f->flightID);
    printf("Current status:\n");
    printf("Flight ID : %s\n", f->flightID);
    printf("State : %s\n", f->state);
    printf("Date : %s\n", f->date);
    printf("Time : %s\n\n", f->time);
}


//8th option
void displayFlightStatus(FlightNode flights, Queue arrival, Queue departure, Queue emergency) {
    printf("\n Flight Status :: \n");

    printf("\nFlights ready to LAND:\n");
    if (arrival == NULL || arrival->front == NULL) { //check
        printf("Sorry , Flights not found\n");
    } else {
        FlightNode curr = arrival->front; // make pointer
        while (curr != NULL) {
            int count = 0;
            if (curr->passengers != NULL)
                count = passengerCount(curr->passengers); // to find total count

            printf("ID: %s, State: %s, Date: %s, Time: %s, Passengers: %d\n",
                   curr->flightID, curr->state, curr->date, curr->time, count);

            curr = curr->next;
        }
    }

    printf("\nFlights ready to DEPART :\n"); // for ready depart
    if (departure == NULL || departure->front == NULL) {
        printf("Sorry , Flights not found.\n");
    } else {
        FlightNode curr = departure->front;
        while (curr != NULL) {
            int count = 0;
            if (curr->passengers != NULL)
                count = passengerCount(curr->passengers);

            printf("ID: %s, State: %s, Date: %s, Time: %s, Passengers: %d\n",
                   curr->flightID, curr->state, curr->date, curr->time, count);

            curr = curr->next;
        }
    }

    printf("\nEMERGENCY Flights:\n"); // for emergency
    if (emergency == NULL || emergency->front == NULL) {
        printf("Sorry , Flights not found.\n");
    } else {
        FlightNode curr = emergency->front;
        while (curr != NULL) {
            int count = 0;
            if (curr->passengers != NULL)
                count = passengerCount(curr->passengers);

            printf("ID: %s, State: %s, Date: %s, Time: %s, Passengers: %d\n",
                   curr->flightID, curr->state, curr->date, curr->time, count);

            curr = curr->next;
        }
    }

    printf("\n Cancelled flights:\n"); // already depart or cancel
    int f = 0;
    FlightNode p = flights; // from flight list
    while (p != NULL) {
        if (strcmp(p->state, "Departed") == 0 || strcmp(p->state, "Cancelled") == 0) {
            int count = 0;
            if (p->passengers != NULL)
                count = passengerCount(p->passengers); // to count

            printf("ID: %s, State: %s, Date: %s, Time: %s, Passengers: %d\n",
                   p->flightID, p->state, p->date, p->time, count);
            f = 1; // means found and print
        }
        p = p->next;
    }
    if (!f) {
        printf("Sorry , Flights not found.\n");
    }
}


//9th option
void managePassengers(FlightNode flights) {
    if (flights == NULL) { //check
        printf("No flights loaded /n");
        return;
    }

    char flightID[20];
    printf("Enter Flight ID to manage passengers: ");
    scanf("%s", flightID);

    FlightNode f = findFlightByID(flights, flightID); // to find the flight
    if (f == NULL) {
        printf("Flight not found.\n");
        return;
    }

    int choice;
    do {
        printf("\n Manage Passengers ::\n");
        printf("1. Print passengers of a given flight.\n");
        printf("2. Add passenger to	a flight.\n");
        printf("3. Remove passenger from a flight.\n");
        printf("4. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {  // a) Print passengers
            if (f->passengers == NULL || f->passengers->next == NULL) {
                printf("No passengers for this flight.\n");
            } else {
                printPassengers(f->passengers); // will print all passenger and there id
            }

        } else if (choice == 2) {  // b) Add passenger
            char name[50];
            char passport[20];

            if (f->passengers == NULL) {
                f->passengers = makePassengerList();
            }

            printf("Enter passenger name: ");
            scanf("%s", name);

            printf("Enter passport number: ");
            scanf("%s", passport);

            PassengerNode newP = createPassenger(name, passport); //new node
            insertPassengerLast(f->passengers, newP); // add to list

            printf("Passenger added successfully.\n");

        } else if (choice == 3) { // c) Remove passenger
            char passport[20];
            printf("Enter passport number to remove: ");
            scanf("%s", passport);

            if (f->passengers == NULL || f->passengers->next == NULL) {
                printf("No passengers for this flight.\n");
            } else {
                deletePassenger(f->passengers, passport);

                  printf("Passenger deleted successfully.\n");
            }

        } else if (choice == 4) {
            printf("Return to main menu...\n");
        } else {
            printf("Invalid choice. Try again.\n");
        }

    } while (choice != 4);
}

//10th option
void exitProgram(FlightNode flights, Queue arrival, Queue departure, Queue emergency) {

    freeFlights(flights);

    freeQueue(arrival); // to make Qs. empty
    freeQueue(departure);
    freeQueue(emergency);

    printf("\n GoodBye \n");
}
// helper for 10
void freePassengerList(PassengerList L) {
    if (L == NULL)
        return;
    PassengerNode curr = L;
    while (curr != NULL) {
        PassengerNode temp = curr;
        curr = curr->next;
        free(temp);
    }
}
// helper for 10
void freeFlights(FlightNode flights) {
    FlightNode curr = flights;
    while (curr != NULL) {
        if (curr->passengers != NULL) {
            freePassengerList(curr->passengers);
        }
        FlightNode temp = curr;
        curr = curr->next;
        free(temp);
    }
}
// helper for 10
void freeQueue(Queue q) {
    if (q == NULL)
        return;
    free(q);
}



// PASSINGER LIST FUNCTION:
//done
PassengerList makePassengerList() {
    PassengerList L = (PassengerList)malloc(sizeof(struct passenger)); // space for header node
    if (!L) { // not have memory
        printf("Out of memory!\n");
        exit(1);
    }
    L->next = NULL; //pointer of header -> next
    return L;
}

//done
PassengerNode createPassenger(char name[], char passport[]) {
    PassengerNode p = (PassengerNode)malloc(sizeof(struct passenger));
    if (!p) {
        printf("Out of memory!\n");
        exit(1);
    }
    strcpy(p->name, name); // take name and put it in p->name
    strcpy(p->passport, passport);
    p->next = NULL; // last
    return p;
}

//done
int isEmpty(PassengerList L) {
    return (L == NULL || L->next == NULL); // return 1 (empty)
}

//done
void insertPassengerLast(PassengerList L, PassengerNode P) {
    if (L == NULL) { // to check if we have the list
        printf("Error: Passenger list is NULL.\n");
        return;
    }
    if (P == NULL) { //check passenger
        printf("Error: Passenger is NULL\n");
        return;
    }

    PassengerNode current = L; // create current varible = header
    while (current->next != NULL) {
        current = current->next; // current = next node
    }

    current->next = P;
    P->next = NULL;
}

//done
void deletePassenger(PassengerList L, char passport[]) {
    if (L == NULL) {
        printf("Passenger list not created!\n");
        return;
    }

    PassengerNode prev = L;       // header
    PassengerNode curr = L->next; // first passenger

    // search by passport
    while (curr != NULL /* ( not reach the end */ && strcmp(curr->passport, passport) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) { // not found passenger
        printf("Passenger not found!\n");
        return;
    }

    prev->next = curr->next;
    free(curr);
}

//done
void printPassengers(PassengerList L) {
    if (L == NULL || L->next == NULL) {
        printf("No passengers.\n");
        return;
    }

    PassengerNode p = L->next; // start with first passenger not header

    printf("Passengers:\n");

    while (p != NULL) { // check we have passenger
        printf("Name: %15s | Passport: %s\n", p->name, p->passport);
        p = p->next;
    }
}

//
int passengerCount(PassengerList L) {
    int count = 0;
    if (L == NULL)
        return 0;

    PassengerNode p = L->next; //start from the next of header

    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}


//QUEUE FUNCTION :-
FlightList makeFlightList() {
    FlightList L = (FlightList)malloc(sizeof(struct flight)); // space for header node
    if (!L) { // not have memory
        printf("Out of memory!\n");
        exit(1);
    }
    L->next = NULL; //pointer of header -> next
    return L;
}

//done
Queue createQueue() {
    Queue Q = (Queue)malloc(sizeof(struct FlightQueue));
    if (Q == NULL) {
        printf("Out of space!\n");
        exit(1);
    }
    Q->front = NULL;   // no flights yet
    Q->rear  = NULL;
    Q->size  = 0;

    return Q;
}

//done
int isEmptyQueue(Queue q) {
    return (q == NULL || q->front == NULL);
}

//done
FlightNode dequeue(Queue q) { //remove from front
    if (q == NULL || isEmptyQueue(q)) {
        return NULL;
    }
    FlightNode temp = q->front;   // first flight
    q->front = q->front->next;    // move front
    if (q->front == NULL) {
        q->rear = NULL;
    }
    temp->next = NULL;
    q->size--;

    return temp;
}

//
void enqueue(Queue q, FlightNode f) {
    if (q == NULL)
        return;

    if (f == NULL) {
        printf("Error: Flight is NULL\n");
        return;
    }

    f->next = NULL; // last element in queue
    if (q->rear == NULL) {
        q->front = f;
        q->rear  = f;
    } else {
        q->rear->next = f;
        q->rear       = f;
    }
    q->size++;
}

//done
FlightNode frontFlight(Queue q) {
    if (q == NULL || isEmptyQueue(q))
        return NULL;
    return q->front;
}
