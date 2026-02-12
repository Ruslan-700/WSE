#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""OGP (Open Game Protocol) client for testing WSE OGP server. Python 2.7"""

import socket
import struct
import sys
import traceback

OGP_MAGIC = b'\xFF\xFF\xFF\xFF' + b'OGP\x00'

TYPE_PING = 0
TYPE_DEFAULT1 = 1
TYPE_ERROR = 0xFF

ERR_INVALID_CHALLENGE = 3


class BufferReader:
    def __init__(self, data):
        self.data = data
        self.pos = 0

    def remaining(self):
        return len(self.data) - self.pos

    def read_uint8(self):
        val = ord(self.data[self.pos])
        self.pos += 1
        return val

    def read_uint16(self):
        val = struct.unpack_from('<H', self.data, self.pos)[0]
        self.pos += 2
        return val

    def read_uint32(self):
        val = struct.unpack_from('<I', self.data, self.pos)[0]
        self.pos += 4
        return val

    def read_var_bitarray(self):
        result = bytearray()
        while True:
            b = self.read_uint8()
            result.append(b)
            if not (b & 0x80):
                break
        return bytes(result)

    def read_var_uint(self):
        b = self.read_uint8()
        if b < 0xFE:
            return b
        elif b == 0xFE:
            return self.read_uint16()
        else:
            return self.read_uint32()

    def read_var_sint(self):
        b = self.read_uint8()
        if b == 0x80:
            val = struct.unpack_from('<h', self.data, self.pos)[0]
            self.pos += 2
            return val
        elif b == 0x81:
            val = struct.unpack_from('<i', self.data, self.pos)[0]
            self.pos += 4
            return val
        else:
            return b if b < 128 else b - 256

    def read_string(self):
        end = self.data.index(b'\x00', self.pos)
        s = self.data[self.pos:end]
        self.pos = end + 1
        return s

    def read_bytes(self, n):
        result = self.data[self.pos:self.pos + n]
        self.pos += n
        return result


def decode_vba(raw):
    """Decode VarBitArray bytes: clear bit 7 (continuation flag) from each."""
    result = bytearray()
    for b in bytearray(raw):
        result.append(b & 0x7F)
    return bytes(result)


def bit(byte_val, n):
    return bool(byte_val & (1 << n))


def vba_byte(raw, index):
    decoded = decode_vba(raw)
    if index < len(decoded):
        return ord(decoded[index])
    return 0


# ---------- packet builders ----------

def build_ping():
    # HeadLen=3 (includes itself: HeadLen+Type+Flags), Type=0(Ping), Flags=0
    return OGP_MAGIC + b'\x03\x00\x00'


def build_default1_body():
    body = b'\x1F'              # RequestFlags: all
    body += b'\x8F\x8B\x0F'    # ServerInfoFields: all
    body += b'\x1F'             # ModFields: all
    body += b'\x01'             # MapFields: filename
    body += b'\x13'             # TeamListFields: name+score+playercount
    body += b'\x8F\x8D\x05'    # PlayerListFields: flags,slot,name,team,score,kills,death,id,ping
    body += b'\x0F'             # AddOnFields: all
    return body


def build_default1(challenge=None):
    body = build_default1_body()
    if challenge is not None:
        # HeadLen=7 (HeadLen+Type+Flags+Challenge4), Type=1, Flags=0x02(bChallengeNumber)
        header = b'\x07\x01\x02' + struct.pack('<I', challenge)
    else:
        # HeadLen=3 (HeadLen+Type+Flags), Type=1, Flags=0
        header = b'\x03\x01\x00'
    return OGP_MAGIC + header + body


# ---------- response parsing ----------

