

#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <cctype>
#include <fstream>
#include <sstream>

class ToDoItem {
private:
    int id;
    std::string description;
    bool completed;
    static int next_id;

public:
    ToDoItem() : id(0), description(""), completed(false) {}
    ~ToDoItem() = default;

    bool create(std::string new_description) {
        id = next_id++;
        description = new_description;
        return true;
    }

    int getid() const { return id; }
    std::string getdescription() const { return description; }
    bool iscompleted() const { return completed; }
    void SetCompleted(bool val) { completed = val; }

    // Сериализация в строку
    std::string serialize() const {
        return std::to_string(id) + "," + description + "," + (completed ? "1" : "0");
    }

    // Десериализация из строки
    static ToDoItem deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        ToDoItem item;

        std::getline(iss, token, ',');
        item.id = std::stoi(token);
        std::getline(iss, item.description, ',');
        std::getline(iss, token, ',');
        item.completed = (token == "1");

        // Обновляем next_id
        if (item.id >= next_id) next_id = item.id + 1;
        return item;
    }
};

int ToDoItem::next_id = 1;

int main() {
    std::string input_description;
    int input_id;
    char input_option;
    std::string TodolistVer = "ver/0.2";
    std::list<ToDoItem> ToDoItems;

    // Загрузка данных из файла
    std::ifstream infile("ToDo.txt");
    std::string line;
    while (std::getline(infile, line)) {
        ToDoItems.push_back(ToDoItem::deserialize(line));
    }
    infile.close();

    while (true) {
        system("cls");
        std::cout << "To Do List - " << TodolistVer << "\n\n";
        for (const auto& item : ToDoItems) {
            std::cout << item.getid() << " | "
                << item.getdescription() << " | "
                << (item.iscompleted() ? "done" : "Not done") << "\n";
        }
        if (ToDoItems.empty()) {
            std::cout << "Add your tasks to be done\n\n\n";
        }

        std::cout << "[A]dd a new task\n"
            << "[C]omplete a task\n"
            << "[D]elete a task\n"
            << "[Q]uit\n";

        std::cin >> input_option;
        input_option = tolower(input_option);

        if (input_option == 'a') {
            std::cout << "Add a new description: ";
            std::cin.ignore();
            std::getline(std::cin, input_description);

            ToDoItem newItem;
            newItem.create(input_description);
            ToDoItems.push_back(newItem);
        }
        else if (input_option == 'c') {
            std::cout << "Enter the id of completed task: ";
            std::cin >> input_id;
            for (auto& item : ToDoItems) {
                if (input_id == item.getid()) {
                    item.SetCompleted(true);
                    break;
                }
            }
        }
        else if (input_option == 'd')
        {
            std::cout << "Enter the id of the task you want to be deleted" << std::endl;
            std::cin.clear();
            std::cin.ignore();
            std::cin >> input_id;
            for (auto it = ToDoItems.begin(); it != ToDoItems.end(); it++)
            {
                if (input_id == it->getid()) { ToDoItems.erase(it); break; }
            }
        }
        else if (input_option == 'q') {
            // Сохранение данных в файл
            std::ofstream file("ToDo.txt");
            for (const auto& item : ToDoItems) {
                file << item.serialize() << "\n";
            }
            file.close();
            break;
        }
    }

    return 0;
}