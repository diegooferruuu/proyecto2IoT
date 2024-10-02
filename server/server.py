import socket
import threading

RANGES = [5, 15, 25, 35]

clients = {}
actuator_socket = None
sensor_socket = None

def handle_client(client_socket, client_address):
    global actuator_socket, sensor_socket
    print(f"[NEW CONNECTION] {client_address} connected.")
    try:
        while True:

            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            if client_socket == actuator_socket:
                print(f"[ACTUATOR] Message received: {message}")
            elif client_socket == sensor_socket:
                print(f"[SENSOR] Message received: {message}")

                if message == "GET_RANGES":
                    response = ','.join(map(str, RANGES)) + '\n'
                    client_socket.send(response.encode('utf-8'))
                    print(f"[SENSOR] Ranges sent: {response}")
                else:
                    if actuator_socket:
                        print(f"[FORWARD] Sending message to actuator: {message}")
                        actuator_socket.send(message.encode('utf-8'))
                    else:
                        print("[ERROR] Actuator not connected.")
    except ConnectionResetError:
        print(f"[DISCONNECTED] {client_address} has disconnected.")
    finally:
        client_socket.close()
        if client_socket == actuator_socket:
            actuator_socket = None
            print("[ACTUATOR DISCONNECTED]")
        elif client_socket == sensor_socket:
            sensor_socket = None
            print("[SENSOR DISCONNECTED]")

# Start the server and accept multiple connections
def start_server():
    global actuator_socket, sensor_socket
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 12345))  # Change the port here
    server.listen(5)
    print("[LISTENING] Server listening on port 12345.")

    while True:
        client_socket, client_address = server.accept()

        # Assign the first connection to the actuator, the second to the sensor
        if not actuator_socket:
            actuator_socket = client_socket
            clients[client_address] = 'ACTUATOR'
            print(f"[ACTUATOR] {client_address} connected.")
        elif not sensor_socket:
            sensor_socket = client_socket
            clients[client_address] = 'SENSOR'
            print(f"[SENSOR] {client_address} connected.")

        # Launch a thread to handle the new connection
        client_handler = threading.Thread(target=handle_client, args=(client_socket, client_address))
        client_handler.start()

if __name__ == "__main__":
    start_server()
