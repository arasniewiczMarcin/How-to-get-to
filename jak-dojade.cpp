#include <iostream>
#include <iomanip>

using namespace std;

#define BUFF 25
#define ROAD '#'
#define CITY '*'
#define DESTINATION '-1'

struct Coords {
    int x;
    int y;
};

struct City {
    char* name = nullptr;
    int timeToNeighbour = {};
    int cityNum = {};
    City* neighbour = nullptr;
    Coords coords = {};
    int** roadMap = {};
    
};

struct Flights {
    char* source;
    char* destination;
    int time;
};

struct Requests {
    char* source;
    char* destination;
    bool mode;
};

struct Queue {
    int road;
    Coords coords;
    Queue* next;
    Queue* previous;

};

struct Information {
    char** map;
    int height;
    int width;
    int citiesCounter;
    int flightsNum;
    Flights *flights = nullptr;
    Requests* req = nullptr;
    Queue* queue = nullptr;


};

struct Graph {
    
};

int queueLen(Queue* head) {
    int len = 0;
    if (head == nullptr)return len;
    else{
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

void addToQueue(Coords pos, int& road, Queue*& curEl, Queue* prevEl) {
        if (curEl == nullptr) {
            Queue* newElement = new Queue;
            newElement->coords = pos;
            newElement->road = road;
            newElement->next = nullptr;
            newElement->previous = nullptr;


            if (prevEl != NULL)newElement->previous = prevEl;
            curEl = newElement;
            if (prevEl != NULL)prevEl->next = curEl;
        }

        else {
            prevEl = curEl;
            if (curEl != NULL) addToQueue(pos, road, curEl->next, prevEl);

        }
}

Queue* front(Queue* head) {
    if (head != nullptr) {
        return head;
    }
}

void createMapAndGetInfoFromInput(Information &info) {
    cin >> info.width >> info.height;
    char field = ' ';
    info.map = new char* [info.height];
    for (int i = 0; i < info.height; i++)
        info.map[i] = new char[info.width];
    for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.width; j++) {
            cin>> info.map[i][j];
            if (info.map[i][j] == '*') {
                info.citiesCounter++;
            }
        }
            
    }
    //cin >> info.flightsNum;
    //info.flights = new Flights[info.flightsNum];
    //
    //for (int i = 0; i < info.flightsNum; i++) {
    //    cin >> info.flights[i].source;
    //    cin >> info.flights[i].destination;
    //    cin >> info.flights[i].time;
    //}
    //for (int i = 0; i < info.flightsNum; i++) {
    //    cout << info.flights[i].source<<" ";
    //    cout << info.flights[i].destination << " ";
    //    cout << info.flights[i].time << endl;
    //}

}

void freeList(Queue*& queue) {
    if (queue != nullptr && queue->next != nullptr)freeList(queue->next);
    queue = nullptr;
    delete queue;
    
}

void freeLinkedCities(City *& city) {
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
        freeLinkedCities(city[i]);
    }
    freeList(info.queue);

    delete[] info.map;
    delete[] city;
    delete[] info.flights;
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

bool checkIfRoad(Information info, int x, int y) {
    if (info.map[x][y] == ROAD)return true;
    return false;
}

bool checkIfCity(Information info, int x, int y) {
    if (info.map[x][y] == CITY)return true;
    return false;
}

char* getCityName(Information& info, Coords coords)

{
    char name[BUFF];
    //look for letter around city
    bool found = false;

    Coords letter;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (onTheMap(info, coords.x + i, coords.y + j) &&
                isLetterOrDigit(info, coords.x + i, coords.y + j)) {
                //letter found
                found = true;
                letter.x = coords.x + i;
                letter.y = coords.y + j;
                int o = 1;

                if (onTheMap(info, letter.x + 1, letter.y) && isLetterOrDigit(info, letter.x + 1, letter.y)){

                    o = 0;
                    while (onTheMap(info, letter.x + o, letter.y) && isLetterOrDigit(info, letter.x + o, letter.y))
                    {
                        name[o++] = info.map[letter.y][letter.x + o];
                    }
                    name[o] = '\0';
                    }
                else {
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
                            name[p++] = info.map[letter.y][letter.x + o + p];
                        }
                        name[p] = '\0';
                      
                }
            }
                if (found == true)break;
        }
            if (found == true)break;
    }
    char* newName = new char[strlen(name) + 1];
    int i = 0;
    while (name[i] != '\0') {
        newName[i] = name[i++];
    }
    newName[i] = '\0';
    return newName;
}

void wypiszKolejke(Queue* head) {
    if (head != nullptr) {
        cout << "Pos: " << front(head)->coords.x << " " << front(head)->coords.y<<" ";
        wypiszKolejke(head->next);
    }
}

bool visited(int** roadMap, Coords pos) {
    if (roadMap[pos.y][pos.x] == -1) return false;
    return true;
}

