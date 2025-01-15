# Redis-From-Scratch

## Project Overview
This project is a simplified Redis-like in-memory key-value store implemented in C++ with a Python client for interaction. It supports basic operations like `SET`, `GET`, and `DEL`, with data persistence to a file. The server handles multiple client connections using multithreading.

## Features
- **Basic Commands:**
  - `SET <key> <value>`: Stores a key-value pair.
  - `GET <key>`: Retrieves the value for the specified key.
  - `DEL <key>`: Deletes the specified key.
- **Persistence:** Data is saved to and loaded from a file (`database.txt`).
- **Multithreading:** Handles multiple client connections concurrently using `std::thread` and `std::mutex`.

## Technologies Used
- **C++** for server-side implementation
- **Python** for the client
- **Winsock API** for network communication
- **Multithreading:** `std::thread`, `std::mutex`
- **File I/O:** `fstream` for data persistence

## Build Instructions
### Prerequisites
- Windows Operating System
- C++ compiler (e.g., MSVC for Windows)
- Python 3.x
- CMake
- Visual Studio Build Tools or MinGW (for `make` support)

### Building with CMake
1. Clone the repository:
   ```bash
   git clone https://github.com/TateCharboneau/Redis-From-Scratch
   cd Redis-From-Scratch
   ```
2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
3. Configure and build the project:
   ```bash
   cmake ..
   cmake --build .
   ```

### Alternative: Building with MinGW Make
1. Install MinGW and add it to your system PATH.
2. Open the MinGW terminal in the project directory.
3. Run:
   ```bash
   mingw32-make
   ```

## Running the Project
### Start the Server
```bash
cd build/Release
.\RedisServer.exe
```

### Run the Python Client
1. Open a new terminal window.
2. Navigate to the project directory.
3. Run the client:
   ```bash
   python RedisClient.py
   ```

### Example Usage
```bash
Enter command (or 'QUIT' to exit): SET name Alice
Server response: OK

Enter command (or 'QUIT' to exit): GET name
Server response: Alice

Enter command (or 'QUIT' to exit): DEL name
Server response: OK
```

## Learning Outcomes
- Implemented a network server with socket programming.
- Managed concurrent client connections using multithreading.
- Ensured data consistency with thread-safe operations.
- Integrated file I/O for data persistence.

## Potential Future Improvements
- Expand supported commands (e.g., `EXPIRE`, `INCR`).
- Implement more robust error handling.
- Port to Unix-based systems with cross-platform sockets.

## License
This project is open-source and free to use under the MIT License.

