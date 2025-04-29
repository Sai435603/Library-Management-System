# Library Management System (C++)

A simple console-based Library Management System written in C++, using Object-Oriented Programming (OOP) and the STL, with JSON persistence via [nlohmann/json](https://github.com/nlohmann/json).

---

## 📚 Features

- **CRUD operations** on books  
  - Create (add) a new book  
  - Read (view) all books or a single book by ISBN  
  - Update book details (title, author, issue status)  
  - Delete a book by ISBN  
- **JSON persistence**  
  - Load existing library data from `library.json` on startup  
  - Save current library data back to `library.json` on exit  
- **OOP design**  
  - `Book` class encapsulates book data & serialization  
  - `Library` class manages collection & persistence  
- **STL usage**  
  - `std::vector` for in-memory storage  
  - `std::find_if`, `std::remove_if` algorithms  

---

## ⚙️ Prerequisites

1. A C++17-compatible compiler (e.g. `g++`, `clang++`)  
2. [nlohmann/json single-header](https://github.com/nlohmann/json) (`json.hpp`) placed in your include path  

---

## 🚀 Building

```bash
# Compile
g++ -std=c++17 -o library_app main.cpp

# Run
./library_app
