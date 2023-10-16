#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

// Data structure for every user
struct User{
   std::string username;
   std::string password;
};

// Stores the users
std::vector<User> users;

void ignoreLine()
{
   // Ignores all characters up to next '\n'
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void badInput_handler()
{
   if(std::cin.eof())
   {
      std::cout << "EXITING!" << std::endl;
      std::exit(0);
   }

   std::cin.clear();	// put std::cin back to "normal" operation mode
   ignoreLine();		// Removes bad input
}

void insideMenu(std::string &username)
{
   char userChoice;
   std::cout << "Welcome " << username << '!' << '\n';

   do{
      std::cout << "-------- Pick a number --------" << '\n';
      std::cout << "1. Display your username" << '\n';
      std::cout << "2. Do addition" << '\n';
      std::cout << "3. Log off" << '\n';

      std::cin >> userChoice;
      if(!std::cin){
         badInput_handler();
      }

      switch(userChoice)
      {
         case '1':{
            std::cout << "You are " << username << '\n';
         }break;

         case '2':{
            int add_1{ 0 }, add_2{ 0 };
            std::cout << "Enter num 1: ";
            std::cin >> add_1;
            if(!std::cin) {
               badInput_handler();
            }
            
            std::cout << "Enter num 2: ";
            std::cin >> add_2;
            if(!std::cin){
               badInput_handler();
            }
            
            std::cout << add_1 << " + " << add_2 << " = " << add_1 + add_2 << '\n';
         }break;

            
         default: break;
      }
      
   } while(userChoice != '3');

   std::cout << "Requesting user " << username << " to log off..." << '\n';
   std::cout << "Log off successful!" << '\n';

   
}

void registerUser(const std::string &username, const std::string &password)
{
   users.push_back({username, password});

   std::ofstream outFile("database.txt");
   if(!outFile.is_open())
   {
      std::cerr << "Error opening file!" << std::endl;
      std::exit(1);
   }

   for(const User &logins: users)
   {
      outFile << logins.username << ' ' << logins.password << '\n';
   }
   outFile.close();
}

bool loginUser(const std::string &username, const std::string &password)
{
   std::ifstream inFile("database.txt");
   if(!inFile.is_open())
   {
      std::cerr << "Failed to open database.txt" << std::endl;
      exit(1);
   }

   User tempUser;
   
   while(inFile >> tempUser.username >> tempUser.password)
   {
      users.push_back(tempUser);
   }

   inFile.close();
   
   for(const User& user: users)
   {
      
      if(user.username == username && user.password == password)
      {
         return true; // User authenticated
      }
   }

   return false; // Authentication failed
}


int main()
{
   char choice;

   do{
      std::cout << "Choose an option: " << '\n';
      std::cout << "1. Register" << '\n';
      std::cout << "2. Log in" << '\n';
      std::cout << "3. Exit" << '\n';
      std::cin >> choice;
      if(!std::cin){
         badInput_handler();
      }


      switch(choice)
      {
         case '1':{
            std::string username, password;
            std::cout << "Enter a username: ";
            std::cin >> username;

            if(!std::cin){
               badInput_handler();
            }
            
            std::cout << "Enter a password: ";
            std::cin >> password;

            if(!std::cin){
               badInput_handler();
            }
            
            registerUser(username, password);
         }break;

         case '2':{
            std::string username, password;
            std::cout << "Enter your username: ";
            std::cin >> username;
            if(!std::cin){
               badInput_handler();
            }
            
            std::cout << "Enter your password: ";
            std::cin >> password;
            if(!std::cin){
               badInput_handler();
            }
            
            if(loginUser(username, password)){
               std::cout << "Login successful!" << '\n';
               insideMenu(username);
            } else {
               std::cout << "Login failed. Incorrect credentials." << '\n';
            }
         }break;
            
         default: break;
      }
   }while(choice != '3');

   
   
   return 0;
}
