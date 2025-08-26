# 🎓 Student–Teacher–Admin Management System (C++ OOP Project)

## 📌 Description
This project is a **Student–Teacher–Admin Management System** written in **C++** as part of the **Object-Oriented Programming course (ECE 1203)** at Rajshahi University of Engineering & Technology.  

It provides a simple academic management platform where:
- **Students** can register for courses, view grades, and calculate their CGPA.
- **Teachers** can assign marks and update their contact info.
- **Admin** can manage students and teachers, view statistics, and export results.  

The system demonstrates **key OOP concepts** including **inheritance, polymorphism, function overloading, static members, exception handling, file handling, friend classes, and virtual inheritance (diamond problem solution)**.

---

## 🚀 Features
- Student course registration, grade viewing, CGPA calculation
- Teacher mark assignment and contact info management
- Admin management of students/teachers, result export, system statistics
- CSV-based file persistence (auto-creates demo data on first run)
- Input validation with exception handling
- Demonstrates advanced OOP concepts with practical examples

---

## 🛠️ Requirements
- C++17 or later  
- Any standard compiler: `g++`, `clang++`, MSVC  
- No external libraries required  

---

## ⚡ How to Run
```bash
git clone https://github.com/TheMustakim/Project-Zero.git
cd <repo-name>
g++ main.cpp -o management_system -std=c++17
./management_system
