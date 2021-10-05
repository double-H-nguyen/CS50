import csv
from cs50 import SQL

# Create empty db file
open("shows.db", "w").close()
# Open SQLite database and store into memory
db = SQL("sqlite:///shows.db")

# Execute SQL statement to create table
db.execute("CREATE TABLE shows (id INTEGER, title TEXT, PRIMARY KEY(id))")
db.execute("CREATE TABLE genres (show_id INTEGER, genre TEXT, FOREIGN KEY(show_id) REFERENCES shows(id)")

# Read csv file and add the data into the database
with open("Favorite TV Shows - Form Responses 1.csv", "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        title = row["title"].strip().upper()
        # After inserting a row into a table that has a primary key, SQL will return the new primary id that was created
        id = db.execute("INSERT INTO shows (title) VALUES(?)", title)
        # For each genre for a given show, add genre into genres table
        for genre in row["genres"].split(", "):
            db.execute("INSERT INTO genres (show_id, genre) VALUES(?, ?)", id, GENRE)

# Query using both shows and genres table (subquery)
# SELECT title FROM shows WHERE id IN (SELECT shows_id FROM genres WHERE genre = "Musical")

# Subquery
# SELECT DISTINCT(genre) FROM genres WHERE show_id IN (SELECT id FROM shows WHERE title = "THE OFFICE") ORDER BY genre

# Multistep query
# SELECT title FROM shows WHERE id IN (SELECT show_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = "Steve Carell"))

# JOIN query
# SELECT title FROM people
# JOIN stars ON people.id = stars.person_id
# JOIN shows ON stars.show_id = shows.id
# WHERE name = "Steve Carell"

# INDEXING (decrease query time)
# CREATE INDEX person_index ON stars(person_id)
# CREATE INDEX show_index ON stars(show_id)
# CREATE INDEX name_index ON people(name)
# then execute your query