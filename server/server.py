import socket
import threading

# Predefined range vector
RANGES = [50, 5, 15, 25, 35]

# Dictionary to store connections
clients = {}
actuator_socket = None  # Actuator connection
sensor_socket = None    # Sensor connection

# Handling device connections
def handle_client(client_socket, client_address):
    global actuator_socket, sensor_socket
    print(f"[NEW CONNECTION] {client_address} connected.")

    try:
        while True:
            # Wait for a message from the client
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            # Identify if the client is the sensor or the actuator
            if client_socket == actuator_socket:
                print(f"[ACTUATOR] Message received: {message}")
            elif client_socket == sensor_socket:
                print(f"[SENSOR] Message received: {message}")

                # If the message is GET_RANGES, return the predefined ranges
                if message == "GET_RANGES":
                    response = ','.join(map(str, RANGES))
                    client_socket.send(response.encode('utf-8'))
                    print(f"[SENSOR] Ranges sent: {response}")
                else:
                    # If the sensor sends any other message, forward it to the actuator
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
