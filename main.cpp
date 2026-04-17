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
*  AVL TEACHER  */

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
/*  FILE LOAD  */
void loadStudents(){
    ifstream f("students.txt");
    if(!f) return;
    string id,name,email,pass; int y,s;
    while(f>>id>>name>>email>>pass>>y>>s)
        studentRoot=insertS(studentRoot,newS(id,name,email,pass,y,s));
}

void loadTeachers(){
    ifstream f("teachers.txt");
    if(!f) return;
    string id,name,pass;
    while(f>>id>>name>>pass)
        teacherRoot=insertT(teacherRoot,newT(id,name,pass));
}
void loadSubjects(){
    ifstream f("subjects.txt");
    if(!f) return;
    string line;
    while(getline(f,line)){
        if(line.empty()) continue;
        istringstream ss(line);
        string id,name,tid; int y,s;
        ss>>id>>name>>tid>>y>>s;
        Subject* sub=newSub(id,name,tid,y,s);
        string token;
        while(ss>>token){
            size_t pos=token.find(':');
            if(pos==string::npos) continue;
            string sid=token.substr(0,pos);
            double m=stod(token.substr(pos+1));
            sub->root=insertE(sub->root,sid,m);
        }
        addSubject(subjectHead,sub);
    }
}
/*  INPUT HELPERS  */
double inputMarks(){
    double m;
    while(true){
        cin>>m;
        if(m>=0 && m<=100) return m;
        cout<<"  Invalid marks. Enter value between 0 and 100: ";
    }
}

/*  MENUS  */

void studentMenu(Student* s){
    cout<<"\n+------------------------------------------+\n";
    cout<<"  Welcome, "<<s->name<<"  (ID: "<<s->id<<")\n";
    cout<<"  Year: "<<s->year<<"  |  Semester: "<<s->semester<<"\n";
    cout<<"+------------------------------------------+\n";

    bool found=false;
    Subject* c=subjectHead;
    while(c){
        /* Only show subjects matching student's year and semester */
        if(c->year==s->year && c->semester==s->semester){
            Enrollment* e=findE(c->root,s->id);
            if(e){
                found=true;
                auto a=avg(c->root);
                double subAvg=a.second?a.first/a.second:0;
                cout<<"\n  Subject : "<<c->id<<" - "<<c->name<<"\n";
                cout<<"  Marks   : "<<e->marks<<" | Grade: "<<getGrade(e->marks)<<"\n";
                cout<<"  ClassAvg: "<<fixed<<setprecision(2)<<subAvg<<"\n";
            }
        }
        c=c->next;
    }
    if(!found) cout<<"  No enrollments found.\n";
    cout<<"+------------------------------------------+\n";
}

