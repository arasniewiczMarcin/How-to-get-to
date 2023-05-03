#include <iostream>
#include <iomanip>

using namespace std;

#define BUFF_VISITED 250
#define BUFF 25
#define ROAD '#'
#define CITY '*'
#define DESTINATION '-1'
#define ENTER 10

struct Coords {
    int x;
    int y;
};

struct List {
    char citiesOnRoad[BUFF];
    List* next;
};

struct Results {
    //char source[BUFF];
    char destination[BUFF] = {};
    int timeFromSource = 0;
    List* townsOnRoad = nullptr;
    Results* next = nullptr;
};

struct City {
    char name[BUFF] = {};
    int timeToNeighbour = {};
    int cityNum = -1;
    City* neighbour = nullptr;
    Coords coords = {};
    int** roadMap = {};
    int timeFromSource = -1;
    bool visited = false;
    char visitedTowns[BUFF] = {};
    int* previousCity;
    Results* result = nullptr;

};

struct Flights {
    char source[BUFF] = {};
    char destination[BUFF] = {};
    bool added = false;
    Coords pos = {};
    int time = 0;
};

struct Requests {
    char source[BUFF] = {};
    char destination[BUFF] = {};
    bool mode = {};
};

struct Queue {
    int road = 0;
    Coords coords = {};
    Queue* next = nullptr;
    Queue* previous = nullptr;
    int indexMap = -1;

};
struct QueueReq {
    Requests req = {};
    Queue* next = nullptr;
    Queue* previous;
};

struct Information {

    char** map;
    int height;
    int width;
    int citiesCounter;
    int flightsNum;
    int reqNum;
    Flights* flights = nullptr;
    Requests* req = nullptr;
    Queue* queue = nullptr;
    Queue* previousCities = nullptr;
};

int findCitiesNum(Coords coords, City** cities, Information info) {
    //cout <<"POS: "<< coords.x << " " << coords.y << endl;
    for (int i = 0; i < info.citiesCounter; i++) {
        //cout << cities[i]->coords.x << " " << cities[i]->coords.y << endl;
        if (cities[i] != nullptr && cities[i]->cityNum != -1 && cities[i]->coords.x == coords.x && cities[i]->coords.y == coords.y) {
            return cities[i]->cityNum;
        }
    }
    return -1;
}

int queueLen(Queue* head) {

    int len = 0;
    if (head == nullptr)return len;
    else {
        while (head != nullptr) {
            len++;
            head = head->next;
        }
    }
    return len;
}

void pop(Queue*& head) {

    if (head != nullptr) {
        Queue* temp = head;
        head = head->next;
        temp = nullptr;
        delete temp;
    }
}

void addToQueue(Coords pos, int& road, int indexMap, Queue*& curEl, Queue* prevEl) {

    if (curEl == nullptr) {

        Queue* newElement = new Queue;
        newElement->coords = pos;
        newElement->road = road;
        newElement->indexMap = indexMap;
        newElement->next = nullptr;
        newElement->previous = nullptr;


        if (prevEl != NULL)newElement->previous = prevEl;
        curEl = newElement;
        if (prevEl != NULL)prevEl->next = curEl;
    }

    else {
        prevEl = curEl;
        if (curEl != NULL) addToQueue(pos, road, indexMap, curEl->next, prevEl);

    }
}

Queue* front(Queue* head) {

    if (head != nullptr) {
        return head;
    }
    return nullptr;
}

