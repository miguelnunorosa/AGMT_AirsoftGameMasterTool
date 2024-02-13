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
    
    cursor.execute('INSERT INTO players (name, isActive) VALUES (?, ?)', (nome, ativo))
    print(f'Nome: {nome}, Ativo: {ativo}')
    conn.commit()
    conn.close()