void teacherMenu(Teacher* t){
    while(true){
        cout<<"\n+------------------------------------------+\n";
        cout<<"  Teacher: "<<t->name<<"  (ID: "<<t->id<<")\n";
        cout<<"+------------------------------------------+\n";
        cout<<"  Your Subjects:\n";

        Subject* c=subjectHead;
        bool any=false;
        while(c){
            /* FIX: only show subjects assigned to this teacher */
            if(c->teacherId==t->id){
                cout<<"  "<<c->id<<" - "<<c->name
                    <<"  [Y"<<c->year<<" S"<<c->semester<<"]\n";
                any=true;
            }
            c=c->next;
        }
        if(!any){ cout<<"  No subjects assigned.\n"; }

        cout<<"\n  Enter Subject ID (or EXIT): ";
        string id; cin>>id;
        if(id=="EXIT") return;

        Subject* s=subjectHead;
        /* FIX: also verify the subject belongs to this teacher */
        while(s && (s->id!=id || s->teacherId!=t->id)) s=s->next;
        if(!s){ cout<<"  Invalid subject or not yours.\n"; continue; }

        while(true){
            cout<<"\n  Subject: "<<s->id<<" - "<<s->name<<"\n";
            cout<<"  1.View Enrollments  2.Add Marks  3.Update Marks\n";
            cout<<"  4.Delete Enrollment 5.Class Avg  6.Back\n";
            cout<<"  Choice: ";
            int ch; cin>>ch;

            if(ch==6) break;

            if(ch==1){
                if(!s->root){ cout<<"  No enrollments.\n"; continue; }
                cout<<"\n  --- Enrollments ---\n";
                printE(s->root);
            }

            else if(ch==2){
                string sid; double m;
                cout<<"  Student ID: "; cin>>sid;
                /* Validate student exists */
                if(!findS(studentRoot,sid)){
                    cout<<"  Student not found.\n"; continue;
                }
                cout<<"  Marks (0-100): "; m=inputMarks();
                s->root=insertE(s->root,sid,m);
                saveSubjects();
                cout<<"  Marks added.\n";
            }

            else if(ch==3){
                string sid; double m;
                cout<<"  Student ID: "; cin>>sid;
                Enrollment* e=findE(s->root,sid);
                if(!e){ cout<<"  Enrollment not found.\n"; continue; }
                cout<<"  New Marks (0-100): "; m=inputMarks();
                e->marks=m;
                saveSubjects();
                cout<<"  Marks updated.\n";
            }

            else if(ch==4){
                string sid; cout<<"  Student ID: "; cin>>sid;
                if(!findE(s->root,sid)){ cout<<"  Enrollment not found.\n"; continue; }
                s->root=deleteE(s->root,sid);
                saveSubjects();
                cout<<"  Enrollment deleted.\n";
            }

            else if(ch==5){
                auto a=avg(s->root);
                cout<<"  Class Average: "<<fixed<<setprecision(2)
                    <<(a.second?a.first/a.second:0)<<"\n";
            }
        }
    }
}
/*  ADMIN  */

void adminMenu(){
    while(true){
        cout<<"\n+------------------------------------------+\n";
        cout<<"  ADMIN PANEL\n";
        cout<<"+------------------------------------------+\n";
        cout<<"  1. Add Student       2. Delete Student\n";
        cout<<"  3. Add Teacher       4. Delete Teacher\n";
        cout<<"  5. Add Subject       6. Delete Subject\n";
        cout<<"  7. List All Students 8. List All Teachers\n";
        cout<<"  9. List All Subjects 0. Back\n";
        cout<<"  Choice: ";
        int ch; cin>>ch;

        if(ch==0) return;

        if(ch==1){
            string id,name,email,pass; int y,s;
            cout<<"  ID: ";cin>>id;
            /* FIX: check duplicate */
            if(findS(studentRoot,id)){ cout<<"  ID already exists.\n"; continue; }
            cout<<"  Name: ";cin>>name;
            cout<<"  Email: ";cin>>email;
            cout<<"  Password: ";cin>>pass;
            cout<<"  Year: ";cin>>y;
            cout<<"  Semester: ";cin>>s;
            studentRoot=insertS(studentRoot,newS(id,name,email,pass,y,s));
            saveStudents();
            cout<<"  Student added.\n";
        }

        else if(ch==2){
            string id; cout<<"  ID: ";cin>>id;
            if(!findS(studentRoot,id)){ cout<<"  Student not found.\n"; continue; }
            studentRoot=deleteS(studentRoot,id);
            deleteStudentGlobal(id);
            saveStudents();
            saveSubjects();
            cout<<"  Student deleted from all records.\n";
        }

        else if(ch==3){
            string id,name,pass;
            cout<<"  ID: ";cin>>id;
            if(findT(teacherRoot,id)){ cout<<"  ID already exists.\n"; continue; }
            cout<<"  Name: ";cin>>name;
            cout<<"  Password: ";cin>>pass;
            teacherRoot=insertT(teacherRoot,newT(id,name,pass));
            saveTeachers();
            cout<<"  Teacher added.\n";
        }

        else if(ch==4){
            string id; cout<<"  ID: ";cin>>id;
            if(!findT(teacherRoot,id)){ cout<<"  Teacher not found.\n"; continue; }
            teacherRoot=deleteT(teacherRoot,id);
            saveTeachers();
            cout<<"  Teacher deleted.\n";
        }
else if(ch==5){
            string id,name,tid; int y,s;
            cout<<"  Subject ID: ";cin>>id;
            /* Check duplicate subject id */
            Subject* tmp=subjectHead;
            bool dup=false;
            while(tmp){ if(tmp->id==id){dup=true;break;} tmp=tmp->next; }
            if(dup){ cout<<"  Subject ID already exists.\n"; continue; }
            cout<<"  Name: ";cin>>name;
            cout<<"  Teacher ID: ";cin>>tid;
            if(!findT(teacherRoot,tid)){ cout<<"  Teacher not found.\n"; continue; }
            cout<<"  Year: ";cin>>y;
            cout<<"  Semester: ";cin>>s;
            addSubject(subjectHead,newSub(id,name,tid,y,s));
            saveSubjects();
            cout<<"  Subject added.\n";
        }

        else if(ch==6){
            string id;
            cout<<"  Subject ID: ";
            cin>>id;
            deleteSubject(id);
            saveSubjects();
        }

        else if(ch==7){
            cout<<"\n  --- Students ---\n";
            function<void(Student*)> pr=[&](Student* r){
                if(!r) return;
                pr(r->left);
                cout<<"  "<<r->id<<" | "<<r->name
                    <<" | Y"<<r->year<<" S"<<r->semester<<"\n";
                pr(r->right);
            };
            pr(studentRoot);
        }

        else if(ch==8){
            cout<<"\n  --- Teachers ---\n";
            function<void(Teacher*)> pr=[&](Teacher* r){
                if(!r) return;
                pr(r->left);
                cout<<"  "<<r->id<<" | "<<r->name<<"\n";
                pr(r->right);
            };
            pr(teacherRoot);
        }

        else if(ch==9){
            cout<<"\n  --- Subjects ---\n";
            Subject* c=subjectHead;
            while(c){
                cout<<"  "<<c->id<<" | "<<c->name
                    <<" | Teacher: "<<c->teacherId
                    <<" | Y"<<c->year<<" S"<<c->semester<<"\n";
                c=c->next;
            }
        }
    }
}
/*  MAIN  */

