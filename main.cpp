#include <bits/stdc++.h>
using namespace std;

/*  STRUCTURES  */

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

/*  UTIL  */

/* --- Student height helpers --- */
int hS(Student* n){return n?n->height:0;}
/* --- Teacher height helpers --- */
int hT(Teacher* n){return n?n->height:0;}
/* --- Enrollment height helpers --- */
int hE(Enrollment* n){return n?n->height:0;}

int mx(int a,int b){return max(a,b);}

/*  GRADE  */

string getGrade(double m){
    if(m>=90) return "A+";
    if(m>=80) return "A";
    if(m>=70) return "B";
    if(m>=60) return "C";
    return "F";
}

/*  CREATE  */

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

/*  AVL STUDENT  */

Student* rRS(Student* y){
    Student* x=y->left; Student* t=x->right;
    x->right=y; y->left=t;
    y->height=1+mx(hS(y->left),hS(y->right));
    x->height=1+mx(hS(x->left),hS(x->right));
    return x;
}
Student* lRS(Student* x){
    Student* y=x->right; Student* t=y->left;
    y->left=x; x->right=t;
    x->height=1+mx(hS(x->left),hS(x->right));
    y->height=1+mx(hS(y->left),hS(y->right));
    return y;
}

int bfS(Student* n){return n?hS(n->left)-hS(n->right):0;}

Student* insertS(Student* r,Student* s){
    if(!r) return s;
    if(s->id<r->id) r->left=insertS(r->left,s);
    else if(s->id>r->id) r->right=insertS(r->right,s);
    else return r;

    r->height=1+mx(hS(r->left),hS(r->right));
    int b=bfS(r);

    if(b>1 && s->id<r->left->id) return rRS(r);
    if(b<-1 && s->id>r->right->id) return lRS(r);
    if(b>1 && s->id>r->left->id){r->left=lRS(r->left);return rRS(r);}
    if(b<-1 && s->id<r->right->id){r->right=rRS(r->right);return lRS(r);}
    return r;
}
Student* findS(Student* r,string id){
    if(!r) return NULL;
    if(id==r->id) return r;
    if(id<r->id) return findS(r->left,id);
    return findS(r->right,id);
}

/*DELETE STUDENT (AVL)  */

Student* minNodeS(Student* r){
    while(r->left) r=r->left;
    return r;
}

Student* deleteS(Student* r,string id){
    if(!r) return NULL;
    if(id<r->id) r->left=deleteS(r->left,id);
    else if(id>r->id) r->right=deleteS(r->right,id);
    else{
        if(!r->left||!r->right){
            Student* t=r->left?r->left:r->right;
            /* FIX: do not shallow-copy node; just unlink and free */
            delete r;
            return t;
        }
        /* In-order successor: copy only data fields, not tree pointers */
        Student* t=minNodeS(r->right);
        r->id       = t->id;
        r->name     = t->name;
        r->email    = t->email;
        r->password = t->password;
        r->year     = t->year;
        r->semester = t->semester;
        r->right=deleteS(r->right,t->id);
    }

    /* Re-balance after deletion */
    r->height=1+mx(hS(r->left),hS(r->right));
    int b=bfS(r);

    if(b>1 && bfS(r->left)>=0) return rRS(r);
    if(b>1 && bfS(r->left)<0){r->left=lRS(r->left);return rRS(r);}
    if(b<-1 && bfS(r->right)<=0) return lRS(r);
    if(b<-1 && bfS(r->right)>0){r->right=rRS(r->right);return lRS(r);}
    return r;
}
* ================= AVL TEACHER ================= */

Teacher* rRT(Teacher* y){
    Teacher* x=y->left; Teacher* t=x->right;
    x->right=y; y->left=t;
    y->height=1+mx(hT(y->left),hT(y->right));
    x->height=1+mx(hT(x->left),hT(x->right));
    return x;
}

Teacher* lRT(Teacher* x){
    Teacher* y=x->right; Teacher* t=y->left;
    y->left=x; x->right=t;
    x->height=1+mx(hT(x->left),hT(x->right));
    y->height=1+mx(hT(y->left),hT(y->right));
    return y;
}

