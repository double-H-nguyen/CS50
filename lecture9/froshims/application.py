import os
from cs50 import SQL
from flask import Flask, redirect, render_template, request
from flask_mail import Mail, Message

app = Flask(__name__)

# Configure mail
# app.config["MAIL_DEFAULT_SENDER"] = os.getenv("MAIL_DEFAULT_SENDER")
# app.config["MAIL_PASSWORD"] = os.getenv("MAIL_PASSWORD")
# app.config["MAIL_PORT"] = 587 #gmail
# app.config["MAIL_SERVER"] = "smtp.gmail.com"
# app.config["MAIL_USE_TLS"] = True
# app.config["MAIL_USERNAME"] = os.getenv("MAIL_USERNAME")
# mail = Mail(app)

# Create empty db file (will overwrite existing one)
#open("froshims.db", "w").close()
# Open SQLite database and store into memory
db = SQL("sqlite:///froshims.db")

# Execute SQL statement to create table
#db.execute("CREATE TABLE registrants (id INTEGER, name TEXT NOT NULL, sport TEXT NOT NULL, PRIMARY KEY(id))")

SPORTS = [
    "Dodgeball",
    "Flag Football",
    "Soccer",
    "Volleyball",
    "Ultimate Frisbee"
]

@app.route("/")
def index():
    return render_template("index.html", sports=SPORTS)

@app.route("/register", methods=["POST"])
def register():
    name = request.form.get("name")
    if not name:
        return render_template("error.html", message="Missing name")
    sport = request.form.get("sport")
    if not sport:
        return render_template("error.html", message="Missing sport")
    if sport not in SPORTS:
        return render_template("error.html", message="Not an available sport")

    db.execute("INSERT INTO registrants(name, sport) VALUES(?, ?)", name, sport)

    # Send email with message
    # email = request.form.get("email")
    # message = Message("You are registered!", recipients=email)

    return redirect("/registrants")


@app.route("/registrants")
def registrants():
    registrants = db.execute("SELECT * FROM registrants")
    return render_template("registrants.html", registrants=registrants)