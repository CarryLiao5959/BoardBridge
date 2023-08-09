import socket
import struct
import time
import argparse

class MsgHead:
    def __init__(self, flag, cmd, len):
        self.flag = flag
        self.cmd = cmd
        self.len = len

    def to_bytes(self):
        # assuming that flag is a string of 8 characters, cmd and len are unsigned 32-bit integers
        return struct.pack('8sII', self.flag.encode(), self.cmd, self.len)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('second', type=int, help='the value for second')
    args = parser.parse_args()

    host = '127.0.0.1'
    port = 8080
    client = socket.socket()
    print("client fd:", client.fileno(), "connect to", host, ":", port, "...")

    try:
        client.connect((host, port))
    except socket.error as e:
        print("Connection failed!")
    else:
        print("Connection success!")

    msg_flag = 'hls_s'
    second = args.second
    msg = MsgHead(msg_flag, 8, second)

    # Send header
    sent_bytes = client.send(msg.to_bytes())
    if sent_bytes == 16: 
        print("Header sent success")
    else:
        print("Header sent fail!")
        client.close()
        exit(1)

    buffer_size = 1024*32

    file_num_bytes = client.recv(4)  # An integer in C++ is typically 4 bytes.
    file_num = struct.unpack('i', file_num_bytes)[0]
    print("received file_num: ", file_num)

    startid = second // 10
    id = startid
    while id < startid + file_num:
        cnt = 1
        filename = "./src/"+f'out{id:03}.ts'
        with open(filename, 'ab') as f:
            while True:
                data = client.recv(buffer_size)
                if not data:
                    break 
                f.write(data)
                if cnt%20 == 0:
                    print("write package "+ str(cnt) +" len: " +str(len(data)))
                if len(data) < buffer_size:
                    break
                cnt += 1
            print(f"{filename} received success")
        id += 1

    time.sleep(1)

    client.close()