void createMapAndGetInfoFromInput(Information& info) {

    cin >> info.width >> info.height;
    info.map = new char* [info.height];
    for (int i = 0; i < info.height; i++)
        info.map[i] = new char[info.width];
    for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.width; j++) {
            cin >> info.map[i][j];
            if (info.map[i][j] == '*') {
                info.citiesCounter++;
            }
        }

    }
    /*for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.width; j++) {
            cout<<setw(4)<< info.map[i][j];
        }
        cout << endl;
    }
    getchar();
    getchar();*/
    cin >> info.flightsNum;

    info.flights = new Flights[info.flightsNum];
    char sign = 's';
    for (int i = 0; i < info.flightsNum; i++) {
        int j = 0;
        sign = getchar();
        while (sign != ' ') {
            if (sign > 32)
                info.flights[i].source[j++] = sign;
            sign = getchar();
        }
        info.flights[i].source[j] = '\0';
        j = 0;
        sign = getchar();
        while (sign != ' ') {
            if (sign > 32)
                info.flights[i].destination[j++] = sign;
            sign = getchar();
        }
        info.flights[i].destination[j] = '\0';

        cin >> info.flights[i].time;
    }
    /*for (int i = 0; i < info.flightsNum; i++) {
        cout << info.flights[i].source << " ";
        cout << info.flights[i].destination << " ";
        cout << info.flights[i].time << endl;
    }*/

    cin >> info.reqNum;

    info.req = new Requests[info.reqNum];
    sign = 's';
    for (int i = 0; i < info.reqNum; i++) {
        int j = 0;
        sign = getchar();
        while (sign != ' ') {
            if (sign > 32)
                info.req[i].source[j++] = sign;
            sign = getchar();
        }
        info.req[i].source[j] = '\0';
        j = 0;
        sign = getchar();
        while (sign != ' ') {
            if (sign > 32)
                info.req[i].destination[j++] = sign;
            sign = getchar();
        }
        info.req[i].destination[j] = '\0';

        cin>>info.req[i].mode;
    }
    /*for (int i = 0; i < info.reqNum; i++) {
        cout << info.req[i].source << " ";
        cout << info.req[i].destination << " ";
        cout << info.req[i].mode << endl;
    }*/
    //cout << endl << endl;

}

void freeList(Queue*& queue) {

    if (queue != nullptr && queue->next != nullptr)freeList(queue->next);
    queue = nullptr;
    delete queue;

}

void freeLinkedCities(City*& city) {

    if (city != nullptr && city->neighbour != nullptr)freeLinkedCities(city->neighbour);
    city = nullptr;
    delete city;
}

void clean(Information& info, City**& city) {


    for (int i = 0; i < info.height; i++) {
        delete[] info.map[i];

    }

    for (int i = 0; i < info.citiesCounter; i++) {
        delete[] city[i]->roadMap;
        delete[] city[i]->previousCity;
        freeLinkedCities(city[i]);
    }
    freeList(info.queue);

    delete[] info.map;
    delete[] city;
    delete[] info.flights;
    delete[] info.req;
}

bool onTheMap(Information info, int x, int y) {

    if (x >= 0 && x < info.width && y >= 0 && y < info.height)
        return true;
    return false;
}

bool isLetterOrDigit(Information info, int x, int y) {

    if ((info.map[y][x] >= '0' && info.map[y][x] <= '9') ||
        (info.map[y][x] >= 'A' && info.map[y][x] <= 'Z') ||
        (info.map[y][x] >= 'a' && info.map[y][x] <= 'z'))
        return true;
    return false;
}


void getCityName(Information& info, Coords coords, char cityName[])

{

    //look for letter around city
    bool found = false;

    Coords letter;
    // int i = 0;
     //name[i] = 'd';
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
           
            if (onTheMap(info, coords.x + j, coords.y + i)) {
            
                if(isLetterOrDigit(info, coords.x + j, coords.y + i)) {
                //letter found
                //found = true;
                letter.x = coords.x + j;
                letter.y = coords.y + i;
                int o = 0;
                

                if (onTheMap(info, letter.x + 1, letter.y) && isLetterOrDigit(info, letter.x + 1, letter.y)
                    && !isLetterOrDigit(info,letter.x - 1, letter.y)) {
                    found = true;
                    o = 0;
                    while (onTheMap(info, letter.x + o, letter.y) && isLetterOrDigit(info, letter.x + o, letter.y))
                    {
                        //char sign = 'G';

                        //if (o == 0)name[0] = sign;
                        //else
                        cityName[o] = info.map[letter.y][letter.x + o];

                        o++;
                    }
                    cityName[o] = '\0';
                }
                else if(onTheMap(info, letter.x - 1, letter.y) && isLetterOrDigit(info, letter.x - 1, letter.y)
                    && !isLetterOrDigit(info, letter.x + 1, letter.y)){
                    found = true;
                    //last letter
                    o = -1;
                    while (onTheMap(info, letter.x + o, letter.y) && isLetterOrDigit(info, letter.x + o, letter.y))
                    {
                        o--;
                    }
                    o++;
                    int p = 0;
                    while (onTheMap(info, letter.x + o + p, letter.y) && isLetterOrDigit(info, letter.x + o + p, letter.y))
                    {
                        
                        cityName[p] = info.map[letter.y][letter.x + o + p];
                        p++;
                    }
                    
                    cityName[p] = '\0';

                }
                else {
                    cityName[o] = info.map[letter.y][letter.x];
                    o++;
                    cityName[o] = '\0';
                }
            }
            if (found == true)break;
        }
        if (found == true)break;
    }
    }

}

