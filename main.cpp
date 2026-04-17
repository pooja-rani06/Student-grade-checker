#include <bits/stdc++.h>
using namespace std;

/* ================= STRUCTURES ================= */

struct Student {
    string id,name,email,password;
    int year,semester,height;
    Student *left,*right;
};

struct Teacher {
    string id,name,password;
    int height;
    Teacher *left,*right;
};

struct Enrollment {
    string studentId;
    double marks;
    int height;
    Enrollment *left,*right;
};

struct Subject {
    string id,name,teacherId;
    int year,semester;
    Enrollment* root;
    Subject* next;
};
