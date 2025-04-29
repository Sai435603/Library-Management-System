#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"         // https://github.com/nlohmann/json

using json = nlohmann::json;
using namespace std;

class Book {
public:
    string title, author, isbn;
    bool isIssued;

    Book() = default;
    Book(string t, string a, string i, bool issued=false)
      : title(move(t)), author(move(a)), isbn(move(i)), isIssued(issued) {}

    // serialize to JSON object
    json to_json() const {
        return json{{"title", title},
                    {"author", author},
                    {"isbn", isbn},
                    {"isIssued", isIssued}};
    }
    // construct from JSON object
    static Book from_json(const json& j) {
        return Book{
            j.at("title").get<string>(),
            j.at("author").get<string>(),
            j.at("isbn").get<string>(),
            j.at("isIssued").get<bool>()
        };
    }
};

class Library {
    vector<Book> books;
    const string filename;

public:
    Library(string file) : filename(move(file)) {
        load();        // on startup read existing JSON
    }

    // ----- CRUD operations on in-memory vector -----  
    void create(const Book& b) {
        books.push_back(b);
        cout << "Book added.\n";
    }
    vector<Book> read_all() const { return books; }
    Book* read_one(const string& isbn) {
        auto it = find_if(books.begin(), books.end(),
                         [&](auto& b){ return b.isbn == isbn; });
        return it!=books.end() ? &*it : nullptr;
    }
    bool update(const string& isbn, const Book& newData) {
        if (auto* b = read_one(isbn)) {
            b->title = newData.title;
            b->author = newData.author;
            b->isIssued = newData.isIssued;
            return true;
        }
        return false;
    }
    bool remove(const string& isbn) {
        auto it = remove_if(books.begin(), books.end(),
                           [&](auto& b){ return b.isbn==isbn; });
        if (it!=books.end()) {
            books.erase(it, books.end());
            return true;
        }
        return false;
    }

    // ----- JSON persistence -----  
    void load() {
        ifstream in(filename);
        if (!in.good()) return;              // no file → start empty
        json j; in >> j;
        books.clear();
        for (auto& item : j) books.push_back(Book::from_json(item));
    }
    void save() const {
        json j = json::array();
        for (auto& b : books) j.push_back(b.to_json());
        ofstream out(filename);
        out << setw(2) << j << endl;        // pretty-print
    }
};

int main(){
    Library lib("library.json");
    int choice;
    string t,a,i;

    while(true){
        cout<<"\n1.Add 2.View all 3.View one 4.Update 5.Delete 6.Save & Exit\n";
        cin>>choice; cin.ignore();
        switch(choice){
          case 1:
            cout<<"Title: "; getline(cin,t);
            cout<<"Author: "; getline(cin,a);
            cout<<"ISBN: "; getline(cin,i);
            lib.create(Book(t,a,i));
            break;
          case 2: {
            auto v = lib.read_all();
            for(auto& b:v)
              cout<<b.title<<" | "<<b.author<<" | "<<b.isbn
                  <<" | Issued:"<<b.isIssued<<"\n";
            break;
          }
          case 3:
            cout<<"ISBN to view: "; getline(cin,i);
            if(auto* b=lib.read_one(i))
              cout<<b->title<<" | "<<b->author<<" | "<<b->isbn
                  <<" | Issued:"<<b->isIssued<<"\n";
            else cout<<"Not found\n";
            break;
          case 4:
            cout<<"ISBN to update: "; getline(cin,i);
            if(auto* b=lib.read_one(i)){
              cout<<"New title: "; getline(cin,t);
              cout<<"New author: "; getline(cin,a);
              cout<<"Is issued? 1/0: "; cin>>b->isIssued; cin.ignore();
              lib.update(i, Book(t,a,i,b->isIssued));
              cout<<"Updated\n";
            } else cout<<"Not found\n";
            break;
          case 5:
            cout<<"ISBN to delete: "; getline(cin,i);
            if(lib.remove(i)) cout<<"Deleted\n"; else cout<<"Not found\n";
            break;
          case 6:
            lib.save();
            cout<<"Saved to library.json. Exiting.\n";
            return 0;
          default: cout<<"Invalid\n";
        }
    }
}
