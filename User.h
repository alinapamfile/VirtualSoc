class User {
public:
    string username;
    string password;
    string firstname, lastname;
    string country, city;
    string occupation;
    string profileVisibility;
    string isAdmin;

    User(const string &username, const string &password, const string &firstname, const string &lastname,
         const string &country, const string &city, const string &occupation, const string &profileVisibility,
         const string &isAdmin) : username(username), password(password), firstname(firstname), lastname(lastname),
                                  country(country), city(city), occupation(occupation),
                                  profileVisibility(profileVisibility), isAdmin(isAdmin) {}
};