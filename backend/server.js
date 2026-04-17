const express = require('express');
const cors = require('cors');

const app = express();
app.use(express.json());
app.use(cors());

/* ───────── MOCK DATABASE ───────── */
let students = [
  { id: "S1", name: "Pooja", email: "pooja@uni.com", year: 2, semester: 3, password: "123" }
];

let teachers = [
  { id: "T1", name: "Dr. Sharma", password: "123" }
];

let subjects = [
  { id: "C1", name: "DSA", teacherId: "T1", year: 2, semester: 3 }
];

let enrollments = [
  { studentId: "S1", subjectId: "C1", marks: 85 }
];

/* ───────── LOGIN ───────── */
app.post('/api/login/student', (req, res) => {
  const { id, password } = req.body;
  const s = students.find(x => x.id === id && x.password === password);
  if (!s) return res.status(401).json({ error: "Invalid student login" });
  res.json({ student: s });
});

app.post('/api/login/teacher', (req, res) => {
  const { id, password } = req.body;
  const t = teachers.find(x => x.id === id && x.password === password);
  if (!t) return res.status(401).json({ error: "Invalid teacher login" });
  res.json({ teacher: t });
});

app.post('/api/login/admin', (req, res) => {
  const { id, password } = req.body;
  if (id === "admin" && password === "admin")
    return res.json({ admin: true });
  res.status(401).json({ error: "Invalid admin login" });
});

/* ───────── STUDENT DASHBOARD ───────── */
app.get('/api/student/:id/dashboard', (req, res) => {
  const student = students.find(s => s.id === req.params.id);

  const subs = enrollments
    .filter(e => e.studentId === student.id)
    .map(e => {
      const sub = subjects.find(s => s.id === e.subjectId);
      return {
        subjectId: sub.id,
        subjectName: sub.name,
        marks: e.marks,
        classAvg: 75,
        grade:
          e.marks >= 90 ? "A+" :
          e.marks >= 80 ? "A" :
          e.marks >= 70 ? "B" :
          e.marks >= 60 ? "C" : "F"
      };
    });

  res.json({ student, subjects: subs });
});

/* ───────── TEACHER ───────── */
app.get('/api/teacher/:id/subjects', (req, res) => {
  const data = subjects.filter(s => s.teacherId === req.params.id);
  res.json({ subjects: data });
});

app.get('/api/teacher/subject/:id/enrollments', (req, res) => {
  const subject = subjects.find(s => s.id === req.params.id);

  const list = enrollments
    .filter(e => e.subjectId === subject.id)
    .map(e => {
      const st = students.find(s => s.id === e.studentId);
      return {
        studentId: st.id,
        name: st.name,
        marks: e.marks,
        grade: e.marks >= 80 ? "A" : "B"
      };
    });

  res.json({
    subject,
    enrollments: list,
    classAvg: 78
  });
});

/* update marks */
app.put('/api/teacher/subject/:sid/marks/:stud', (req, res) => {
  const e = enrollments.find(x =>
    x.subjectId === req.params.sid &&
    x.studentId === req.params.stud
  );
  if (e) e.marks = req.body.marks;
  res.json({ success: true });
});

/* add enrollment */
app.post('/api/teacher/subject/:sid/marks', (req, res) => {
  enrollments.push({
    subjectId: req.params.sid,
    studentId: req.body.studentId,
    marks: req.body.marks
  });
  res.json({ success: true });
});

/* delete enrollment */
app.delete('/api/teacher/subject/:sid/enrollment/:stud', (req, res) => {
  enrollments = enrollments.filter(e =>
    !(e.subjectId === req.params.sid && e.studentId === req.params.stud)
  );
  res.json({ success: true });
});

/* ───────── ADMIN ───────── */
app.get('/api/admin/students', (req, res) => res.json({ students }));
app.get('/api/admin/teachers', (req, res) => res.json({ teachers }));
app.get('/api/admin/subjects', (req, res) => res.json({ subjects }));

app.post('/api/admin/students', (req, res) => {
  students.push(req.body);
  res.json({ success: true });
});

app.post('/api/admin/teachers', (req, res) => {
  teachers.push(req.body);
  res.json({ success: true });
});

app.post('/api/admin/subjects', (req, res) => {
  subjects.push(req.body);
  res.json({ success: true });
});

app.delete('/api/admin/students/:id', (req, res) => {
  students = students.filter(s => s.id !== req.params.id);
  res.json({ success: true });
});

app.delete('/api/admin/teachers/:id', (req, res) => {
  teachers = teachers.filter(t => t.id !== req.params.id);
  res.json({ success: true });
});

app.delete('/api/admin/subjects/:id', (req, res) => {
  subjects = subjects.filter(s => s.id !== req.params.id);
  res.json({ success: true });
});

/* ───────── START ───────── */
app.listen(3000, () =>
  console.log("Server running at http://localhost:3000")
);