void wypiszKolejke(Queue* head) {

    if (head != nullptr) {
        cout << "Pos: " << front(head)->coords.x << " " << front(head)->coords.y << " ";
        wypiszKolejke(head->next);
    }
}

bool visited(int** roadMap, Coords pos) {

    if (roadMap[pos.y][pos.x] == -1) return false;
    return true;
}

bool checkIfWordsEqual(char* word1, char* word2) {

    //cout << word1 << " " << word2 << endl;
    int i = 0;
    int word1Len = 0;
    int word2Len = 0;
    while (word1[i]) {
        word1Len++;
        i++;
    }
    i = 0;
    while (word2[i]) {
        word2Len++;
        i++;
    }
    //cout << word1Len << " " << word2Len << endl;
    i = 0;
    if (word1Len != word2Len)return false;
    while (word1[i] && word2[i]) {
        //cout << "spierdalaj" << endl;
        if (word1[i] != word2[i]) {
            //cout << "word1: " << word1 << " " << "word2: " << word2 << endl;
           // cout << "-"<<word1[i] << " - " << word2[i] <<"-" << endl;
            return false;
        }
        i++;

    }
    return true;
}

void findFlightIn(Flights f, City* city, bool& overwrite) {

    if (city != nullptr && city->coords.x == f.pos.x && city->coords.y == f.pos.y) {
        overwrite = true;
        if (f.time < city->timeToNeighbour) city->timeToNeighbour = f.time;
    }
    else if (city != nullptr) {
        findFlightIn(f, city->neighbour, overwrite);
    }
}

void addFlight(Flights f, City*& city) {

    if (city == nullptr) {
        City* newCity = new City;
        newCity->coords = f.pos;
        //newCity->name = new char[strlen(f.destination) + 1];
        int i = 0;
        while (f.destination[i]) {
            newCity->name[i] = f.destination[i];
            i++;
        }
        newCity->name[i] = '\0';
        //newCity->name = f.destination;
        newCity->timeToNeighbour = f.time;
        newCity->neighbour = nullptr;
        city = newCity;
    }
    else addFlight(f, city->neighbour);
}

void addFlights(Information& info, City*& city) {

    bool overwrite = false;
    for (int i = 0; i < info.flightsNum; i++) {
        if (city != nullptr && checkIfWordsEqual(info.flights[i].source, city->name)) {
            if (city != nullptr && city->neighbour != nullptr)
                findFlightIn(info.flights[i], city->neighbour, overwrite);
            if (overwrite == false) {
                addFlight(info.flights[i], city);
            }

        }
    }
}

