import socket
import time
from random import randint

IP = "172.30.113.98"
PORT = 1500
ADDR = (IP, PORT)
SIZE = 4
FORMAT = "utf-8"


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)

    result = ''

    while True:

        num = randint(0, 9)
        print("Num = " + str(num))

        num = str(num)
        num += '\0'

        num = num.encode()

        #num = int.to_bytes(4, num, "little")
        client.send(num)

        result = client.recv(1).decode()
        print(result)

        time.sleep(1)

        if result == 'W' or result == 'L' or result == 'E':
            break



    client.close()


if __name__ == "__main__":
    main()