int bfT(Teacher* n){return n?hT(n->left)-hT(n->right):0;}

Teacher* insertT(Teacher* r,Teacher* t){
    if(!r) return t;
    if(t->id<r->id) r->left=insertT(r->left,t);
    else if(t->id>r->id) r->right=insertT(r->right,t);
    else return r; /* duplicate: ignore */

    r->height=1+mx(hT(r->left),hT(r->right));
    int b=bfT(r);

    if(b>1 && t->id<r->left->id) return rRT(r);
    if(b<-1 && t->id>r->right->id) return lRT(r);
    if(b>1 && t->id>r->left->id){r->left=lRT(r->left);return rRT(r);}
    if(b<-1 && t->id<r->right->id){r->right=rRT(r->right);return lRT(r);}
    return r;
}

Teacher* findT(Teacher* r,string id){
    if(!r) return NULL;
    if(id==r->id) return r;
    if(id<r->id) return findT(r->left,id);
    return findT(r->right,id);
}

/* ================= DELETE TEACHER (AVL) ================= */

Teacher* minNodeT(Teacher* r){
    while(r->left) r=r->left;
    return r;
}

Teacher* deleteT(Teacher* r,string id){
    if(!r) return NULL;
    if(id<r->id) r->left=deleteT(r->left,id);
    else if(id>r->id) r->right=deleteT(r->right,id);
    else{
        if(!r->left||!r->right){
            Teacher* t=r->left?r->left:r->right;
            /* FIX: do not shallow-copy node; just unlink and free */
            delete r;
            return t;
        }
        Teacher* t=minNodeT(r->right);
        r->id       = t->id;
        r->name     = t->name;
        r->password = t->password;
        r->right=deleteT(r->right,t->id);
    }

    /* Re-balance after deletion */
    r->height=1+mx(hT(r->left),hT(r->right));
    int b=bfT(r);

    if(b>1 && bfT(r->left)>=0) return rRT(r);
    if(b>1 && bfT(r->left)<0){r->left=lRT(r->left);return rRT(r);}
    if(b<-1 && bfT(r->right)<=0) return lRT(r);
    if(b<-1 && bfT(r->right)>0){r->right=rRT(r->right);return lRT(r);}
    return r;
}

// AVL ENROLLMENT //

Enrollment* rRE(Enrollment* y){
    Enrollment* x=y->left; Enrollment* t=x->right;
    x->right=y; y->left=t;
    y->height=1+mx(hE(y->left),hE(y->right));
    x->height=1+mx(hE(x->left),hE(x->right));
    return x;
}

Enrollment* lRE(Enrollment* x){
    Enrollment* y=x->right; Enrollment* t=y->left;
    y->left=x; x->right=t;
    x->height=1+mx(hE(x->left),hE(x->right));
    y->height=1+mx(hE(y->left),hE(y->right));
    return y;
}

int bfE(Enrollment* n){return n?hE(n->left)-hE(n->right):0;}

Enrollment* insertE(Enrollment* r,string id,double m){
    if(!r) return newE(id,m);
    if(id<r->studentId) r->left=insertE(r->left,id,m);
    else if(id>r->studentId) r->right=insertE(r->right,id,m);
    else{ r->marks=m; return r; } /* update existing */

    r->height=1+mx(hE(r->left),hE(r->right));
    int b=bfE(r);

    if(b>1 && id<r->left->studentId) return rRE(r);
    if(b<-1 && id>r->right->studentId) return lRE(r);
    if(b>1 && id>r->left->studentId){r->left=lRE(r->left);return rRE(r);}
    if(b<-1 && id<r->right->studentId){r->right=rRE(r->right);return lRE(r);}
    return r;
}

Enrollment* findE(Enrollment* r,string id){
    if(!r) return NULL;
    if(id==r->studentId) return r;
    if(id<r->studentId) return findE(r->left,id);
    return findE(r->right,id);
}

/*  DELETE ENROLLMENT (AVL)  */