def parse_header(reader):
    hdr = {}
    head_len_pos = reader.pos
    head_len = reader.read_uint8()
    body_start = head_len_pos + head_len

    hdr['type'] = reader.read_uint8()

    flags_raw = reader.read_var_bitarray()
    fb = vba_byte(flags_raw, 0)

    hdr['bAnswer'] = bit(fb, 0)
    hdr['bChallengeNumber'] = bit(fb, 1)
    hdr['bRequestID'] = bit(fb, 2)
    hdr['bSplit'] = bit(fb, 3)
    hdr['bUTF8'] = bit(fb, 4)

    if hdr['bChallengeNumber']:
        hdr['challenge'] = reader.read_uint32()
    if hdr['bRequestID']:
        hdr['request_id'] = reader.read_uint32()
    if hdr['bSplit']:
        if hdr['bAnswer']:
            hdr['packet_no'] = reader.read_uint8()
            hdr['packet_count'] = reader.read_uint8()
        else:
            hdr['max_bytes'] = reader.read_uint16()

    reader.pos = body_start
    return hdr


def parse_default1_body(reader):
    r = {}
    r['game_id'] = reader.read_uint16()

    rf = reader.read_var_bitarray()
    rf0 = vba_byte(rf, 0)
    has_si = bit(rf0, 0)
    has_tl = bit(rf0, 1)
    has_pl = bit(rf0, 2)
    has_rl = bit(rf0, 3)
    has_al = bit(rf0, 4)

    if has_si:
        si = {}
        sif = reader.read_var_bitarray()
        s0, s1, s2 = vba_byte(sif, 0), vba_byte(sif, 1), vba_byte(sif, 2)

        if bit(s0, 0):  # GameName
            si['game_name'] = reader.read_string()
        if bit(s0, 1):  # ServerFlags
            sf = reader.read_var_bitarray()
            sf0 = vba_byte(sf, 0)
            si['server_type'] = sf0 & 0x03
            si['password'] = bit(sf0, 2)
            si['os'] = (sf0 >> 4) & 0x07
        if bit(s0, 2):  # HostName
            si['host_name'] = reader.read_string()
        if bit(s0, 3):  # ConnectPort
            si['connect_port'] = reader.read_uint16()

        if bit(s1, 0):  # Mod
            si['mod_name'] = reader.read_string()
            if si['mod_name']:
                mf = reader.read_var_bitarray()
                mf0 = vba_byte(mf, 0)
                if bit(mf0, 0):
                    si['mod_id'] = reader.read_string()
                if bit(mf0, 1):
                    si['mod_size'] = reader.read_uint32()
                if bit(mf0, 2):
                    si['mod_ver'] = reader.read_string()
                if bit(mf0, 3):
                    si['mod_url'] = reader.read_string()
                if bit(mf0, 4):
                    si['mod_author'] = reader.read_string()

        if bit(s1, 1):  # GameType
            si['game_type'] = reader.read_string()
        if bit(s1, 2):  # GameMode
            si['game_mode'] = reader.read_string()

        if bit(s1, 3):  # Map
            maf = reader.read_var_bitarray()
            maf0 = vba_byte(maf, 0)
            si['map_name'] = reader.read_string()
            if bit(maf0, 0):
                si['map_file'] = reader.read_string()
            if bit(maf0, 1):
                si['map_size'] = reader.read_uint32()
            if bit(maf0, 2):
                reader.read_bytes(16)  # MD5
            if bit(maf0, 3):
                si['map_ver'] = reader.read_string()
            if bit(maf0, 4):
                si['map_url'] = reader.read_string()
            if bit(maf0, 5):
                si['map_author'] = reader.read_string()

            if bit(s1, 4):  # NextMap
                si['next_map'] = reader.read_string()
                if bit(maf0, 0):
                    si['next_map_file'] = reader.read_string()
                if bit(maf0, 1):
                    reader.read_uint32()
                if bit(maf0, 2):
                    reader.read_bytes(16)
                if bit(maf0, 3):
                    reader.read_string()
                if bit(maf0, 4):
                    reader.read_string()
                if bit(maf0, 5):
                    reader.read_string()

        if bit(s2, 0):
            si['players'] = reader.read_var_uint()
        if bit(s2, 1):
            si['max_slots'] = reader.read_var_uint()
        if bit(s2, 2):
            si['bots'] = reader.read_var_uint()
        if bit(s2, 3):
            si['reserved'] = reader.read_var_uint()

        r['server'] = si

    if has_tl:
        teams = []
        count = reader.read_var_uint()
        tf0 = 0
        if count:
            tf = reader.read_var_bitarray()
            tf0 = vba_byte(tf, 0)
        for _ in range(count):
            t = {}
            if bit(tf0, 0):
                t['name'] = reader.read_string()
            if bit(tf0, 1):
                t['score'] = reader.read_var_sint()
            if bit(tf0, 2):
                t['avg_ping'] = reader.read_uint16()
            if bit(tf0, 3):
                t['avg_loss'] = reader.read_uint16()
            if bit(tf0, 4):
                t['players'] = reader.read_var_uint()
            if bit(tf0, 5):
                t['color'] = reader.read_uint16()
            teams.append(t)
        r['teams'] = teams

    if has_pl:
        players = []
        count = reader.read_var_uint()
        p0 = p1 = p2 = p3 = 0
        if count:
            pf = reader.read_var_bitarray()
            p0, p1, p2 = vba_byte(pf, 0), vba_byte(pf, 1), vba_byte(pf, 2)
            p3 = vba_byte(pf, 3)
        for _ in range(count):
            pl = {}
            if bit(p0, 0):  # Flags
                fl = reader.read_var_bitarray()
                fl0 = vba_byte(fl, 0)
                pl['alive'] = bit(fl0, 0)
                pl['dead'] = bit(fl0, 1)
                pl['bot'] = bit(fl0, 2)
            if bit(p0, 1):
                pl['slot'] = reader.read_var_uint()
            if bit(p0, 2):
                pl['name'] = reader.read_string()
            if bit(p0, 3):
                pl['team'] = reader.read_var_sint()
            if bit(p0, 4):
                pl['class'] = reader.read_string()
            if bit(p0, 5):
                pl['race'] = reader.read_string()
            if bit(p1, 0):
                pl['score'] = reader.read_var_sint()
            if bit(p1, 1):
                pl['frags'] = reader.read_var_sint()
            if bit(p1, 2):
                pl['kills'] = reader.read_var_uint()
            if bit(p1, 3):
                pl['deaths'] = reader.read_var_uint()
            if bit(p1, 4):
                pl['suicides'] = reader.read_var_uint()
            if bit(p1, 5):
                pl['teamkills'] = reader.read_var_uint()
            if bit(p2, 0):
                pl['id'] = reader.read_uint32()
            if bit(p2, 1):
                pl['global_id'] = reader.read_string()
            if bit(p2, 2):
                pl['ping'] = reader.read_uint16()
            if bit(p2, 3):
                pl['loss'] = reader.read_uint16()
            if bit(p2, 4):
                pl['model'] = reader.read_string()
            if bit(p2, 5):
                pl['time'] = reader.read_uint16()
            if bit(p3, 0):
                al = reader.read_var_uint()
                pl['addr'] = reader.read_bytes(al)
            players.append(pl)
        r['players'] = players

    if has_rl:
        rules = []
        count = reader.read_var_uint()
        for _ in range(count):
            key = reader.read_string()
            val = reader.read_string()
            rules.append((key, val))
        r['rules'] = rules

    if has_al:
        addons = []
        count = reader.read_var_uint()
        af0 = 0
        if count:
            af = reader.read_var_bitarray()
            af0 = vba_byte(af, 0)
        for _ in range(count):
            a = {}
            if bit(af0, 0):
                fl = reader.read_var_bitarray()
                fl0 = vba_byte(fl, 0)
                a['active'] = bit(fl0, 0)
            if bit(af0, 1):
                a['short'] = reader.read_string()
            if bit(af0, 2):
                a['long'] = reader.read_string()
            if bit(af0, 3):
                a['version'] = reader.read_string()
            addons.append(a)
        r['addons'] = addons

    return r