int main(){

    /* Load persisted data first */
    loadStudents();
    loadTeachers();
    loadSubjects();

    /* Seed only if files were empty (first run) */
    if(!studentRoot){
        studentRoot=insertS(studentRoot,newS("S1","Amit","amit@uni.com","123",2,3));
        studentRoot=insertS(studentRoot,newS("S2","Ravi","ravi@uni.com","123",2,3));
    }
    if(!teacherRoot){
        teacherRoot=insertT(teacherRoot,newT("T1","Sharma","111"));
    }
    if(!subjectHead){
        Subject* c1=newSub("C1","C++","T1",2,3);
        c1->root=insertE(c1->root,"S1",90);
        c1->root=insertE(c1->root,"S2",80);
        addSubject(subjectHead,c1);
    }

    while(true){
        cout<<"\n==========================================\n";
        cout<<"   STUDENT MANAGEMENT SYSTEM\n";
        cout<<"==========================================\n";
        cout<<"  1. Student Login\n";
        cout<<"  2. Teacher Login\n";
        cout<<"  3. Admin Panel\n";
        cout<<"  4. Exit\n";
        cout<<"  Choice: ";
        int c; cin>>c;

        if(c==4){
            saveStudents();
            saveTeachers();
            saveSubjects();
            cout<<"  Data saved. Goodbye!\n";
            break;
        }

        if(c==1){
            string id,p;
            cout<<"  ID: ";cin>>id;
            cout<<"  Password: ";cin>>p;
            Student* s=findS(studentRoot,id);
            if(s && s->password==p) studentMenu(s);
            else cout<<"  Invalid ID or password.\n";
        }

        else if(c==2){
            string id,p;
            cout<<"  ID: ";cin>>id;
            cout<<"  Password: ";cin>>p;
            Teacher* t=findT(teacherRoot,id);
            if(t && t->password==p) teacherMenu(t);
            else cout<<"  Invalid ID or password.\n";
        }

        else if(c==3){
            adminMenu();
        }
    }

    return 0;
}
