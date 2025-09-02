struct PersonalInfo{
	string fullName;
	string email;
	string phone;
};

struct UserProfile {
	string username;
	string password;
	string secQ;
	string secA;
	string role;
	PersonalInfo info;
};

//convert security answer to lowercase and remove space
string normalize(string s) {
	string result;
	for (char c : s) {
		if (c != ' ') result += tolower(c);
	}
	return result;
}

bool usernameExists(vector<UserProfile>& users, string uname) { 
	for (auto& u : users) { 
		if (u.username == uname) 
			return true;
	}
	return false;
}

// check password strength
bool isStrongPassword(string pwd) {
	regex pattern("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z]).{6,}$");
	return regex_match(pwd, pattern);
}

bool isValidEmail(string email) {
	regex pattern("^[a-zA-Z0-9._%+-]+@gmail\\.com$");
	return regex_match(email, pattern);
}

bool isValidPhone(string phone) {
	regex pattern("^\\+?[0-9]{1,3}[ ]?[0-9]{2,4}-?[0-9]{6,8}$");
	return regex_match(phone, pattern);
}

void saveUsers(vector<UserProfile>& users) {
	ofstream fout("users.txt");
	for (auto& u : users) {
		fout << u.username << "|" << u.password << "|" << u.secQ << "|" << u.secA << "|" << u.role << "|" << u.info.fullName << "|" << u.info.email << "|" << u.info.phone << endl;
	}
	fout.close();
}

void loadUsers(vector<UserProfile>& users) {
	ifstream fin("users.txt");
	string line;
	while (getline(fin, line)) {
		UserProfile u;
		size_t pos = 0, prev = 0;
		vector<string> parts;
		while ((pos = line.find('|', prev)) != string::npos) {
			parts.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		parts.push_back(line.substr(prev)); // last part

		if (parts.size() == 8) {
			u.username = parts[0];
			u.password = parts[1];
			u.secQ = parts[2];
			u.secA = parts[3];
			u.role = parts[4];
			u.info.fullName = parts[5];
			u.info.email = parts[6];
			u.info.phone = parts[7];
			users.push_back(u);
		}
	}
	fin.close();
}

UserProfile* login(vector < UserProfile>& users, string uname, string pwd) {
	for (auto& u : users) {
		if (u.username == uname && u.password == pwd)
			return &u;
	}
	return nullptr;
}

void regAccount(vector<UserProfile>& users) {
	UserProfile u;
	
	cout << "Enter username: ";
	getline(cin,u.username);

	while (usernameExists(users, u.username)) {
		cout << "Username already exist, please re-enter: ";
		getline(cin, u.username);
	}

	do {
		cout << "Enter password (at least 6 chars, must include digit, uppercase, lowercase): ";
		getline(cin, u.password);
	} while (!isStrongPassword(u.password));

	cout << "\nSelect Security Question\n";
	cout << "1. What is your pet's name?\n ";
	cout << "2. What is your favorite color?\n";
	cout << "3. What city were you born in?\n";

	int qChoice;

	cin >> qChoice;
	cin.ignore();

	switch (qChoice) {
	case 1: 
		u.secQ = "What is your pet's name?";
		break;

	case 2: 
		u.secQ = "What is your favorite color?";
		break;

	case 3: 
		u.secQ = "What city were you born in?";
		break;

	default:
		u.secQ = "What is your pet's name? ";
		break;
	}

	cout << "Enter your answer: ";
	getline(cin, u.secA);

	cout << "Enter your full name: ";
	getline(cin, u.info.fullName);

	do {
		cout << "Enter email";
		getline(cin, u.info.email);
	} while (!isValidEmail(u.info.email));

	do {
		cout << "Enter phone number: ";
		getline(cin, u.info.phone);
	} while (!isValidPhone(u.info.phone));

	u.role = "user";
	users.push_back(u);
	cout << "\nAccount created successfully!\n";

	saveUsers(users);
}

//forgor password
void forgotPwd(vector<UserProfile>& users, string uname) {
	for (auto& u : users) {
		if (u.username == uname) {
			cout << "Security Questions: " << u.secQ << endl;
			cout << "Answer: ";
			string ans;
			getline(cin, ans);

			if (normalize(ans) == normalize(u.secA)) {
				cout << "Enter new password: ";
				string newPwd;
				getline(cin, newPwd);
				if (isStrongPassword(newPwd)) {
					u.password = newPwd;
					cout << "Password reset successfully.\n";
				}
				else {
					cout << "New password is not strong enough.\n";
				}
			}
			else {
				cout << "Incorrect answer.";
			}
			return;
		}
	}
	cout << "Username not found.\n";
}

//changed password
void changePwd(UserProfile& user) {
	cout << "Enter current password: ";
	string current;
	getline(cin, current);

	if (current == user.password) {
		cout << "Enter new password: ";
		string newPwd;
		getline(cin, newPwd);

		if (isStrongPassword(newPwd)) {
			user.password = newPwd;
			cout << "Password changed successfully.\n";
		}
		else {
			cout << "New password does not meet requirements.\n";
		}
	}
	else {
		cout << "Incorrect current password.\n";
	}
}

//update personal info
void updateInfo(UserProfile& user) {
	cout << "Update full name: ";
	getline(cin, user.info.fullName);

	do {
		cout << "Update email: ";
		getline(cin, user.info.email);
	} while (!isValidEmail(user.info.email));

	cout << "Update phone: ";
	getline(cin, user.info.phone);

	cout << "Personal info updated successfully!\n";
}

void loginModule(vector<UserProfile>& users) {
	int choice;

	do {
		cout << "\n=== User/Login Module ===\n";
		cout << "1. Register Account\n";
		cout << "2. Login\n";
		cout << "3. Forgot Password\n";
		cout << "4. Exit to Main Menu\n";
		cout << "Choice: ";
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			regAccount(users);
		}
		else if (choice == 2) {
			string uname, pwd;
			cout << "Username: ";
			getline(cin, uname);
			cout << "Password: ";
			getline(cin, pwd);

			UserProfile* user = login(users, uname, pwd);
			if (user) {
				cout << "Login successful! Welcome " << user->info.fullName << "\n";
				int subChoice;
				do {
					cout << "\n--- Account Menu ---\n";
					cout << "1. Change Password\n";
					cout << "2. Update Personal Info\n";
					cout << "3. Logout\n";
					cout << "Choice: ";
					cin >> subChoice;
					cin.ignore();

					if (subChoice == 1) {
						changePwd(*user);
					}
					else if (subChoice == 2) {
						updateInfo(*user);
					}

				} while (subChoice != 3);
			}
			else {
				cout << "Invalid username or password.\n";
			}
		}
		else if (choice == 3) {
			string uname;
			cout << "Enter your username: ";
			getline(cin, uname);
			forgotPwd(users, uname);
		}

	} while (choice != 4);

	saveUsers(users);
}