Enrollment* minNodeE(Enrollment* r){
    while(r->left) r=r->left;
    return r;
}

Enrollment* deleteE(Enrollment* r,string id){
    if(!r) return NULL;
    if(id<r->studentId) r->left=deleteE(r->left,id);
    else if(id>r->studentId) r->right=deleteE(r->right,id);
    else{
        if(!r->left||!r->right){
            Enrollment* t=r->left?r->left:r->right;
            /* FIX: do not shallow-copy node; just unlink and free */
            delete r;
            return t;
        }
        Enrollment* t=minNodeE(r->right);
        r->studentId = t->studentId;
        r->marks     = t->marks;
        r->right=deleteE(r->right,t->studentId);
    }

    /* Re-balance after deletion */
    r->height=1+mx(hE(r->left),hE(r->right));
    int b=bfE(r);

    if(b>1 && bfE(r->left)>=0) return rRE(r);
    if(b>1 && bfE(r->left)<0){r->left=lRE(r->left);return rRE(r);}
    if(b<-1 && bfE(r->right)<=0) return lRE(r);
    if(b<-1 && bfE(r->right)>0){r->right=rRE(r->right);return lRE(r);}
    return r;
}

/*  SUBJECT (Linked List)  */

void addSubject(Subject*& h,Subject* s){
    s->next=h; h=s;
}

void deleteSubject(string id){
    Subject *curr=subjectHead, *prev=NULL;
    while(curr){
        if(curr->id==id){
            if(prev) prev->next=curr->next;
            else subjectHead=curr->next;

            function<void(Enrollment*)> freeTree=[&](Enrollment* r){
                if(!r) return;
                freeTree(r->left);
                freeTree(r->right);
                delete r;
            };
            freeTree(curr->root);
            delete curr;
            cout<<"Subject deleted successfully\n";
            return;
        }
        prev=curr;
        curr=curr->next;
    }
    cout<<"Subject not found\n";
}
/*  DELETE STUDENT FROM ALL SUBJECTS  */
void deleteStudentGlobal(string id){
    Subject* c=subjectHead;
    while(c){
        c->root=deleteE(c->root,id);
        c=c->next;
    }
}

/*  AVG  */

pair<double,int> avg(Enrollment* r){
    if(!r) return {0,0};
    auto L=avg(r->left),R=avg(r->right);
    return {L.first+R.first+r->marks,L.second+R.second+1};
}
/* PRINT  */
void printE(Enrollment* r){
    if(!r) return;
    printE(r->left);
    Student* s=findS(studentRoot,r->studentId);
    string name=s?s->name:r->studentId;
    cout<<"  "<<r->studentId<<" ("<<name<<")"<<" -> "<<r->marks<<" ("<<getGrade(r->marks)<<")\n";
    printE(r->right);
}

/*  FILE SAVE  */
void saveStudents(){
    ofstream f("students.txt");
    function<void(Student*)> dfs=[&](Student* r){
        if(!r) return;
        dfs(r->left);
        /* FIX: save email field too */
        f<<r->id<<" "<<r->name<<" "<<r->email<<" "<<r->password<<" "<<r->year<<" "<<r->semester<<"\n";
        dfs(r->right);
    };
    dfs(studentRoot);
}

void saveTeachers(){
    ofstream f("teachers.txt");
    function<void(Teacher*)> dfs=[&](Teacher* r){
        if(!r) return;
        dfs(r->left);
        f<<r->id<<" "<<r->name<<" "<<r->password<<"\n";
        dfs(r->right);
    };
    dfs(teacherRoot);
}

void saveSubjects(){
    ofstream f("subjects.txt");
    Subject* c=subjectHead;
    while(c){
        f<<c->id<<" "<<c->name<<" "<<c->teacherId<<" "<<c->year<<" "<<c->semester<<" ";
        function<void(Enrollment*)> dfs=[&](Enrollment* r){
            if(!r) return;
            dfs(r->left);
            f<<r->studentId<<":"<<r->marks<<" ";
            dfs(r->right);
        };
        dfs(c->root);
        f<<"\n";
        c=c->next;
    }
}
