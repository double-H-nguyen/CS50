from flask import Flask, redirect, render_template, request, session
from flask_sqlalchemy import SQLAlchemy
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime


#*******************************************
#* Application Configuration
#*******************************************
# Configure application
app = Flask(__name__)
# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
  response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
  response.headers["Expires"] = 0
  response.headers["Pragma"] = "no-cache"
  return response


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


#*******************************************
#* Database Configuration
#*******************************************
# Initialize database - https://flask-sqlalchemy.palletsprojects.com/en/2.x/quickstart/#a-minimal-application
# Create initial database with the following python commands
# 'python'
# 'from application import db'
# 'db.create_all()'
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///goal_tracker.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)


class Users(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  username = db.Column(db.Text, nullable=False)
  password = db.Column(db.Text, nullable=False)
  goals = db.relationship('Goals', backref='users', lazy=True)

  def __repr__(self):
    return f'<User username="{self.username}", goals(length)={len(self.goals)}>'


class Goals(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  title = db.Column(db.Text, nullable=False)
  description = db.Column(db.Text, nullable=True)
  num_of_completions_required = db.Column(db.Integer, nullable=False)
  reward = db.Column(db.Text, nullable=False)
  num_of_completed = db.Column(db.Integer, default=0, nullable=False)
  date_created = db.Column(db.DateTime, default=datetime.utcnow, nullable=False)
  user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)

  def __repr__(self):
    return f'<User id={self.id}, title="{self.title}", description="{self.description}", num_of_completions_required={self.num_of_completions_required}, reward="{self.reward}", num_of_completed="{self.num_of_completed}" date_created={self.date_created}, user_id={self.user_id}>'


#*******************************************
#* Routes
#*******************************************
@app.route('/', methods=['GET'])
def index():
  user_id = 1 #! HARD-CODED user_id
  
  # Query goals
  goals = Goals.query.all()
  return render_template("index.html", goals=goals)


@app.route('/add_goal', methods=['GET', 'POST'])
def add_goal():
  user_id = 1 #! HARD-CODED user_id
  if request.method == "POST":
    title = request.form['title']
    description = request.form['description']
    completions_required = request.form['completions_required']
    reward = request.form['reward']
    #TODO: validate inputs

    # Add goal to database
    new_goal = Goals(title=title, description=description, num_of_completions_required=completions_required, reward=reward, user_id=user_id)
    try:
      db.session.add(new_goal)
      db.session.commit()
      return redirect('/')
    except:
      return "Failed to add goal to database"

  return render_template("add_goal.html")


#TODO: Create Register route
#TODO: Create Login route

if __name__ == "__main__":
  db.create_all() # create tables if they haven't already been created
  app.run(debug=True) # debug mode enabled