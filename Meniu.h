#include <iostream>

using namespace std;

class Meniu {
public:
    static constexpr const char* meniu_auth = "Enter one of the commands below:\n"
                              "1. sign_up <username> <password> <firstname> <lastname> <profile visibility(public/private)>\n"
                              "2. log_in <username> <password>\n"
                              "3. continue\n"
                              "4. quit\n";
    static constexpr const char* meniu_unauth_user = "Because you are not authenticated, "
                                                      "you can only access a few of the app's functionalities.\n"
                                "Enter one of the commands below:\n"
                                "1. search_user\n"
                                "2. back\n"
                                "3. quit";
    static constexpr const char* meniu_auth_user = "Enter one of the commands below:\n"
                              "1. search_user <pattern>\n"
                              "2. share_post <post visibility(public/friends/close_friends)> \"<content>\"\n"
                              "3. delete_post <postId>\n"
                              "4. send_message \"<message>\" <receiver username> [<receiver username> ... ]\n"
                              "5. see_unread_messages\n"
                              "6. edit_profile <field> <new_value>\n"
                              "7. delete_account\n"
                              "8. log_out";
    static constexpr const char* meniu_user_on_user = "Enter one of the commands below:\n"
                                    "1. see_user_details\n"
                                    "2. see_user_posts\n"
                                    "3. add_friend\n"
                                    "4. add_close_friend\n"
                                    "5. send_message\n"
                                    "6. back\n";
    static constexpr const char* meniu_user_on_friend_profile = "Enter one of the commands below:\n"
                              "1. see_user_details\n"
                              "2. see_user_posts\n"
                              "3. remove_friend\n"
                              "4. send_message\n"
                              "5. back\n";
    static constexpr const char* meniu_unauth_user_on_profile = "Because you are not authenticated, "
                                                     "you can only access a few of the app's functionalities.\n"
                                                     "Enter one of the commands below:\n"
                                                     "1. see_user_details\n"
                                                     "2. see_user_posts\n"
                                                     "3. back";
    static constexpr const char* meniu_admin_on_user_profile = "Enter one of the commands below:\n"
                                          "1. see_user_details\n"
                                          "2. see_user_posts\n"
                                          "3. add_friend\n"
                                          "4. add_close_friend\n"
                                          "5. send_message\n"
                                          "6. make_admin\n"
                                          "7. delete_post\n"
                                          "8. delete_user\n"
                                          "9. back\n";
    static constexpr const char* meniu_admin_on_friend_profile = "Enter one of the commands below:\n"
                                    "1. see_user_details\n"
                                    "2. see_user_posts\n"
                                    "3. remove_friend\n"
                                    "4. send_message\n"
                                    "5. make_admin\n"
                                    "6. delete_post\n"
                                    "7. delete_user\n"
                                    "8. back\n";
    static constexpr const char* meniu_admin_on_admin_user_profile= "Enter one of the commands below:\n"
                                                        "1. see_user_details\n"
                                                        "2. see_user_posts\n"
                                                        "3. add_friend\n"
                                                        "4. add_close_friend\n"
                                                        "5. send_message\n"
                                                        "6. delete_post\n"
                                                        "7. delete_user\n"
                                                        "8. back\n";
    static constexpr const char* meniu_admin_on_admin_friend_profile = "Enter one of the commands below:\n"
                                                      "1. see_user_details\n"
                                                      "2. see_user_posts\n"
                                                      "3. remove_friend\n"
                                                      "4. send_message\n"
                                                      "5. delete_post\n"
                                                      "6. delete_user\n"
                                                      "7. back\n";
private:
    Meniu() {}
};
