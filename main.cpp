#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <cctype>

// User Data Structure
struct User{
   std::string username;
   std::string password;
};

// Stores the users
std::vector<User> users{};

void ignoreLine()
{
   // Ignores all characters up to next '\n'
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Handles bad input inside a fail std::cin
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

// Handles function when user logins
void insideMenu(std::string &username)
{
   char userChoice{};
   std::cout << "Welcome " << username << '!' << '\n';

   do{
      std::cout << "-------- Pick a number --------" << '\n';
      std::cout << "1. Display your username" << '\n';
      std::cout << "2. Do addition" << '\n';
      std::cout << "3. Delete a user" << '\n';
      std::cout << "4. Log off" << '\n';

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
            while(!(std::cin >> add_1)){
               std::cout << "Enter num 1: ";
               if(!std::cin) {
                  badInput_handler();
               }
            }
            
            std::cout << "Enter num 2: ";
            while(!(std::cin >> add_2)){
               std::cout << "Enter num 2: ";
               if(!std::cin){
                  badInput_handler();
               }
            }
            
            std::cout << add_1 << " + " << add_2 << " = " << add_1 + add_2 << '\n';
         }break;

         case '3':{
            std::string SelectedUser{};
            char safetyCheck{};

            if(users.size() <= 1){
               std::cout << "Unable to locate other users to delete. You are the only user." << '\n';
               break; // Exit the case without deleting
            }
            std::cout << "Which user would you like to delete?" << "\n\n";
            for(User& savedUsers: users){
               std::cout << savedUsers.username << '\n';
            }
            std::cout << std::endl;;
            
            std::cout << "Enter user: ";
            std::cin >> SelectedUser;
            if(!std::cin){
               badInput_handler();
            }

            if(SelectedUser == username){
               std::cout << "You are not able to delete yourself..." << std::endl;
               break;
            }

            do{
               std::cout << "You're about to delete user \"" << SelectedUser << "\"" << " Are you sure? (Y/N): ";
               std::cin >> safetyCheck;
               if(!std::cin){
                  badInput_handler();
               }
            }while(std::tolower(safetyCheck) != 'y' && std::tolower(safetyCheck) != 'n');

            if(std::tolower(safetyCheck) == 'y')
            {
               auto it = std::find_if(users.begin(), users.end(), [&](const User& u){return u.username == SelectedUser;});

               if(it != users.end()){
                  users.erase(it);
                  std::cout << "User \"" << SelectedUser << "\" has been deleted..." << std::endl;

                  // Edits the file
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
                  
               } else {
                  std::cout << "User \"" << SelectedUser << "\" not found.." << std::endl;
               }
               
            } else if(std::tolower(safetyCheck) == 'n'){
               std::cout << "Deletion aborted..." << std::endl;
            }
         }break;

            
         default: break;
      }
      
   } while(userChoice != '4');

   std::cout << "Requesting user " << username << " to log off..." << '\n';
   std::cout << "Log off successful!" << '\n';
}

//  Handles users being registered
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

// Handles users logging-in
bool loginUser(const std::string &username, const std::string &password)
{
   std::ifstream inFile("database.txt");
   if(!inFile.is_open())
   {
      std::cerr << "Could not find file database.txt. Try registering a user" << std::endl;
      std::exit(1);
   }

   // Clears the array preventing double the users to be added per loginUser call
   users.clear();

   // Temporary User object to store onto users vector
   User tempUser;
   
   while(inFile >> tempUser.username >> tempUser.password)
   {
      users.emplace_back(tempUser);
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
      std::cout << "3. Wipe all users" << '\n';
      std::cout << "4. Exit" << '\n';
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
            } else{
               std::cout << "Login failed. Incorrect credentials." << '\n';
            }
         }break;

         case '3':{
            char safetyCheck{};

            do{
               std::cout << "Are you sure you want to wipe all users? (Y/N): ";
               std::cin >> safetyCheck;
               if(!std::cin){
                  badInput_handler();
               }
            }while(std::tolower(safetyCheck) != 'y' && std::tolower(safetyCheck) != 'n');

            if(std::tolower(safetyCheck) == 'y')
            {
               users.clear();

               // Edit file
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

               std::cout << "WIPE SUCCESSFUL!" << std::endl;
            } else if(std::tolower(safetyCheck) == 'n'){
               std::cout << "!!!WIPE DATA ABORTED!!!" << std::endl;
            }
            
         }break;
            
         default: break;
      }
   }while(choice != '4');

   return 0;
}