# ---------- display ----------

def hexdump(data, prefix=''):
    data = bytearray(data)
    for i in range(0, len(data), 16):
        h = ' '.join('%02X' % b for b in data[i:i + 16])
        a = ''.join(chr(b) if 32 <= b < 127 else '.' for b in data[i:i + 16])
        print('%s%04X: %-48s %s' % (prefix, i, h, a))


def show(result):
    si = result.get('server', {})
    if si:
        print('')
        print('  === Server Info ===')
        for label, key in [
            ('Game',      'game_name'),
            ('Server',    'host_name'),
            ('Port',      'connect_port'),
            ('Mod',       'mod_name'),
            ('Mod ID',    'mod_id'),
            ('Mod Ver',   'mod_ver'),
            ('Game Type', 'game_type'),
            ('Map',       'map_name'),
            ('Map File',  'map_file'),
            ('Players',   'players'),
            ('Max Slots', 'max_slots'),
            ('Bots',      'bots'),
            ('Reserved',  'reserved'),
        ]:
            if key in si:
                print('  %-12s %s' % (label + ':', si[key]))

        types = {0: 'Unknown', 1: 'Listen', 2: 'Dedicated'}
        oses = {0: 'Unknown', 1: 'Windows', 2: 'Linux', 3: 'Mac'}
        if 'server_type' in si:
            print('  %-12s %s' % ('Type:', types.get(si['server_type'], '?')))
        if 'password' in si:
            print('  %-12s %s' % ('Password:', 'Yes' if si['password'] else 'No'))
        if 'os' in si:
            print('  %-12s %s' % ('OS:', oses.get(si['os'], '?')))

    teams = result.get('teams', [])
    if teams:
        print('')
        print('  === Teams (%d) ===' % len(teams))
        for i, t in enumerate(teams):
            name = t.get('name', '') or '(none)'
            score = t.get('score', '?')
            pc = t.get('players', '?')
            print('  #%d  %-20s  score=%-6s  players=%s' % (i, name, score, pc))

    players = result.get('players', [])
    bots = si.get('bots', 0) if si else 0
    print('')
    print('  === Players (%d) | Bots: %d ===' % (len(players), bots))
    if players:
        print('  %-6s %-20s %-6s %-6s %-9s %-6s %-10s %s'
              % ('Slot', 'Name', 'Team', 'Score', 'K/D', 'Ping', 'ID', 'Status'))
        print('  ' + '-' * 75)
        for p in players:
            slot = str(p.get('slot', '?'))
            name = p.get('name', '?')
            team = str(p.get('team', '?'))
            score = str(p.get('score', '?'))
            kills = p.get('kills', '?')
            deaths = p.get('deaths', '?')
            kd = '%s/%s' % (kills, deaths)
            ping = str(p.get('ping', '?'))
            pid = str(p.get('id', '?'))
            alive = 'ALIVE' if p.get('alive') else ('DEAD' if p.get('dead') else '?')
            print('  %-6s %-20s %-6s %-6s %-9s %-6s %-10s %s'
                  % (slot, name, team, score, kd, ping, pid, alive))
    else:
        print('  (no players connected)')

    rules = result.get('rules', [])
    if rules:
        print('')
        print('  === Rules (%d) ===' % len(rules))
        for k, v in rules:
            print('  %s = %s' % (k, v))

    addons = result.get('addons', [])
    if addons:
        print('')
        print('  === Add-ons (%d) ===' % len(addons))
        for a in addons:
            parts = [a.get('short', ''), a.get('long', ''), a.get('version', '')]
            active = ' [active]' if a.get('active') else ''
            print('  %s (%s) v%s%s' % (parts[0], parts[1], parts[2], active))