void findPreviousCity(int**& roadMap, Coords pos, Coords basicPos, Information& info, int counter, int basicTime, City**& cities) {
    //znajdz poprzednie miasto idac po malejacych liczbach
    //cout << "pos: " << pos.x << " " << pos.y << " basicPos: " << basicPos.x << " " << basicPos.y << " basicTime: " << basicTime << endl;
    //cout << basicTime << " "<<counter;
    if (info.map[pos.y][pos.x] == CITY)counter++;
    //if (basicTime == 0)return;
    if (counter != 2) {
        //miasto nieznalezione
        if (onTheMap(info, pos.x + 1, pos.y) && roadMap[pos.y][pos.x + 1] == basicTime - 1) {
            pos.x += 1;
            basicTime--;

        }
        else if (onTheMap(info, pos.x - 1, pos.y) && roadMap[pos.y][pos.x - 1] == basicTime - 1) {
            pos.x -= 1;
            basicTime--;

        }
        else if (onTheMap(info, pos.x, pos.y + 1) && roadMap[pos.y + 1][pos.x] == basicTime - 1) {
            pos.y += 1;
            basicTime--;

        }
        else if (onTheMap(info, pos.x, pos.y - 1) && roadMap[pos.y - 1][pos.x] == basicTime - 1) {
            pos.y -= 1;
            basicTime--;

        }
        /*getchar();
        getchar();*/
        findPreviousCity(roadMap, pos, basicPos, info, counter, basicTime, cities);

    }
    else {
        //cout << "znaleziony "<< endl;
        int basicCity = findCitiesNum(basicPos, cities, info);
        int previousCity = findCitiesNum(pos, cities, info);
        //cout << cities[basicCity]->name << " " << cities[previousCity]->name << endl;
        //cout << "index: " << front(info.previousCities)->indexMap << endl;
        //cout << "basic " << basicCity << endl;
        cities[front(info.previousCities)->indexMap]->previousCity[basicCity] = previousCity;
        //cout << "znaleziony "<< previousCity <<basicCity<< endl;
        /*int i = 0;
        while (cities[previousCity]->name[i]) {
            cities[basicCity]->visitedTowns[i] = cities[previousCity]->name[i];
            i++;
        }
        cities[basicCity]->visitedTowns[i] = '\0';*/
        pop(info.previousCities);



    }
}

void checkRoadsAround(Information& info, Queue*& q, int**& roadMap, Queue*& neighToLink, City* city) {


    Coords newPos;
    Coords neigh;
    newPos.x = front(q)->coords.x + 1;
    newPos.y = front(q)->coords.y;
    bool newField = false;
    if (onTheMap(info, front(q)->coords.x + 1, front(q)->coords.y) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;
        if (info.map[front(q)->coords.y][front(q)->coords.x + 1] == ROAD) {
            roadMap[front(q)->coords.y][front(q)->coords.x + 1] = q->road;
            addToQueue(newPos, q->road, -1, q, q);
        }
        else if (info.map[front(q)->coords.y][front(q)->coords.x + 1] == CITY) {
            roadMap[front(q)->coords.y][front(q)->coords.x + 1] = q->road;
            neigh.x = front(q)->coords.x + 1;
            neigh.y = front(q)->coords.y;
            addToQueue(neigh, q->road, -1, neighToLink, neighToLink);
            addToQueue(newPos, q->road, -1, q, q);
            addToQueue(newPos, q->road, city->cityNum, info.previousCities, info.previousCities);
        }

    }
    newPos.x = front(q)->coords.x - 1;
    newPos.y = front(q)->coords.y;
    if (onTheMap(info, front(q)->coords.x - 1, front(q)->coords.y) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;
        if (info.map[front(q)->coords.y][front(q)->coords.x - 1] == ROAD) {
            roadMap[front(q)->coords.y][front(q)->coords.x - 1] = q->road;
            addToQueue(newPos, q->road, -1, q, q);
        }
        else if (info.map[front(q)->coords.y][front(q)->coords.x - 1] == CITY) {
            roadMap[front(q)->coords.y][front(q)->coords.x - 1] = q->road;
            neigh.x = front(q)->coords.x - 1;
            neigh.y = front(q)->coords.y;
            addToQueue(neigh, q->road, -1, neighToLink, neighToLink);
            addToQueue(newPos, q->road, -1, q, q);
            addToQueue(newPos, q->road, city->cityNum, info.previousCities, info.previousCities);
        }
    }
    newPos.x = front(q)->coords.x;
    newPos.y = front(q)->coords.y - 1;
    if (onTheMap(info, front(q)->coords.x, front(q)->coords.y - 1) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;
        if (info.map[front(q)->coords.y - 1][front(q)->coords.x] == ROAD) {
            roadMap[front(q)->coords.y - 1][front(q)->coords.x] = q->road;
            addToQueue(newPos, q->road, -1, q, q);
        }
        else if (info.map[front(q)->coords.y - 1][front(q)->coords.x] == CITY) {
            roadMap[front(q)->coords.y - 1][front(q)->coords.x] = q->road;
            neigh.x = front(q)->coords.x;
            neigh.y = front(q)->coords.y - 1;
            addToQueue(neigh, q->road, -1, neighToLink, neighToLink);
            addToQueue(newPos, q->road, -1, q, q);
            addToQueue(newPos, q->road, city->cityNum, info.previousCities, info.previousCities);
        }
    }
    newPos.x = front(q)->coords.x;
    newPos.y = front(q)->coords.y + 1;
    if (onTheMap(info, front(q)->coords.x, front(q)->coords.y + 1) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;

        if (info.map[front(q)->coords.y + 1][front(q)->coords.x] == ROAD) {
            roadMap[front(q)->coords.y + 1][front(q)->coords.x] = q->road;
            addToQueue(newPos, q->road, -1, q, q);
        }
        else if (info.map[front(q)->coords.y + 1][front(q)->coords.x] == CITY) {
            roadMap[front(q)->coords.y + 1][front(q)->coords.x] = q->road;
            neigh.x = front(q)->coords.x;
            neigh.y = front(q)->coords.y + 1;
            addToQueue(neigh, q->road, -1, neighToLink, neighToLink);
            addToQueue(newPos, q->road, -1, q, q);
            addToQueue(newPos, q->road, city->cityNum, info.previousCities, info.previousCities);
        }
    }
    pop(q);


}


