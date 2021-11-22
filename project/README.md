# YOUR PROJECT TITLE

## Video Demo:  <URL HERE>

## Description:
This is a web-based tool that tracks the user's goals and the respective reward for achieving their goal. This tool is intended for goals that require a fixed number of completions to check-off. If the goal is achieved, the user will be able to reward themselves with a clear conscious. Below are examples of suitable goals:
* "If I eat healthy for 5 days, I can have a cheat day"
* "If I workout for 30 days, I can buy a new set of gym clothes"
* "If I study for a total 4 hours today, I can reward myself with a snack from my favorite coffee shop"

## Scope
This will be a minimum-viable-product (MVP). I hope to improve upon it after taking the CS50 Web Programming course.

The user will enter their goal, provide a description (optional), number of completions required, and reward.

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
* How to create a progress bar using CSS