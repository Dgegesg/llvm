from flask import Flask, request, redirect, url_for
import os

app = Flask(__name__)
DB_FILE = 'tasks.txt'

def read_tasks():
    if not os.path.exists(DB_FILE):
        return []
    with open(DB_FILE, 'rb') as f:
        return [line.decode('utf-8').strip() for line in f.readlines()]

def write_tasks(tasks):
    with open(DB_FILE, 'wb') as f:
        for task in tasks:
            f.write((task + '\n').encode('utf-8'))

@app.route('/')
def index():
    tasks = read_tasks()
    query = request.args.get('q', '')
    filtered_tasks = [task for task in tasks if query.lower() in task.lower()]
    return f'''
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>To-Do List</title>
    </head>
    <body>
        <h1>To-Do List</h1>
        <form action="/add" method="post">
            <input type="text" name="task" placeholder="Add a new task" required>
            <button type="submit">Add</button>
        </form>
        <form action="/" method="get">
            <input type="text" name="q" placeholder="Search tasks" value="{query}">
            <button type="submit">Search</button>
        </form>
        <ul>
            {''.join(f'<li>{task} <form action="/delete/{task}" method="post" style="display:inline;"><button type="submit">Delete</button></form></li>' for task in filtered_tasks)}
        </ul>
    </body>
    </html>
    '''

@app.route('/add', methods=['POST'])
def add_task():
    task = request.form.get('task')
    if task:
        tasks = read_tasks()
        tasks.append(task)
        write_tasks(tasks)
    return redirect(url_for('index'))

@app.route('/delete/<task>', methods=['POST'])
def delete_task(task):
    tasks = read_tasks()
    tasks = [t for t in tasks if t != task]
    write_tasks(tasks)
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(debug=True)
