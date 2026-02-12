import socket
import struct
import sys

HOST = 'localhost'
PORT = 7340
PASSWORD = 'qwerty'

SERVERDATA_AUTH = 3
SERVERDATA_EXECCOMMAND = 2
SERVERDATA_RESPONSE_VALUE = 0


def rcon_packet(id, ptype, body):
    body_encoded = body.encode('utf-8') + b'\x00\x00'
    size = 4 + 4 + len(body_encoded)
    return struct.pack('<iii', size, id, ptype) + body_encoded


def read_packet(sock):
    raw_size = b''
    while len(raw_size) < 4:
        chunk = sock.recv(4 - len(raw_size))
        if not chunk:
            return None
        raw_size += chunk
    size = struct.unpack('<i', raw_size)[0]
    data = b''
    while len(data) < size:
        chunk = sock.recv(size - len(data))
        if not chunk:
            return None
        data += chunk
    id = struct.unpack('<i', data[0:4])[0]
    ptype = struct.unpack('<i', data[4:8])[0]
    body = data[8:-2].decode('utf-8', 'replace')
    return {'id': id, 'type': ptype, 'body': body}


def main():
    command = ' '.join(sys.argv[1:]) if len(sys.argv) > 1 else 'help'

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    s.connect((HOST, PORT))
    print('Connected to %s:%d' % (HOST, PORT))

    # Auth
    s.sendall(rcon_packet(1, SERVERDATA_AUTH, PASSWORD))
    resp1 = read_packet(s)
    resp2 = read_packet(s)

    if resp2 is None or resp2['id'] == -1:
        print('AUTH FAILED')
        s.close()
        return 1

    print('AUTH OK')

    # Command + sentinel
    s.sendall(rcon_packet(2, SERVERDATA_EXECCOMMAND, command))
    s.sendall(rcon_packet(3, SERVERDATA_RESPONSE_VALUE, ''))
    print('Command: ' + command)
    print('---')

    # Read response(s) until sentinel echo (id=3)
    result = ''
    while True:
        try:
            resp = read_packet(s)
            if resp is None:
                break
            if resp['id'] == 3:
                break
            if resp['body']:
                result += resp['body']
        except socket.timeout:
            break

    if result:
        print(result)

    s.close()
    return 0


if __name__ == '__main__':
    sys.exit(main())
