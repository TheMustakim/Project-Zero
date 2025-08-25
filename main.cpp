#include <bits/stdc++.h>
using namespace std;

vector<string> courses = {"Mathematics", "Physics", "Chemistry", "English", "Computer Science"};
vector<int> courseCredits = {3, 3, 3, 3, 3};

// Class using Static Members 
class SystemInfo {
public:
    static int totalUsers;
    static void incrementUserCount() { totalUsers++; }
    static void decrementUserCount() { totalUsers--; }
    static int getUserCount() { return totalUsers; }
};
int SystemInfo::totalUsers = 0;


// Using a Generic Function to display any kinds of info
template<typename T>
void printList(const vector<T> &items, const string &title = "") {

    if (!title.empty()) cout << title << endl;
    for (int i = 0; i < items.size(); i++) {
        cout << i+1 << ". " << items[i] << endl;
    }

}

// Abstract User Class for Blueprint
class User
{
protected:
    int id;
    string name;

public:
    User(int id, string name) : id(id), name(name) {
        SystemInfo::incrementUserCount();
    }

    virtual ~User() {
        SystemInfo::decrementUserCount();
    }

    virtual void menu() = 0;
    int getID() const { return id; }
    string getName() const { return name; }
    
};

// Forward declaring classes and relevant things
class Student;
vector<Student *> students;

class Teacher;
vector<Teacher *> teachers;

class Admin;
Admin *adminUser;

void saveData();


// For Diamond Problem Demonstration
class Person {
protected:
    string contactInfo;
public:
    Person(string info = "") : contactInfo(info) {}
    virtual ~Person() {}
    virtual string getInfo() const { return contactInfo; }
};

class StudentPerson : virtual public Person {
};

class TeacherPerson : virtual public Person {
};


// TeachingAssistant demonstrates diamond problem solved with virtual inheritance
class TeachingAssistant : public StudentPerson, public TeacherPerson {
public:
    TeachingAssistant(string info = "") {
        contactInfo = info; // No ambiguity due to virtual inheritance
    }
};


