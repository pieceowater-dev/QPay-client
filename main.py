import boot
import usocket as socket
import time

WEBSOCKET_HOST = f"{boot.BACKEND_HOST}"

def websocket_client():
    try:
        ws = socket.socket()

        print(WEBSOCKET_HOST)

        addr_info = socket.getaddrinfo(WEBSOCKET_HOST, 443)
        print("Address info:", addr_info)
        
        addr = addr_info[0][-1]
        print("Connecting to:", addr)
        
        ws.connect(addr)
        print("WebSocket connected")

        ws.send(b"ping")
        print(f"Sent 'ping' message to {WEBSOCKET_HOST}")


        while True:
            data = ws.recv(1024)
            if not data:
                break
            
            if b"ping" in data:
                print("Received 'ping' event")

        ws.close()
        print("WebSocket connection closed")


    except Exception as e:
        print("WebSocket error:", e)

        if ws:
            ws.close()

websocket_client()
