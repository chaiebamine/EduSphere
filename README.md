# EduSphere

EduSphere is a comprehensive school management system designed to streamline student, teacher, and administrative workflows. It offers functionalities such as managing student records, handling teacher schedules, maintaining waitlists, tracking attendance, and more, all through a user-friendly CLI interface.

---

## Features

- **Student Management**: Add, update, and delete student records, and manage waitlists efficiently.
- **Teacher Scheduling**: Create, update, and view teacher schedules.
- **Attendance Tracking**: Mark and display attendance for students and teachers.
- **File-Based Data Management**: Persistent data storage using structured text files.
- **Classroom Management**: Assign students to classes and manage group assignments.

---

## Getting Started

### Prerequisites

Ensure the following tools are installed on your system:

- **GCC**: A compiler for building C programs.
- **Git**: To clone the repository.
- **Code Editor** *(Optional)*: For code exploration and customization (e.g., VS Code, Sublime Text).

### Clone the Repository

1. Open a terminal/command prompt.
2. Clone the project repository using the following command:

```bash
git clone https://github.com/chaiebamine/EduSphere.git
```

3. Navigate to the project directory:

```bash
cd EduSphere
```

---

## Project Structure

```
EduSphere/
├──EduSphere/
|	├── code/                 # Source code files (.c, .h)
|	├── data/                 # Data files (e.g., waitlists, attendance records)
|	├── sound/ 		   # Sound files
|	├── Makefile              # For build automation (if provided)			  
├── README.md             # Project documentation
├── Presentation of the project (pptx)
└── execution video
```

---

## Building and Running the Project

### Build Instructions

1. Compile the source code files using GCC:

```bash
gcc -o edusphere src/*.c
```

2. Ensure the required data files are located in the `data/` directory.

### Run the Program

Execute the compiled program:

```bash
./edusphere
```

---

## Usage Instructions

1. **Start the Application**: Follow the CLI prompts to navigate through the functionalities.
2. **Manage Data**:
   - Add, remove, or update students and teachers.
   - Generate schedules.
   - View or edit attendance records.
3. **Save Changes**: All changes are automatically saved to the respective files in the `data/` directory.

---

## Contributing

We welcome contributions to improve EduSphere! To contribute:

1. Fork the repository on GitHub.
2. Create a new branch for your feature/bug fix.
3. Submit a pull request with a detailed description of your changes.

---

## Contact

For any inquiries or issues, please contact:

- **Author**: Amine Chaieb
- **Email**: chaiebamine741@gmail.com
