This project, developed in C by myself and two fellow students, was conceived during our Operating Systems course. It utilizes advanced concepts like multithreading and synchronization to seamlessly manage and process bus tickets and information.

Features:
File-based Data Input: The system reads all data from files, minimizing reliance on user input.
Efficient Processing: Leveraging advanced OS concepts, the system efficiently manages and processes thousands of records.
Files Used:

users.txt: Contains user information including IDs, names, and contact numbers.
routes.txt: Contains route information including IDs, names, starting points, and destinations.
buses.txt: Contains bus numbers.
tickets.txt: Contains ticket IDs and prices.
profits.txt: Tracks earned profits.

Project Description:
This system implements five buses, each with a capacity of 50 passengers. Each thread is allocated ten ticket processing tasks per bus. The system offers various options such as booking tickets, checking ticket availability, visualizing bus data, generating random data, and reading data from files. It also displays arrival and departure times for buses. Data is read from files and stored in structures before processing. The system employs advanced libraries such as pthread for multithreading, sys/shm for shared memory, and ipc for inter-process communication. Mutexes are used to address critical section problems.

How you can access it:
Clone the repository.
Compile the source files.
Run the executable. (Ensure the txt files are in the same directory as the compiled file)

Contributions:
We welcome contributions to enhance the functionality and efficiency of the system. Feel free to open issues or pull requests.

Acknowledgments:
We express our gratitude to our instructor for guiding us through the development process and to the open-source community for invaluable resources and inspiration.
