from flask import Flask, redirect, session, render_template
from functools import wraps


def login_required(f):
  """
  Decorate routes to require login.
  https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
  """
  @wraps(f)
  def decorated_function(*args, **kwargs):
    if session.get("user_id") is None:
      return redirect("/login")
    return f(*args, **kwargs)
  return decorated_function


def error(message, code=400):
  """Render error message"""
  return render_template("error.html", msg=message, code=code)