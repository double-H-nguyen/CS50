# Goalie

## Video Demo:  <https://youtu.be/IWRMzsx-BGc>

## Description:
This is a web-based tool that tracks the user's goals and the respective reward for achieving their goal. This tool is intended for goals that require a fixed number of completions to check-off. If the goal is achieved, the user will be able to reward themselves with a clear conscious. Below are examples of suitable goals:
  * "If I eat healthy for 5 days, I can have a cheat day"
  * "If I workout for 30 days, I can buy a new set of gym clothes"
  * "If I study for a total 4 hours today, I can reward myself with a snack from my favorite coffee shop"

`application.py` is the main file in the project. It configures the Flask application, database, and possible routes. It also contains the core logic of the entire application.

`helpers.py` contains useful functions that are called by `application.py` such as form validation, decorate route, and filters. The reason I put them in a separate file was to keep `application.py` readable. Core business logic belonged in `application.py` while functions that "helped" the business logic belonged in `helpers.py`.

`layout.js` and `style.css` were static files used to enable tooltips and custom font-sizes. I decided to keep to one javascript file and css file for this project because the website didn't contain a lot of front-end logic or custom decoration (thanks to Bootstrap).

`register.html` and `login.html` were created to support the user to create an account and log in.

`change_password.html` was added to allow users to change their password at any point. I added `delete_account.html` because I believe that it's the users' right to delete their digital data if they choose to. Deleting the account will also delete of their goals. `account.html` was created to provide a central area where users can modify their account. It also allows future account-related webpages to be listed here.

`add_goal.html`, `index.html`, `update_goal.html`, and `delete_goal.html` serves as the Create, Read, Update, and Delete (CRUD) views. `index.html` initially showed the goals in a table format, but I changed it to a paragraph + progress bar format to make it more mobile friendly.

`error.html` is a default template page the displays the error message from the application. I was planning to make this page more user friendly, but ran out of time.

## Scope
This will be a minimum-viable-product (MVP). I hope to improve upon it after taking the CS50 Web Programming course.

The user will enter their goal, provide a description, number of completions required, and reward.

The user will log their progress by pressing a button to increment their number of completed. When this value is equal to the number of completions, the reward will be unlocked.

## Technology Used
* Flask
  * SQLAlchemy
  * Flask Session
* JavaScript
* CSS
  * Bootstrap
* HTML
* Database
  * SQLite

## New Stuff I Learned During This Project
* How to use SQLAlchemy to create and interface with a Sqlite database via Flask
* How to modify an existing database and keep track of changes (database migration)
* How to use Bootstrap to quickly apply CSS to my html elements