void linkNeighbours(City*& city, City** cities, Queue*& neighToLink, Information& info) {

    if (queueLen(neighToLink) > 0) {
        City* newCity = new City;
        newCity->coords = front(neighToLink)->coords;
        newCity->timeToNeighbour = front(neighToLink)->road;
        newCity->neighbour = nullptr;
        city->neighbour = newCity;
        pop(neighToLink);
        linkNeighbours(city->neighbour, cities, neighToLink, info);
    }
}


int** createRoadMap(Information& info, City*& city, City**& cities) {

    int** roadMap = new int* [info.height];
    for (int i = 0; i < info.height; i++)
        roadMap[i] = new int[info.width];

    int road = -1;
    for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.width; j++) {
            roadMap[i][j] = road;
        }
    }
    
    road++;
    Coords pos = city->coords;
    roadMap[pos.y][pos.x] = road;
    /*for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.height; j++) {
            cout << setw(4) << roadMap[i][j];
        }
        cout << endl;
    }
    getchar();
    getchar();*/
    addToQueue(pos, road, -1, info.queue, info.queue);

    Queue* neigh = nullptr;

    while (queueLen(info.queue) > 0) {
        checkRoadsAround(info, info.queue, roadMap, neigh, city);
    }
    linkNeighbours(city, cities, neigh, info);
    /*for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.width; j++) {
            cout << setw(4) << roadMap[i][j];
        }
        cout << endl;
    }
    getchar();
    getchar();*/

    return roadMap;
}

void getInfoAboutCities(Information& info, City*& city, City**& cities) {

    int cityCounter = -1;
    bool found = false;
    for (int j = 0; j < info.height; j++) {
        for (int o = 0; o < info.width; o++) {
            if (info.map[j][o] == CITY)
            {
                cityCounter++;
                if (cityCounter == city->cityNum) {
                    city->coords.y = j;
                    city->coords.x = o;
                    found = true;
                }
            }
            if (found == true)break;
        }
        if (found == true)break;
    }
    getCityName(info, city->coords, city->name);
    //cout << city->name << endl;
    city->roadMap = createRoadMap(info, city, cities);
   /* for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.height; j++) {
            cout << setw(4) << city->roadMap[i][j];
        }
        cout << endl;
    }
    getchar();
    getchar();*/

}

City* createVertexes(Information& info, int i, City**& cities) {

    City* newCity = new City;
    newCity->cityNum = i;
    getInfoAboutCities(info, newCity, cities);

    return newCity;
}

void addCitiesNums(City*& city, City** cities, Information& info) {

    if (city != nullptr) {
        if (city->cityNum == -1)
            city->cityNum = findCitiesNum(city->coords, cities, info);
        addCitiesNums(city->neighbour, cities, info);
    }

}

