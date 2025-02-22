import asyncio
import json
from dataclasses import dataclass
from typing import List, Optional

@dataclass
class MissionItem:
    latitude: float
    longitude: float
    altitude: float

class AsyncSocketClient:
    def __init__(self, host: str = 'localhost', port: int = 14001):
        self.host = host
        self.port = port
        self.reader: Optional[asyncio.StreamReader] = None
        self.writer: Optional[asyncio.StreamWriter] = None
    
    async def connect(self):
        """Establish connection with the C++ server"""
        try:
            self.reader, self.writer = await asyncio.open_connection(
                self.host, self.port
            )
            print(f"Connected to {self.host}:{self.port}")
        except Exception as e:
            print(f"Connection failed: {e}")
            raise

    async def receive_data(self):
        """Continuously receive and process data from the server"""
        try:
            while True:
                # Read data from the socket
                data = await self.reader.read(1024)
                if not data:
                    print("Connection closed by server")
                    break

                # Try to parse as JSON first (for mission items)
                try:
                    json_data = json.loads(data.decode())
                    if isinstance(json_data, list):
                        # Process mission items
                        mission_items = [
                            MissionItem(item['latitude'], item['longitude'], item['altitude'])
                            for item in json_data
                        ]
                        print("Received mission items:")
                        for item in mission_items:
                            print(f"  Lat: {item.latitude}, Lon: {item.longitude}, Alt: {item.altitude}")
                    else:
                        print(f"Received JSON data: {json_data}")
                except json.JSONDecodeError:
                    # Not JSON, try to parse as regular command
                    try:
                        message = data.decode()
                        if "heartbeat" in message.lower():
                            print("Received heartbeat")
                        elif "param" in message.lower():
                            # Parse parameter setting
                            # Assuming format similar to what's shown in the C++ code
                            print(f"Received parameter setting: {message}")
                        else:
                            print(f"Received message: {message}")
                    except UnicodeDecodeError:
                        print(f"Received binary data: {data}")

        except asyncio.CancelledError:
            print("Receive task cancelled")
            raise
        except Exception as e:
            print(f"Error receiving data: {e}")
            raise
        finally:
            if self.writer:
                self.writer.close()
                await self.writer.wait_closed()

async def main():
    client = AsyncSocketClient()
    try:
        await client.connect()
        await client.receive_data()
    except KeyboardInterrupt:
        print("\nClient stopped by user")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    asyncio.run(main())
