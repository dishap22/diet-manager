# Diet Manager

## Running

Compile `main.cpp`, for e.g. `g++ main.cpp -o DietManager`.

Then run the executable, e.g. `./DietManager`

## To-Do

### Food Database

- [x] The program must maintain a database of basic foods for consumption (e.g., cheddar cheese, hot dogs, whole milk, etc.). To keep things simple, your prototype need only track the following for each basic food: an identifying string, a list of search keywords used to locate the food, and the calories associated with one serving. It should be obvious from the design how this could be easily expanded to include other nutritional information (protein, carbohydrates, fiber, fat, saturated fat, various minerals, vitamins, etc.)

- [ ] The user must have some way to create composite foods from basic foods (e.g., a peanut butter sandwich from two pieces of bread and a serving of peanut butter, or a peanut butter and jelly sandwich from a peanut butter sandwich and a serving of jelly).

- [ ] The database itself may consist of one file with both basic and composite foods, or two files, one for each category of food. The format of the database file(s) is up to you to decide, with the stipulation that these must be text files that can be viewed (and changed if necessary) in a standard text editor

- [ ] The database must be loaded when the program is executed and saved at program exit. The user must be able to save the database during a session without terminating the program.

- [ ] The user must be able to add new basic foods via the user interface. For the prototype, new basic foods will be created by entering the identifier, keywords, and calories. However, in the production version users will be able to download food information from a variety of web sites, either those provided by food producers (e.g., restaurants like McDonalds) or by general diet databases. As these sites all have their own idiosyncratic information formats, you must clearly show how your design could be easily extended to handle an additional web site without changes rippling through
the system.

- [ ] The user must be able to define new composite foods by selecting one or more existing composite and basic foods and giving the number of servings of each selected food. Each composite, like each basic food, will have an identifying string and list of keywords used in searching. The calories per serving are determined by summing the calories of the component foods.

### Daily Logs

- [ ] The program must maintain a log of the foods consumed each day; this log must be read when the program starts, and saved when the program terminates or by a user command. The format of the log is only constrained by the fact that it must be text that can be read and processed by a standard
text editor.

- [ ] Users add food to the log by selecting either a composite or basic food and giving the number of servings. Selection can be done by selecting from the whole list of foods, or by narrowing the selection using keywords. The user must have the option of finding foods that match either all or any of the keywords. Note: The same food may be entered several times in the log for a given day.

- [ ] Users must be able to delete foods from the log (changing the number of servings can be done by deleting a food and then reentering it with the new serving count).

- [ ] Users must be able to undo commands to an indefinite depth (that is, there is no predefined limit on how many commands may be undone other than available memory). The undo information is discarded when the program is terminated (that is, it is not carried over from session to session).

- [ ] Users must be able to select, view and update the information for any date in the log, not just the current date.

### Diet Goal Profile

- [ ] The program must record the user’s gender, height, age, weight and activity level; the latter three must be changeable every day, though the default is to carry over the previous day’s information

- [ ] Based on the information above, the program will compute the target calorie intake for the day. There are several methods for doing this calculation; search the web and incorporate at least two of these in your program. Note that the user must be able to change the method used at any time.

- [ ] At any point, the user can determine the total calories consumed, the target calorie intake, and the difference between these two for whatever date is currently selected. Negative values represent calories available; positive values represent consumption in excess of the target.