City** createGraph(Information& info) {

    City** cities = new City * [info.citiesCounter];
    for (int i = 0; i < info.citiesCounter; i++) {
        cities[i] = nullptr;
    }

    for (int i = 0; i < info.citiesCounter; i++) {
        cities[i] = createVertexes(info, i, cities);
        //cities[i]->visitedTowns[0] = '\0';
    }
    for (int i = 0; i < info.citiesCounter; i++) {
        for (int j = 0; j < info.flightsNum; j++) {
            //da sie lepiej
            if (checkIfWordsEqual(info.flights[j].destination, cities[i]->name)) {
                info.flights[j].pos = cities[i]->coords;
            }
        }
    }
    for (int i = 0; i < info.citiesCounter; i++) {
        if (info.flightsNum > 0)addFlights(info, cities[i]);
    }
    for (int i = 0; i < info.citiesCounter; i++) {
        addCitiesNums(cities[i], cities, info);
        cities[i]->previousCity = new int[info.citiesCounter];

    }
    for (int i = 0; i < info.citiesCounter; i++) {
        for (int j = 0; j < info.citiesCounter; j++) {
            cities[i]->previousCity[j] = -1;

        }

    }
    /*for(int i = 0; i< info.citiesCounter)
    newCity->cityNum = findCitiesNum(front(neighToLink)->coords, cities, info);*/
    //cout << queueLen(info.previousCities);
    while (queueLen(info.previousCities) > 0) {
        findPreviousCity(cities[front(info.previousCities)->indexMap]->roadMap, front(info.previousCities)->coords, front(info.previousCities)->coords, info, 0,
            cities[front(info.previousCities)->indexMap]->roadMap[front(info.previousCities)->coords.y][front(info.previousCities)->coords.x], cities);
    }

    return cities;
}

void wypiszMiasta(Information& info, City* city) {

    if (city != nullptr) {
        cout << city->name << " - ";
        wypiszMiasta(info, city->neighbour);
    }
    /*
    for (int i = 0; i < info.citiesCounter; i++) {
        for (int o = 0; o < info.height; o++) {
            for (int p = 0; p < info.width; p++) {
                cout << city[i]->roadMap[o][p];
            }
            cout << endl;
        }
        cout << endl<< endl;
    }
    */
}

City* findCity(char* source, City** cities, Information info) {

    for (int i = 0; i < info.citiesCounter; i++) {
        if (checkIfWordsEqual(cities[i]->name, source)) {
            return cities[i];
        }
    }
    return nullptr;
}

void AddString(char mainString[], char* s) {

    int i = 0;
    while (s[i]) {
        mainString[i] = s[i];
    }
    mainString[i] = '\0';
}
//usunac
//void addTownsOnRoad(List* townsOnRoad, )
void addResult(Results*& sourceCity, City* destinationCity, List* townsOnRoad) {

    if (sourceCity == nullptr) {
        Results* newR = new Results;
        AddString(newR->destination, destinationCity->name);
        newR->timeFromSource = destinationCity->timeFromSource;
        newR->townsOnRoad = townsOnRoad;
        newR->next = nullptr;
        sourceCity = newR;
    }
    else {
        addResult(sourceCity->next, destinationCity, townsOnRoad);
    }
}

void findTheLowestTime(City** cities, int lowest, int& index, Information info) {

    for (int i = 0; i < info.citiesCounter; i++) {
        if ((lowest > cities[i]->timeFromSource || lowest == 0) && cities[i]->timeFromSource != -1 && cities[i]->visited == false) {
            lowest = cities[i]->timeFromSource;
            index = i;
        }
    }
}

void clearResults(Information info, City**& cities) {

    for (int i = 0; i < info.citiesCounter; i++) {
        cities[i]->timeFromSource = -1;
        cities[i]->visited = false;
        cities[i]->visitedTowns[0] = '\0';
    }
}

