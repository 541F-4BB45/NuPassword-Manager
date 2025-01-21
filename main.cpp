using namespace std;
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

string firstName, lastName, masterPassword, fileName;

char cardinalConverter(int x)
{
    x = (x < 0)? -x : x;
    x = x % 26;
    char converted_char = static_cast<char>(x + 'a');

    return converted_char;
}


int cardinalConverter(char ch)
{
    int x;

    if (isalpha(ch) == false)
    {
        ch = 'a';
    }
    else
    {
        x = static_cast<int>(ch - 'a');
    }

    return x;
}


string shift_cipher_encryption (string plainString)
{
    int stringSize = plainString.length();

    if (stringSize < 8)
    {
        plainString += "password";
        stringSize = plainString.length();
    }

    const int SIZE = stringSize;
    std::vector<int> plainArray(SIZE), encryptedArray(SIZE);
    string encryptedString;
    int key_k = (firstName[0] + lastName[0]) % 26;
    
    for (int i = 0; i < SIZE; i++)
    {
        plainArray[i] = cardinalConverter(plainString[i]);
    }
    
    for (int i = 0; i < SIZE; i++)
    {
        encryptedArray[i] = (plainArray[i] + key_k) % 26;
    }

    for (int i = 0; i < SIZE; i++)
    {
        encryptedString += cardinalConverter(encryptedArray[i]);
    }

    return encryptedString;
}


string affine_cipher_encryption(string plainString)
{
    int stringSize = plainString.length();

    if (stringSize < 8)
    {
        plainString += "password";
        stringSize = plainString.length();
    }

    std::vector<int> plain_array(stringSize), encrypted_array(stringSize);
    int key_a, key_k;
    string cipherText;
    key_a = (cardinalConverter(firstName[0]) * cardinalConverter(lastName[0])) % 26;
    key_k = (cardinalConverter(firstName[0]) + cardinalConverter(lastName[0])) % 26;

    for (int i = 0; i < stringSize; i++)
    {
        plain_array[i] = cardinalConverter(plainString[i]);
        encrypted_array[i] = ((key_a * plain_array[i]) + key_k) % 26;
    }

    for (int i = 0; i < stringSize; i++)
    {
        cipherText += cardinalConverter(encrypted_array[i]);
    }

    return cipherText;
}


bool responseChecker()
{
    char resp;

    do
    {
        cout << "\nEnter 'Y' for yes, 'N' for no or 'E' to exit the program: ";
        cin >> resp;
        resp = tolower(resp);

        switch (resp)
        {
            case 'y':
            {
                return true;
                break;
            }
            case 'n':
            {
                return false;
                break;
            }
            case 'e':
            {
                exit(0);
                break;
            }
            default:
            {
                cout << "\nInvalid input.";
            }
        }
    }while (true);
}


bool loginChecker()
{
    bool correct = false;
    bool response;
    string inputPassword;

    while (true)
    {
        cout << "Enter your master password (without any spaces): ";
        cin >> inputPassword;
        cout << "\nProceed?: \n";
        response = responseChecker();
        if (response == true)
        {
            break;
        }
    }
    
    if (inputPassword == masterPassword)
    {
        correct = true;
    }

    return correct;
}


void loginViewer()
{
    cout << "\nThese are your logins: \n";
    ifstream readFile(fileName);
    string readString;

    if (readFile.is_open() == true)
    {
        for (int i = 0; i < 3; i++)
        {
            readFile >> readString;
        }

        while (readFile.eof() != true)
        {
            getline (readFile, readString);
            cout << readString << endl;
        }

        readFile.close();
    }
}


