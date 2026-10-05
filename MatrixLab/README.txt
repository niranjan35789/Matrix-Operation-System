# MatrixLab — Simple C++ Matrix Calculator

This project is designed from the supplied MatrixLab screenshot.

## What is included

- `server.cpp` — small C++ web server + matrix calculations
- `matrix.cpp` — very simple standalone C++ matrix operation code for learning
- `web/index.html` — attractive MatrixLab webpage
- `README.txt` — this guide

## Operations

1. Addition
2. Subtraction
3. Matrix Multiplication
4. Scalar Multiplication
5. Transpose
6. Trace
7. Determinant (1x1, 2x2 and 3x3)
8. Matrix Power

The webpage sends the values to the C++ server. The actual calculations are performed in C++.

## Windows (MinGW / g++)

1. Install MinGW-w64 and make sure `g++` works in Command Prompt.
2. Open Command Prompt inside this MatrixLab folder.
3. Run:

   g++ server.cpp -std=c++17 -lws2_32 -o matrixlab.exe

4. Start:

   matrixlab.exe

5. Open in your browser:

   http://localhost:8080

Keep the Command Prompt running while using the webpage.

## Linux / macOS

Compile:

   g++ server.cpp -std=c++17 -o matrixlab

Run:

   ./matrixlab

Then open:

   http://localhost:8080

## Important beginner note

This is intentionally a learning project. The C++ server is kept small and the matrix algorithms are basic loops, so you can easily understand and modify them.

The server supports matrices up to 4x4 for normal operations. Determinant is intentionally limited to 3x3 to keep the C++ code simple.
