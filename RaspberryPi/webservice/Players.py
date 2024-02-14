import sqlite3


dbPath = 'settings/database/agmtdatabase.db'



def create_table():
    conn = sqlite3.connect(dbPath)
    cursor = conn.cursor()
    create_table_query = '''CREATE TABLE IF NOT EXISTS "players" (
                            "id"	INTEGER NOT NULL,
                            "name"	TEXT NOT NULL,
                            "isActive"	INTEGER NOT NULL DEFAULT 0,
                            PRIMARY KEY("id" AUTOINCREMENT)
                        )'''



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
    
    
def update_players(nome, ativo, jogador_id):
    conn = sqlite3.connect(dbPath)
    cursor = conn.cursor()
    cursor.execute('UPDATE players SET name = ?, isActive = ? WHERE id = ?', (nome, ativo, jogador_id))
    conn.commit()
    conn.close()



def get_player_by_id(jogador_id):
    conn = sqlite3.connect(dbPath)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM players WHERE id = ?", (jogador_id,))
    jogador = cursor.fetchone()
    conn.close()
    return jogador



def get_active_players():
    conn = sqlite3.connect(dbPath)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM players WHERE isActive = 1")
    jogadores = cursor.fetchall()
    conn.close()
    return jogadores