void loginHandler()
{
    bool response = false, validInput = false;
    string loginName, encryptedString;
    int encryptionMethod;

    while (true)
    {
        cout << "Enter the name of the login: ";
        cin >> loginName;
        cout << "Proceed?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    cout << "\n1. Shift Encryption\n2. Affine Encryption\n";

    while (true)
    {
        do
        {
            cout << "Enter the method of encryption: ";
            cin >> encryptionMethod;

            switch (encryptionMethod)
            {
                case 1:
                {
                    validInput = true;
                    encryptedString = shift_cipher_encryption(loginName);
                    break;
                }
                case 2:
                {
                    validInput = true;
                    encryptedString = affine_cipher_encryption(loginName);
                    break;
                }
                default:
                {
                    cout << "Invalid Input. Try again.\n";
                    break;
                }
            }
        }while (validInput != true);

        cout << "Proceed?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    ofstream writeFile(fileName, ios::app);

    if (writeFile.is_open())
    {
        writeFile << "Login: " << loginName << " Password: " << encryptedString << endl;
    }

    writeFile.close();
    cout << "\nLogin added successfully!\n";
}


bool passwordChecker(string passwordString)
{
    bool validLength = false, numericCheck = false;
    int passwordSize = passwordString.length();
    int numericCount = 0;

    if (passwordSize >= 8)
    {
        validLength = true;
    }

    if (validLength == true)
    {
        for (int i = 0; i < passwordSize; i++)
        {
                if (isdigit(passwordString[i]))
                {
                   numericCount++;
                }
                if (numericCount >= 2)
                {
                    numericCheck = true;
                    break;
                }
         }
    }

    if ((numericCheck == true) && (validLength == true))
    {
        return true;
    }
    else
    {
        return false;
    }
}


void identitySetter()
{
    bool response = false, validPassword = false;
    cout << "\nCreate your profile: \n";

    while (true)
    {
        cout << "Enter your first name (with no spaces): ";
        cin >> firstName;
        cout << "Confirm?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    while (true)
    {
        cout << "Enter your last name (with no spaces): ";
        cin >> lastName;
        cout << "Confirm?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    while (true)
    {
        cout << "Enter your master password (with no spaces): ";
        cin >> masterPassword;
        validPassword = passwordChecker(masterPassword);

        if (validPassword != true)
        {
            cout << "The password should contain atleast 2 numbers and atleast 8 digits. Try again: ";
            continue;
        }

        cout << "Confirm?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    for (int i = 0; i < firstName.length(); i++)
    {
        firstName[i] = tolower(firstName[i]);
    }

    for (int i = 0; i < lastName.length(); i++)
    {
        lastName[i] = tolower(lastName[i]);
    }

    fileName = firstName + lastName + "NPM.txt";
    ofstream writeFile(fileName);
    writeFile << firstName << " " << lastName << " " << masterPassword << endl;
    writeFile.close();
    cout << "\nYour identity has been set!\n";
}


bool identityGetter()
{
    bool response = false, fileExists = false;
    string first_name, last_name, master_password, file_name;
    cout << "\nSign in: \n";

    while (true)
    {
        cout << "Enter your first name (with no spaces): ";
        cin >> first_name;
        cout << "Confirm?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    while (true)
    {
        cout << "Enter your last name (with no spaces): ";
        cin >> last_name;
        cout << "Confirm?: ";
        response = responseChecker();

        if (response == true)
        {
            break;
        }
    }

    for (int i = 0; i < first_name.length(); i++)
    {
        first_name[i] = tolower(first_name[i]);
    }

    for (int i = 0; i < last_name.length(); i++)
    {
        last_name[i] = tolower(last_name[i]);
    }

    file_name = first_name + last_name + "NPM.txt";
    ifstream readFile(file_name);

    if (readFile.is_open())
    {
        fileExists = true;
        readFile >> firstName >> lastName >> masterPassword;

        for (int i = 0; i < firstName.length(); i++)
        {
            firstName[i] = tolower(firstName[i]);
        }

        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = tolower(lastName[i]);
        }

        fileName = firstName + lastName + "NPM.txt";
        readFile.close();
    }

    return fileExists;
}


int main()
{
    char userResponse;
    bool returningUser, validPassword;
    cout << "Are you a returning user?: ";
    userResponse = responseChecker();

    if (userResponse == true)
    {
        returningUser = true;
    }
    else
    {
        returningUser = false;
    }

    if (returningUser != true)
    {
        identitySetter();
        returningUser = true;
    }
    else
    {
       returningUser = identityGetter();

       if (returningUser == true)
       {
            cout << "\nYour file has been found!\nSign in: \n";
            validPassword = loginChecker();

            if (validPassword != true)
            {
                for (int i = 1; i <= 4; i++)
                {
                    cout << "\nIncorrect password. Attempt " << i << " of 4.\n";
                    validPassword = loginChecker();

                    if (validPassword == true)
                    {
                        break;
                    }
                }
            }
            if (validPassword == false)
            {
                cout << "\nYou have entered the incorrect password too many times. Create a new user: \n";
                identitySetter();
                returningUser = true;
            }            
       }
       else
       {
            for (int i = 0; i < 4; i++)
            {
                cout << "\nFile was not found. Try again.\n";
                returningUser = identityGetter();

                if (returningUser == true)
                {
                    break;
                }
            }

            if (returningUser == false)
            {
                cout << "\nNo file was found. Please create a new user profile\n";
                identitySetter();
                returningUser = true;
            }
       }
    }

   while (true)
   {
       cout << "Would you like to add a new login?: ";
       userResponse = responseChecker();

       if (userResponse == true)
       {
           loginHandler();
       }
       else
       {
           loginViewer();
       }
   }

   return 0;
}