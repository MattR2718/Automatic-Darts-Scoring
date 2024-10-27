import socket
import threading
import json
import image_processing

class SocketServer:
    HEADER = 64  # Bytes
    PORT = 5050
    SERVER = socket.gethostbyname(socket.gethostname())
    ADDR = (SERVER, PORT)
    FORMAT = 'utf-8'
    DISCONNECT_MESSAGE = "!DISCONNECT"

    def __init__(self):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind(self.ADDR)
        self.coords = None
        

    def handle_client(self, conn, addr):
        print(f"[NEW CONNECTION] {addr} connected.")
        connected = True
        while connected:
            if self.coords is not None:
                print("Sending data!")
                self.send_object(conn, self.coords)
                self.coords = None

        conn.close()

    def send_coords(self, x, y):
        self.coords = (x,y)
    
     

    def send_object(self, conn, obj):
        # Serialize the object using JSON (converts to byte stream)
        message = json.dumps([{"x": coord[0], "y": coord[1]} for coord in obj]).encode(self.FORMAT)
        msg_length = 40 + len(obj[0][0]) + len(obj[0][1])
        print(msg_length)
        # Send header (message length) and serialized object
        send_length = str(msg_length).encode(self.FORMAT)
        send_length += b' ' * (self.HEADER - len(send_length))
        conn.send(send_length)
        conn.send(message)
        print(f"[SERVER] Sent object: {obj}")

    def start(self):
        self.server.listen()
        print(f"[LISTENING] Server is listening on {self.SERVER}")
        while True:
            conn, addr = self.server.accept()
            thread = threading.Thread(target=self.handle_client, args=(conn, addr))
            thread.start()
            print(f"[ACTIVE CONNECTIONS] {threading.activeCount() - 1}")

if __name__ == "__main__":
    print("[STARTING] Server is starting...")
    server = SocketServer()
    server_thread = threading.Thread(target=server.start)
    server_thread.start()
    print("[SERVER THREAD] Server is running in the background.")
    img = image_processing.Detection(server)
    
