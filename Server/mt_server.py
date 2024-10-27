# server.py
import socket
import struct
import threading
import queue
from typing import Tuple
import image_processing

class Server:
    def __init__(self, host='0.0.0.0', port=5050):
        self.host = host
        self.port = port
        self.running = False
        self.server_thread = None
        self.connected = False
        self.send_queue = queue.Queue()
        self.current_connection = None
        
    def _server_thread(self):
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        
        try:
            server_address = (self.host, self.port)
            server_socket.bind(server_address)
            server_socket.listen(1)
            print(f"Server is listening on {self.host}:{self.port}")
            
            while self.running:
                # Wait for a connection
                print("\nWaiting for connection...")
                connection, client_address = server_socket.accept()
                self.current_connection = connection
                self.connected = True
                print("Connection from", client_address)
                
                try:
                    while self.running and self.connected:
                        try:
                            # Check for new data to send with a timeout
                            doubles = self.send_queue.get(timeout=0.1)
                            print(doubles)
                            data = struct.pack('dd', float(doubles[0]), float(doubles[1]))
                            connection.sendall(data)
                            print(f"Sent values: {doubles[0]}, {doubles[1]}")
                        except queue.Empty:
                            continue
                        except ConnectionError:
                            print("Connection lost")
                            self.connected = False
                            break
                finally:
                    connection.close()
                    self.current_connection = None
                    self.connected = False
                    
        except Exception as e:
            print(f"Server error: {e}")
        finally:
            server_socket.close()
            self.running = False
    
    def start(self):
        """Start the server thread"""
        if not self.running:
            self.running = True
            self.server_thread = threading.Thread(target=self._server_thread)
            self.server_thread.daemon = True
            self.server_thread.start()
    
    def stop(self):
        """Stop the server thread"""
        self.running = False
        if self.current_connection:
            self.current_connection.close()
        if self.server_thread:
            self.server_thread.join()
    
    def send_doubles(self, double1: float, double2: float):
        """Send two doubles to the client"""
        if not self.connected:
            return False
        self.send_queue.put((double1, double2))
        return True
    
    def is_connected(self) -> bool:
        """Check if a client is connected"""
        return self.connected

# Example usage
if __name__ == "__main__":
    server = Server()
    server.start()

    try:
        img = image_processing.Detection(server)

        server.stop()
    except KeyboardInterrupt:
        server.stop()   

