import sqlite3

dbPath = 'settings/database/agmtdatabase.db'



def get_players():
    conn = sqlite3.connect(dbPath)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM players")
    players = cursor.fetchall()
    conn.close()
    return players



def insert_players(nome, ativo):
    conn = sqlite3.connect(dbPath)
    cursor = conn.cursor()
    
    cursor.execute('INSERT INTO jogadores (nome, ativo) VALUES (?, ?)', (nome, ativo))
    conn.commit()
    conn.close()
    return redirect(url_for('jogadores'))
