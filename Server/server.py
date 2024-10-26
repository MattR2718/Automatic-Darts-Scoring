import socket
import threading
import json

HEADER = 64  # Bytes
PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)

def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    
    connected = True
    while connected:
        # Example object to send
        coordinates = [(10, 20), (30, 40), (50, 60)]
        send_object(conn, coordinates)
        
    conn.close()

def send_object(conn, obj):
    # Serialize the object using pickle (converts to byte stream)

    message = json.dumps([{"x": x, "y": y} for x, y in obj]).encode(FORMAT)

    msg_length = len(message)
    
    # Send header (message length) and serialized object
    send_length = str(msg_length).encode(FORMAT)
    send_length += b' ' * (HEADER - len(send_length))
    conn.send(send_length)
    conn.send(message)
    print(f"[SERVER] Sent object: {obj}")

def start():
    server.listen()
    print(f"[LISTENING] Server is listening on {SERVER}")
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.activeCount() - 1}")

print("[STARTING] Server is starting...")
start()
