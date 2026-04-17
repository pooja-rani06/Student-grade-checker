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
Student* studentRoot=NULL;
Teacher* teacherRoot=NULL;
Subject* subjectHead=NULL;

/* ================= UTIL ================= */

/* --- Student height helpers --- */
int hS(Student* n){return n?n->height:0;}
/* --- Teacher height helpers --- */
int hT(Teacher* n){return n?n->height:0;}
/* --- Enrollment height helpers --- */
int hE(Enrollment* n){return n?n->height:0;}

int mx(int a,int b){return max(a,b);}

/* ================= GRADE ================= */

string getGrade(double m){
    if(m>=90) return "A+";
    if(m>=80) return "A";
    if(m>=70) return "B";
    if(m>=60) return "C";
    return "F";
}

/* ================= CREATE ================= */

Student* newS(string id,string name,string email,string pass,int y,int s){
    return new Student{id,name,email,pass,y,s,1,NULL,NULL};
}
Teacher* newT(string id,string name,string pass){
    return new Teacher{id,name,pass,1,NULL,NULL};
}
Enrollment* newE(string id,double m){
    return new Enrollment{id,m,1,NULL,NULL};
}
Subject* newSub(string id,string name,string tid,int y,int s){
    return new Subject{id,name,tid,y,s,NULL,NULL};
}

/* ================= AVL STUDENT ================= */

Student* rRS(Student* y){
    Student* x=y->left; Student* t=x->right;
    x->right=y; y->left=t;
    y->height=1+mx(hS(y->left),hS(y->right));
    x->height=1+mx(hS(x->left),hS(x->right));
    return x;
}
