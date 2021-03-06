// Jimson Huang
// CS457
// 10/18/2021
// Insert command for inserting tuples into a table, following an existing
// schema.

#pragma once

#include "ICommand.h"

class InsertCommand : public ICommand {
    // Check command prefix
    virtual bool match(std::string input) {
        return to_upper(first_word(input)) == "INSERT";
    };

    // Execute the command
    virtual std::pair<std::string, std::shared_ptr<Transaction>> execute(std::string input, std::string database, std::shared_ptr<Transaction> transaction) {
        std::vector<std::string> parms = split(input);

        // Command formatting check
        if (parms.size() < 4 || to_upper(parms[1]) != "INTO" || (to_upper(parms[3]) != "VALUES" && to_upper(parms[3].substr(0, 7)) != "VALUES(") || remove_semicolon(parms[parms.size()-1]).back() != ')') {
            std::cout << "!INSERT command failed. Bad syntax." << std::endl;
            return default_return;
        }

        // Command fails if no database is selected.
        if (database == "") {
            std::cout << "!INSERT command failed. No database is being used." << std::endl;
            return default_return;
        }

        std::string table = parms[2];

        // Reformat the input string to a tuple of attributes separated by space.
        std::string tuple;
        for (int i = 3; i < parms.size(); i++) {
            // Remove leading or trailing parenthesis, and any whitespace
            if (i == 3) {
                if (to_upper(parms[i]) == "VALUES") {
                    continue;
                }
                else {
                    tuple += remove_ws(remove_parenthesis(parms[i].substr(7, parms[i].length() - 7)));
                }
            }
            else {
                tuple += remove_ws(remove_parenthesis(parms[i]));
            }
        }
        // Change tuple format to vec
        std::vector<std::string> tuple_vec = split(tuple, ",");


        // Try inserting the newly created tuple into the table, error out if not successful.
        bool success;
        try {
            success = Table::insert(database, table, tuple_vec);
        }
        catch (const char * e) {
            std::cout << "!INSERT command failed. Tuple does not fit the table schema." << std::endl;
            return default_return;
        }

        // Command fails if table does not exist
        if (!success) {
            std::cout << "!INSERT command failed. Table does not exist." << std::endl;
            return default_return;
        }

        std::cout << "1 new record inserted." << std::endl;

        return default_return;
    };
};