#include <iostream>

using namespace std;

#define BUFF 25
#define ROAD '#'
#define CITY '*'
#define ROAD_START '!'

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
    char** roadMap = {};
    
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

struct Queue {
    Coords coords;
    Queue* next;
    
};



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
    if (queue->next != nullptr)freeList(queue->next);
    queue = nullptr;
    delete queue;
    
}

void clean(Information& info, City*& city) {

    for (int i = 0; i < info.height; i++) {
        delete[] info.map[i];
    }
    for (int i = 0; i < info.citiesCounter; i++) {
        delete[] city[i].roadMap;
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

char* getCityName(Information& info, Coords coords) {
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

int queueLen(Queue* head) {
    int len = 0;
    while (head->next != nullptr)
    {
        len++;
        head = head->next;
    }
    return len;
}

void pop(Queue* head) {

}
void addToQueue(Queue* head, Coords pos){
    if (head->next != nullptr) {
        addToQueue(head, pos);
    }
    else {
        Queue* node = new Queue;
        node->coords = pos;
        node->next = nullptr;
        head->next = node;
    }
}
void checkRoadsAround(char road, Information info, Queue* front) {
    Coords newPos;
    if (onTheMap(info, front->coords.y, front->coords.x + 1) && info.map[front->coords.y][front->coords.x + 1] == ROAD) {
        info.map[front->coords.y][front->coords.x + 1] = road;
        newPos.x = front->coords.x + 1;
        newPos.y = front->coords.y;
        addToQueue(front, newPos);
        pop(front);
    }
        
    if (onTheMap(info, front->coords.y, front->coords.x - 1) && info.map[front->coords.y][front->coords.x - 1] == ROAD) {
        info.map[front->coords.y][front->coords.x - 1] = road;
    }
    if (onTheMap(info, front->coords.y - 1, front->coords.x) && info.map[front->coords.y - 1][front->coords.x] == ROAD) {
        info.map[front->coords.y - 1][front->coords.x] = road;
    }

    if (onTheMap(info, front->coords.y + 1, front->coords.x) && info.map[front->coords.y + 1][front->coords.x] == ROAD) {
        info.map[front->coords.y + 1][front->coords.x] = road;
    }

}



char** createRoadMap(Information info, City city) {
    char** roadMap = new char* [info.height];
    for (int i = 0; i < info.height; i++)
        roadMap[i] = new char[info.width];
    
    char road = '0';
    for (int i = 0; i < info.height; i++) {
        for (int j = 0; j < info.height; j++) {
            roadMap[i][j] = road;
        }
    }
    roadMap[city.coords.y][city.coords.x] = ++road;
    Coords pos = city.coords;
    checkRoadsAround(++road, info, pos);
    while(queueLen(info.queue) > 0)
        checkRoadsAround(++road, info, info.queue);

    return roadMap;
}

void getInfoAboutCities(Information info, City& city)    {
    int cityCounter = -1;
    bool found = false;
    for (int j = 0; j < info.height; j++) {
        for (int o = 0; o < info.width; o++) {
            if (info.map[j][o] == CITY)
            {
                cityCounter++;
                if (cityCounter == city.cityNum) {
                    city.coords.x = o;
                    city.coords.y = j;
                    found = true;
                }
            }
            if (found == true)break;
        }
        if (found == true)break;
    }
    city.name = getCityName(info, city.coords);
    city.roadMap = createRoadMap(info, city);

}

City createVertexes(Information info, int i) {
    City city;
    city.cityNum = i;
    getInfoAboutCities(info, city);

    return city;

}

City* createGraph(Information info, Queue* head) {
    City* city = new City[info.citiesCounter];

    for (int i = 0; i < info.citiesCounter; i++) {
        city[i] = createVertexes(info, i);
        for (int o = 0; o < info.height; o++) {
            for (int j = 0; j < info.height; j++) {
                cout<< city[i].roadMap[o][j];
            }
            cout << endl;
        }
        cout << endl << endl;
    }
    
    return city;
}

int main()
{
    Information info = {};
    createMapAndGetInfoFromInput(info);
    Queue* head = NULL;
    City* city = createGraph(info, head);
    

    clean(info, city);
}