void checkRoadsAround(Information& info, Queue*& q, int**& roadMap, Queue *& neighToLink) {
                                      
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
            addToQueue(newPos, q->road, q, q);
        }
        else if (info.map[front(q)->coords.y][front(q)->coords.x + 1] == CITY) {
            roadMap[front(q)->coords.y][front(q)->coords.x + 1] = q->road;
            neigh.x = front(q)->coords.x + 1;
            neigh.y = front(q)->coords.y;
            addToQueue(neigh, q->road, neighToLink, neighToLink);
        }

    }
    newPos.x = front(q)->coords.x - 1;
    newPos.y = front(q)->coords.y;
    if (onTheMap(info, front(q)->coords.x - 1, front(q)->coords.y) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;
        if (info.map[front(q)->coords.y][front(q)->coords.x - 1] == ROAD) {
            roadMap[front(q)->coords.y][front(q)->coords.x - 1] = q->road;
            addToQueue(newPos, q->road, q, q);
        }
        else if (info.map[front(q)->coords.y][front(q)->coords.x - 1] == CITY) {
            roadMap[front(q)->coords.y][front(q)->coords.x - 1] = q->road;
            neigh.x = front(q)->coords.x -1;
            neigh.y = front(q)->coords.y;
            addToQueue(neigh, q->road, neighToLink, neighToLink);
        }
    }
    newPos.x = front(q)->coords.x;
    newPos.y = front(q)->coords.y - 1;
    if (onTheMap(info, front(q)->coords.x, front(q)->coords.y - 1) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;
        if (info.map[front(q)->coords.y - 1][front(q)->coords.x] == ROAD) {
            roadMap[front(q)->coords.y - 1][front(q)->coords.x] = q->road;
            addToQueue(newPos, q->road, q, q);
        }
        else if (info.map[front(q)->coords.y - 1][front(q)->coords.x] == CITY) {
            roadMap[front(q)->coords.y - 1][front(q)->coords.x] = q->road;
            neigh.x = front(q)->coords.x;
            neigh.y = front(q)->coords.y - 1;
            addToQueue(neigh, q->road, neighToLink, neighToLink);
        }
    }
    newPos.x = front(q)->coords.x;
    newPos.y = front(q)->coords.y + 1;
    if (onTheMap(info, front(q)->coords.x, front(q)->coords.y + 1) && !visited(roadMap, newPos)) {
        if (newField == false)++q->road;
        newField = true;

        if (info.map[front(q)->coords.y + 1][front(q)->coords.x] == ROAD) {
            roadMap[front(q)->coords.y + 1][front(q)->coords.x] = q->road;
            addToQueue(newPos, q->road, q, q);
        }
        else if (info.map[front(q)->coords.y + 1][front(q)->coords.x] == CITY) {
            roadMap[front(q)->coords.y + 1][front(q)->coords.x] = q->road;
            neigh.x = front(q)->coords.x;
            neigh.y = front(q)->coords.y + 1;
            addToQueue(neigh, q->road, neighToLink, neighToLink);
        }
    }
    pop(q);


}

void linkNeighbours(City*& city, Queue*& neighToLink) {
    if(queueLen(neighToLink) > 0) {
        City* newCity = new City;
        newCity->coords = front(neighToLink)->coords;
        newCity->timeToNeighbour = front(neighToLink)->road;
        city->neighbour = newCity;
        pop(neighToLink);
        linkNeighbours(city->neighbour, neighToLink);
    }
}

int** createRoadMap(Information& info, City*& city, City**& cities) {
    int** roadMap = new int* [info.height];
    for (int i = 0; i < info.height; i++)
        roadMap[i] = new int[info.width];
    
    int road = -1;
    for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.height; j++) {
            roadMap[i][j] = road;
        }
    }
    road++;
    Coords pos = city->coords;
    roadMap[pos.y][pos.x] = road;
    addToQueue(pos, road, info.queue, info.queue);

    Queue* neigh = nullptr;

    while (queueLen(info.queue) > 0) {
        checkRoadsAround(info, info.queue, roadMap, neigh);
    }
    linkNeighbours(city, neigh);

    return roadMap;
}

void getInfoAboutCities(Information info, City*& city, City**& cities)    {
    int cityCounter = -1;
    bool found = false;
    for (int j = 0; j < info.height; j++) {
        for (int o = 0; o < info.width; o++) {
            if (info.map[j][o] == CITY)
            {
                cityCounter++;
                if (cityCounter == city->cityNum) {
                    city->coords.x = o;
                    city->coords.y = j;
                    found = true;
                }
            }
            if (found == true)break;
        }
        if (found == true)break;
    }
    city->name = getCityName(info, city->coords);
    city->roadMap = createRoadMap(info, city, cities);
}

City* createVertexes(Information info, int i, City**& cities) {
    City* newCity = new City;
    newCity->cityNum = i;
    getInfoAboutCities(info, newCity, cities);

    return newCity;
}

City** createCities(Information info, Queue* head) {
    City** cities = new City*[info.citiesCounter];

    for (int i = 0; i < info.citiesCounter; i++) {
        cities[i] = createVertexes(info, i, cities);
    }
    
    return cities;
}

void wypiszMiasta(Information info, City* city) {
    if (city != nullptr) {
        cout << city->timeToNeighbour << " ";
        wypiszMiasta(info, city->neighbour);
    }
    /*for (int i = 0; i < info.citiesCounter; i++) {
        for (int o = 0; o < info.height; o++) {
            for (int p = 0; p < info.width; p++) {
                cout << city[i]->roadMap[o][p];
            }
            cout << endl;
        }
        cout << endl<< endl;
    }*/
}

int main()
{
    Information info = {};
    createMapAndGetInfoFromInput(info);
    Queue* head = NULL;
    City** city = createCities(info, head);


    /*for (int i = 0; i < info.citiesCounter; i++) {
        for (int o = 0; o < info.height; o++) {
            for (int p = 0; p < info.width; p++) {
                
                cout << setw(4)<< city[i]->roadMap[o][p];
            }
            cout << endl;
        }
        cout << endl << endl;
    }

    for (int i = 0; i < info.citiesCounter; i++) {
        wypiszMiasta(info, city[i]);
        cout << endl;
    }*/
    


    clean(info, city);
}