// safe input validation with try-catch
int inputInt(string msg, int minVal = INT_MIN, int maxVal = INT_MAX, bool allowAny = false)
{
    int x;
    while (true)
    {
        cout << msg;
        try
        {
            if (!(cin >> x)) throw runtime_error("Not a number");

            if (!allowAny && (x < minVal || x > maxVal)) throw out_of_range("Out of range");
            return x;
        }

        catch (exception &e)
        {
            cout << "[ERROR] " << e.what() << ". Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


class Student : public User, public StudentPerson
{
    bool registered = false;
    vector<int> marks;

public:
    Student(int id, string name) : User(id, name) { 
        marks.assign(5, -1); // -1 means teacher have not given marks yet
    }

    void registerCourse()
    {
        if (registered)
        {
            cout << "\n[INFO] Already registered!\n";
            return;
        }
        
        printList(courses, "Available courses:"); // Using generic function
        
        int temp = inputInt("\nEnter your Student ID to confirm (or 0 to cancel): ", 0, INT_MAX, true);
        if (temp == 0)
        {
            cout << "[INFO] Cancelled.\n";
            return;
        }
        if (temp == id)
        {
            registered = true;
            cout << "[SUCCESS] Registration complete!\n";
            saveData();
        }
        else
            cout << "[ERROR] ID mismatch.\n";
    }

    // Function overloading (compile-time polymorphism)
    void viewGrades(bool force = false)
    {
        cout << "\n=== Grades for " << name << " (" << id << ") ===\n";
        if (!registered && !force)
        {
            cout << "Not registered.\n";
            return;
        }
        if (!allMarksGiven() && !force)
        {
            cout << "[INFO] Result not published.\n";
            return;
        }
        for (int i = 0; i < 5; i++){
            cout << courses[i] << " : " << (marks[i] == -1 ? "no marks" : to_string(marks[i])) << " (Credits " << courseCredits[i] << ")\n";
        }
    }
    
    // Overloaded version of viewGrades function
    void viewGrades(const string& message, bool force = false) {
        cout << "\n=== " << message << " ===\n";
        viewGrades(force);
    }

    bool allMarksGiven()
    {
        for (int m : marks){
            if (m == -1){
                return false;
            }
        }
        return true;
    }

    double cgpa()
    {
        if (!registered)
        {
            cout << "[INFO] Not registered.\n";
            return -1;
        }
        if (!allMarksGiven())
        {
            cout << "[INFO] Result not published.\n";
            return -1;
        }

        double totalPoints = 0;
        int totalCredits = 0;

        for (int i = 0; i < marks.size(); i++)
        {
            int mark = marks[i];
            double gp = (mark >= 80) ? 4.0 : (mark >= 70) ? 3.5 : (mark >= 60)   ? 3.0 : (mark >= 50)   ? 2.5 : (mark >= 40)   ? 2.0 : 0.0;

            totalPoints += gp * courseCredits[i];
            totalCredits += courseCredits[i];
        }
        return totalPoints / totalCredits;
    }

    void menu() override
    {
        while (true)
        {
            cout << "\n=== Student Menu: " << name << " (" << id << ") ===\n";
            cout << "1) Register courses\n2) View grades\n3) Calculate CGPA\n4) Logout\n> ";
            int ch = inputInt("", 1, 4);

            if (ch == 4)
                break;
            if (ch == 1)
                registerCourse();
            else if (ch == 2)
                viewGrades();
            else if (ch == 3)
            {
                double g = cgpa();
                if (g != -1)
                    cout << "[CGPA] " << fixed << setprecision(2) << g << "\n";
            }
        }
    }

    string toCSV()
    {
        stringstream ss;
        ss << id << "," << name << "," << (registered ? 1 : 0);
        for (int m : marks)
            ss << "," << m;
        return ss.str();
    }

    void fromCSV(const vector<string> &tokens)
    {
        registered = (tokens[2] == "1");
        for (int i = 0; i < 5; i++)
            marks[i] = stoi(tokens[3 + i]);
    }

    friend class Teacher;
    friend class Admin;
};


class Teacher : public User, public TeacherPerson
{
public:
    Teacher(int id, string name) : User(id, name) {}
    
    void menu() override
    {
        while (true)
        {
            cout << "\n=== Teacher Menu: " << name << " (" << id << ") ===\n";
            cout << "1) Give marks\n2) View/Update Contact Info\n3) Logout\n> ";
            int ch = inputInt("", 1, 3);
            if (ch == 3)
                break;
            if (ch == 1)
            {
                cout << "\nStudents:\n";
                for (int i = 0; i < students.size(); i++)
                    cout << i + 1 << ") " << students[i]->getName() << " (" << students[i]->getID() << ")\n";
                int s = inputInt("Select student: ", 1, students.size()) - 1;
                Student *st = students[s];
                if (!st->registered)
                {
                    cout << "[INFO] Not registered.\n";
                    continue;
                }
                while (true)
                {
                    cout << "\n--- Marks for " << st->getName() << " ---\n";
                    for (int i = 0; i < courses.size(); i++)
                        cout << i + 1 << ") " << courses[i] << " [" << (st->marks[i] == -1 ? "no marks" : to_string(st->marks[i])) << "]\n";
                    cout << "6) Exit\n> ";
                    int c = inputInt("", 1, 6) - 1;
                    if (c == 5)
                        break;
                    int m = inputInt("Enter marks (0-100): ", 0, 100);
                    st->marks[c] = m;
                    cout << "[SUCCESS] Updated!\n";
                    saveData();
                }
            }
            else if (ch == 2)
            {
                string temp;
                cout << "Current contact info: " << contactInfo << endl;
                cout << "Enter new contact info (0 to skip): ";
                cin.ignore();
                getline(cin, temp);
                if(temp == "0") continue;
                contactInfo = temp;
                cout << "Contact info updated!\n";
                saveData();
            }
        }
    }
    
    string toCSV()
    {
        stringstream ss;
        ss << id << "," << name << "," << contactInfo;
        return ss.str();
    }
    
    void fromCSV(const vector<string>& tokens)
    {
        if (tokens.size() > 2)
            contactInfo = tokens[2];
    }
};


class Admin : public User
{
public:
    Admin(int id, string name) : User(id, name) {}
    
    void addStudent() {
        int id = inputInt("Enter student ID: ", 1, INT_MAX);
        string name;
        cout << "Enter student name: ";
        cin.ignore();
        getline(cin, name);
        
        // Check if ID already exists
        for (auto s : students) {
            if (s->getID() == id) {
                cout << "[ERROR] Student ID already exists!\n";
                return;
            }
        }
        
        students.push_back(new Student(id, name));
        cout << "[SUCCESS] Student added!\n";
        saveData();
    }
    
    void addTeacher() {
        int id = inputInt("Enter teacher ID: ", 1, INT_MAX);
        string name;
        cout << "Enter teacher name: ";
        cin.ignore();
        getline(cin, name);
        
        // Check if ID already exists
        for (auto t : teachers) {
            if (t->getID() == id) {
                cout << "[ERROR] Teacher ID already exists!\n";
                return;
            }
        }
        
        teachers.push_back(new Teacher(id, name));
        cout << "[SUCCESS] Teacher added!\n";
        saveData();
    }
    
    void removeStudent() {
        if (students.empty()) {
            cout << "[INFO] No students to remove.\n";
            return;
        }
        
        for (int i = 0; i < students.size(); i++)
            cout << i + 1 << ") " << students[i]->getName() << " (" << students[i]->getID() << ")\n";
            
        int idx = inputInt("Select student to remove (0 to cancel): ", 0, students.size()) - 1;
        if (idx == -1) return;
        
        delete students[idx];
        students.erase(students.begin() + idx);
        cout << "[SUCCESS] Student removed!\n";
        saveData();
    }
    
    void removeTeacher() {
        if (teachers.empty()) {
            cout << "[INFO] No teachers to remove.\n";
            return;
        }
        
        for (int i = 0; i < teachers.size(); i++)
            cout << i + 1 << ") " << teachers[i]->getName() << " (" << teachers[i]->getID() << ")\n";
            
        int idx = inputInt("Select teacher to remove (0 to cancel): ", 0, teachers.size()) - 1;
        if (idx == -1) return;
        
        delete teachers[idx];
        teachers.erase(teachers.begin() + idx);
        cout << "[SUCCESS] Teacher removed!\n";
        saveData();
    }
    
    void menu() override
    {
        while (true)
        {
            cout << "\n=== Admin Menu ===\n";
            cout << "1) View all students\n2) View student detail\n3) Export results\n";
            cout << "4) View system statistics\n5) Add Student\n6) Add Teacher\n";
            cout << "7) Remove Student\n8) Remove Teacher\n9) Logout\n> ";
            int ch = inputInt("", 1, 9);

            if (ch == 9) break;

            switch(ch) {
                case 1: {
                    for (auto s : students)
                    {
                        cout << "ID: " << s->id << ", Name: " << s->name << ", ";
                        if (!s->registered)
                            cout << "[Not registered]\n";
                        else if (s->allMarksGiven())
                            cout << "CGPA=" << fixed << setprecision(2) << s->cgpa() << "\n";
                        else
                            cout << "[Result not published]\n";
                    }
                    break;
                }

                case 2: {
                    for (int i = 0; i < students.size(); i++)
                        cout << i + 1 << ") " << students[i]->getName() << " (" << students[i]->getID() << ")\n";

                    int idx = inputInt("Select student: ", 1, students.size()) - 1;
                    students[idx]->viewGrades("Detailed Student Report", true);

                    if (students[idx]->allMarksGiven())
                        cout << "[CGPA] " << fixed << setprecision(2) << students[idx]->cgpa() << "\n";
                    break;
                }

                case 3: {
                    ofstream fout("results.csv");
                    fout << "ID,Name,Registered";
                    for (auto c : courses)
                        fout << "," << c;
                    fout << ",CGPA\n";
                    for (auto s : students)
                    {
                        fout << s->id << "," << s->name << "," << (s->registered ? 1 : 0);
                        for (int m : s->marks)
                            fout << "," << m;
                        double g = (s->allMarksGiven() ? s->cgpa() : -1);
                        if (g == -1)
                            fout << ",NA\n";
                        else
                            fout << "," << fixed << setprecision(2) << g << "\n";
                    }
                    fout.close();
                    cout << "[SUCCESS] Results exported to results.csv\n";
                    saveData();
                    break;
                }

                case 4: {
                    cout << "System Statistics:\n";
                    cout << "Total users: " << SystemInfo::getUserCount() << endl;
                    cout << "Students: " << students.size() << endl;
                    cout << "Teachers: " << teachers.size() << endl;
                    
                    // Demonstrating diamond problem here
                    TeachingAssistant ta("teaching.assistant@ruet.ac.bd");
                    cout << "Teaching Assistant Contact: " << ta.getInfo() << endl;
                    break;
                }

                case 5:
                    addStudent();
                    break;
                case 6:
                    addTeacher();
                    break;
                case 7:
                    removeStudent();
                    break;
                case 8:
                    removeTeacher();
                    break;
            }
        }
    }
};


class LoginManager
{
public:
    User *login(string role, int id)
    {
        if (role == "student")
            for (auto &s : students)
                if (s->getID() == id)
                    return s;

        if (role == "teacher")
            for (auto &t : teachers)
                if (t->getID() == id)
                    return t;

        if (role == "admin")
            if (adminUser && adminUser->getID() == id)
                return adminUser;
        return nullptr;
    }

    User *loginPrompt(string role)
    {
        while (true)
        {
            int id = (role == "student") ? inputInt("Enter Student ID (0 to go back): ", 0, INT_MAX, true)
                                         : inputInt("Enter ID (0 to go back): ", 0, INT_MAX, true);
            if (id == 0)
                return nullptr;
            User *u = login(role, id);
            if (u)
                return u;
            cout << "[ERROR] Invalid " << role << " ID. Try again.\n";
        }
    }
};

// File handling helping funtions
vector<string> split(const string &s, char delim)
{
    vector<string> out;
    string tmp;
    stringstream ss(s);
    while (getline(ss, tmp, delim))
        out.push_back(tmp);
    return out;
}

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}


void loadData()
{
    // Load or create students.csv
    ifstream fin("students.csv");
    if (!fin.good()) {
        cout << "[INFO] Creating demo students.csv\n";
        ofstream fout("students.csv");
        fout << "ID,Name,Registered,Math,Physics,Chem,English,CS\n";
        fout << "2310047,Mustakim,0,-1,-1,-1,-1,-1\n";
        fout.close();
    } else {
        string line;
        getline(fin, line); // Skip header
        while (getline(fin, line)) {
            auto tokens = split(line, ',');
            if (tokens.size() < 8) continue;
            int id = stoi(tokens[0]);
            string name = tokens[1];
            Student *s = new Student(id, name);
            s->fromCSV(tokens);
            students.push_back(s);
        }
    }
    fin.close();

    // Load or create teachers.csv
    ifstream ft("teachers.csv");
    if (!ft.good()) {
        cout << "[INFO] Creating demo teachers.csv\n";
        ofstream fout("teachers.csv");
        fout << "ID,Name,Contact\n";
        fout << "100,Kim Jung,Kim@jung.com\n";
        fout.close();
    } else {
        string line;
        getline(ft, line); // Skip header
        while (getline(ft, line)) {
            auto tokens = split(line, ',');
            if (tokens.size() < 2) continue;
            int id = stoi(tokens[0]);
            string name = tokens[1];
            Teacher *t = new Teacher(id, name);
            if (tokens.size() > 2) t->fromCSV(tokens);
            teachers.push_back(t);
        }
    }
    ft.close();
}

void saveData()
{
    // Save students
    ofstream fout("students.csv");
    fout << "ID,Name,Registered,Math,Physics,Chem,English,CS\n";
    for (auto s : students)
        fout << s->toCSV() << "\n";
    fout.close();
    
    // Save teachers
    ofstream ft("teachers.csv");
    ft << "ID,Name,Contact\n";
    for (auto t : teachers)
        ft << t->toCSV() << "\n";
    ft.close();
}


int main()
{
    loadData();
    adminUser = new Admin(999, "SuperAdmin");
    LoginManager lm;

    while (true)
    {
        cout << "\n=== Main Menu ===\n1) Student 2) Teacher 3) Admin 4) Exit\n> ";
        int ch = inputInt("", 1, 4);
        
        if (ch == 4) break;

        string role = (ch == 1 ? "student" : ch == 2 ? "teacher" : "admin");

        User *u = lm.loginPrompt(role);

        if (u) u->menu();
    }

    saveData();
    
    // Cleaning up memory
    for (auto s : students) delete s;
    for (auto t : teachers) delete t;
    delete adminUser;
}