void fastestPathToNeigh(City* curCity, City* mainCity, City**& cities, Information info, bool visitedTowns) {
    if (curCity != nullptr) {
        //cout << cities[curCity->cityNum]->name << " ";
       // cout << cities[mainCity->cityNum]->name << endl;
        
        int index = curCity->cityNum;
        if (cities[index]->visited == false && cities[index]->timeFromSource > curCity->timeToNeighbour
            || cities[index]->timeFromSource == -1) {
            cities[index]->timeFromSource = curCity->timeToNeighbour;


        }
        if (curCity->neighbour != nullptr) {
            fastestPathToNeigh(curCity->neighbour, curCity, cities, info, visitedTowns);
        }

    }
}

void showVisitedTowns(City** cities, Information info, City* destination, int basicIndex, char* dest) {

    if (cities[basicIndex]->previousCity[destination->cityNum] != -1) {
        showVisitedTowns(cities, info, cities[cities[basicIndex]->previousCity[destination->cityNum]], basicIndex, dest);
       if(!checkIfWordsEqual(dest, destination->name)) cout << " " << destination->name;
        
    }

}

void getFastestPath(Information info, City**& cities, City*& head, Requests req, int lastIndex) {

    if (head != nullptr) {
        head->timeFromSource = 0;
        int lowestIndex = 0;

            findTheLowestTime(cities, 0, lowestIndex, info);
            //if (cities[lowestIndex]->timeFromSource != -1)
                fastestPathToNeigh(cities[lowestIndex], cities[lowestIndex], cities, info, req.mode);
            

            cities[lowestIndex]->visited = true;

    }
    //getchar();
    //getchar();

    

        if (req.mode == true && findCity(req.destination, cities, info) != nullptr && findCity(req.destination, cities, info)->timeFromSource != -1) {
            // cout << req.source << " " << req.destination << " " << req.mode << " ";
            cout << findCity(req.destination, cities, info)->timeFromSource;
            showVisitedTowns(cities, info, findCity(req.destination, cities, info), findCity(req.source, cities, info)->cityNum, req.destination);
            cout << endl;
        }
        else  if (findCity(req.destination, cities, info) != nullptr && findCity(req.destination, cities, info)->timeFromSource != -1)
            cout << findCity(req.destination, cities, info)->timeFromSource << endl;

        clearResults(info, cities);

}
int findCitiesNumByName(Coords coords, City** cities, Information info) {

    for (int i = 0; i < info.citiesCounter; i++) {
        if (cities[i]->coords.x == coords.x && cities[i]->coords.y == coords.y) {
            return cities[i]->cityNum;
        }
    }
    return 0;
}
int findCityNum2(char* source, City** cities, Information info) {

    for (int i = 0; i < info.citiesCounter; i++) {
        //cout << cities[i]->name << " " << source << endl;
        if (checkIfWordsEqual(cities[i]->name, source)) {
            return i;
        }
    }
    return -1;
}

void dijkstra(City**& cities, Information info) {

    for (int i = 0; i < info.reqNum; i++) {
      
       int index = findCityNum2(info.req[i].source, cities, info);
       
        
       if (index != -1) getFastestPath(info, cities, cities[index], info.req[i], index);


    }

}

int main()
{

    Information info = {};
    createMapAndGetInfoFromInput(info);
    Queue* head = NULL;
    City** city = createGraph(info);
  


    /*for (int i = 0; i < info.citiesCounter; i++) {
        for (int j = 0; j < info.citiesCounter; j++) {
           cout << " "<<city[i]->previousCity[j];
        }
        cout << endl;
    }*/

    //OK jestesmy w B to wypisz 

    //if (city[1]->previousCity[2] != -1) cout << city[3]->name << " " << city[city[1]->previousCity[3]]->name << endl;

    //int ind = 0;
    //for (int i = 0; i < info.citiesCounter; i++) {
    //    for (int j = 0; j < info.citiesCounter; j++) {
    //        //ind = city[i]->previousCity[j];
    //       // cout << city[ind]->name;
    //       //if(city[i]->previousCity[j] != -1) cout << city[j]->name << " " << city[city[i]->previousCity[j]]->name << endl;
    //        // cout << city[i]->previousCity << endl;
    //    }
    //    //cout << endl << endl;
    //}
   dijkstra(city, info);


    clean(info, city);
}

