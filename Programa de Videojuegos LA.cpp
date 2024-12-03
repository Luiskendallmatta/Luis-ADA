#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Player {
    int id;
    string name;
    int score;
    Player* next;
};

Player* createNode(int id, string name, int score) {
    return new Player{id, name, score, nullptr};
}

void appendNode(Player*& head, int id, string name, int score) {
    Player* newNode = createNode(id, name, score);
    if (head == nullptr) {
        head = newNode;
        return;
    }
    Player* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}

double calculateAverage(Player* head) {
    if (head == nullptr) {
        cout << "La lista está vacía. No se puede calcular el promedio." << endl;
        return 0;
    }
    double sum = 0;
    int count = 0;
    Player* temp = head;
    while (temp != nullptr) {
        sum += temp->score;
        count++;
        temp = temp->next;
    }
    return sum / count;
}

Player* findHighestScore(Player* head) {
    if (head == nullptr) return nullptr;
    Player* highest = head;
    Player* temp = head->next;
    while (temp != nullptr) {
        if (temp->score > highest->score) {
            highest = temp;
        }
        temp = temp->next;
    }
    return highest;
}

Player* findLowestScore(Player* head) {
    if (head == nullptr) return nullptr;
    Player* lowest = head;
    Player* temp = head->next;
    while (temp != nullptr) {
        if (temp->score < lowest->score) {
            lowest = temp;
        }
        temp = temp->next;
    }
    return lowest;
}

void removeBelowAverage(Player*& head, double average) {
    while (head != nullptr && head->score < average) {
        Player* toDelete = head;
        head = head->next;
        delete toDelete;
    }

    Player* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->next->score < average) {
            Player* toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
        } else {
            current = current->next;
        }
    }
}

void freeList(Player*& head) {
    while (head != nullptr) {
        Player* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    Player* head = nullptr;
    string fileName = "C:\\Users\\ASUS TUF GAMING\\Documents\\TRABAJOS\\jugadores.txt";

    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Error al abrir el archivo: " << fileName << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        size_t firstTab = line.find('\t');
        size_t lastTab = line.rfind('\t');

        if (firstTab == string::npos || lastTab == string::npos || firstTab == lastTab) {
            cerr << "Linea invalida (se omite): " << line << endl;
            continue;
        }

        try {
            int id = stoi(line.substr(0, firstTab));
            string name = line.substr(firstTab + 1, lastTab - firstTab - 1);
            int score = stoi(line.substr(lastTab + 1));
            appendNode(head, id, name, score);
        } catch (const exception& e) {
            cerr << "Error al procesar la linea (se omite): " << line << endl;
        }
    }
    inputFile.close();

    double average = calculateAverage(head);
    cout << "Puntuacion Promedio: " << average << endl;

    Player* highest = findHighestScore(head);
    Player* lowest = findLowestScore(head);

    if (highest) {
        cout << "Puntuacion mas Alta: PlayerID=" << highest->id << ", PlayerName=" << highest->name
             << ", Score=" << highest->score << endl;
    }
    if (lowest) {
        cout << "Puntuacion mas Baja: PlayerID=" << lowest->id << ", PlayerName=" << lowest->name
             << ", Score=" << lowest->score << endl;
    }

    clock_t start = clock();
    removeBelowAverage(head, average);
    clock_t end = clock();

    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "Tiempo para eliminar jugadores por debajo del promedio: " << elapsed << " segundos" << endl;

    freeList(head);

    return 0;
}