# ---------- main ----------

def main():
    host = 'localhost'
    port = 7340
    timeout = 3.0

    if len(sys.argv) > 1:
        host = sys.argv[1]
    if len(sys.argv) > 2:
        port = int(sys.argv[2])

    print('=' * 60)
    print('OGP Test Client  ->  %s:%d' % (host, port))
    print('=' * 60)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(timeout)

    # --- 1. Ping ---
    print('\n[1] PING')
    pkt = build_ping()
    hexdump(pkt, '  > ')
    try:
        sock.sendto(pkt, (host, port))
        data, addr = sock.recvfrom(65536)
        print('  < %d bytes from %s:%d' % (len(data), addr[0], addr[1]))
        hexdump(data, '  < ')
        if data[:8] == OGP_MAGIC:
            hdr = parse_header(BufferReader(data[8:]))
            if hdr['type'] == TYPE_PING and hdr['bAnswer']:
                print('  >> PONG OK')
            else:
                print('  >> unexpected type=0x%02X' % hdr['type'])
        else:
            print('  >> not OGP')
    except socket.timeout:
        print('  >> TIMEOUT (server not responding)')
        sock.close()
        return
    except Exception as e:
        print('  >> ERROR: %s' % e)
        sock.close()
        return

    # --- 2. Default1 without challenge ---
    print('\n[2] QUERY (no challenge)')
    pkt = build_default1()
    hexdump(pkt, '  > ')
    challenge = None
    try:
        sock.sendto(pkt, (host, port))
        data, addr = sock.recvfrom(65536)
        print('  < %d bytes' % len(data))
        hexdump(data, '  < ')
        if data[:8] == OGP_MAGIC:
            reader = BufferReader(data[8:])
            hdr = parse_header(reader)
            print('  >> type=0x%02X answer=%s challenge=%s'
                  % (hdr['type'], hdr['bAnswer'], hdr.get('challenge', '-')))
            if hdr['type'] == TYPE_ERROR:
                err = reader.read_uint8()
                if err == ERR_INVALID_CHALLENGE:
                    challenge = hdr.get('challenge')
                    print('  >> got challenge: 0x%08X' % challenge)
                else:
                    print('  >> error code: %d' % err)
            elif hdr['type'] == TYPE_DEFAULT1 and hdr['bAnswer']:
                print('  >> server answered without challenge!')
                show(parse_default1_body(reader))
    except socket.timeout:
        print('  >> TIMEOUT')
        sock.close()
        return
    except Exception as e:
        print('  >> ERROR: %s' % e)
        traceback.print_exc()
        sock.close()
        return

    if challenge is None:
        print('\n[!] No challenge obtained, stopping.')
        sock.close()
        return

    # --- 3. Default1 with challenge ---
    print('\n[3] QUERY (with challenge 0x%08X)' % challenge)
    pkt = build_default1(challenge)
    hexdump(pkt, '  > ')
    try:
        sock.sendto(pkt, (host, port))
        data, addr = sock.recvfrom(65536)
        print('  < %d bytes' % len(data))
        hexdump(data, '  < ')
        if data[:8] == OGP_MAGIC:
            reader = BufferReader(data[8:])
            hdr = parse_header(reader)
            print('  >> type=0x%02X answer=%s' % (hdr['type'], hdr['bAnswer']))
            if hdr['type'] == TYPE_DEFAULT1 and hdr['bAnswer']:
                print('  >> SUCCESS - parsing server data...')
                result = parse_default1_body(reader)
                show(result)
            elif hdr['type'] == TYPE_ERROR:
                err = reader.read_uint8()
                print('  >> ERROR code=%d' % err)
            else:
                print('  >> unexpected type=0x%02X' % hdr['type'])
    except socket.timeout:
        print('  >> TIMEOUT')
    except Exception as e:
        print('  >> ERROR: %s' % e)
        traceback.print_exc()

    sock.close()
    print('\nDone.')


if __name__ == '__main__':
    main()
