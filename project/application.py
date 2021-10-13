from flask import Flask, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

#* Index
@app.route('/')
def index():
  msg = "Hello world!"
  return render_template("index.html", msg=msg)


if __name__ == "__main__":
  app.run(debug=True)