 '''midi decompiler 2.0.0
    made by SnowyFirefly-Kami
    Python 3.x
    document:https://github.com/KamiHane/-/blob/cbs_document/cbs_midi_en.md
'''
 # document:
def bin_format(byte_bin):
    # input '0b0000'
    if byte_bin[0:2] == '0b':
        byte_bin = byte_bin[2:]
    return (8-len(byte_bin)) * '0' + byte_bin
def hex_to_bin(hex_str):
    hex_bit = {
    '0':'0000',
    '1':'0001',
    '2':'0010',
    '3':'0011',
    '4':'0100',
    '5':'0101',
    '6':'0110',
    '7':'0111',
    '8':'1000',
    '9':'1001',
    'a':'1010',
    'b':'1011',
    'c':'1100',
    'd':'1101',
    'e':'1110',
    'f':'1111'
    }
    if hex_str[0:2] == '0x':
        hex_str = hex_str[2:]
    bin_str = ''
    for bit in hex_str:
        bin_str += hex_bit[bit]
    return bin_str
def bytes_to_bin(bytes_str):
    bin_strs = ''
    for byte in bytes_str:
        bin_strs += bin_format(bin(byte))
    return bin_strs
def bytes_to_dec(bytes_str):
    return int(bytes_to_bin(bytes_str),2)
def action_byte(bytes_str):
    bin_str = ''
    for dec in bytes_str:
        bin_str += bin_format(bin(dec))[1:]
    return int(bin_str,2)
def bytes_to_hex(bytes_str):
    i = []
    for e in bytes_str:
        i.append(hex(e)[2:])
    return i
class File:
    def __init__(self,path):
        self.file = open(path,'rb')
        self.test = self.file.read(4) == b'MThd'
        if not self.test:
            print("This is not midi file!")
    def load(self):
        # default data
        self.beats_per_minute = 100
        self.microsecond_per_beat = 600000
        head_data_length = bytes_to_dec(self.file.read(4))
        head_data_block = self.file.read(head_data_length)
        # head data block
        self.type = bytes_to_dec(head_data_block[0:2])
        self.track_number = bytes_to_dec(head_data_block[2:4])
        self.ticks_per_beat = bytes_to_dec(head_data_block[4:6])
        # track block
        self.tracks = []
        while self.file.read(4) == b'MTrk':
            self.file.read(4)
            self.tracks.append([])
            while True:
                delta_bytes = self.file.read(1)
                while delta_bytes[-1] >= 128:
                    delta_bytes += self.file.read(1)
                msg_block = delta_bytes
                msg_block += self.file.read(1)
                if msg_block[-1] == 255:
                    # is meta msg
                    msg_block += self.file.read(2) # meta message type || meta message length
                    type_byte = msg_block[-2]
                    msg_block += self.file.read(msg_block[-1]) # meta message body
                    self.tracks[-1].append(Message(msg_block))
                    if type_byte == 47:
                        break
                else:
                    # is normal mag
                    if msg_block[-1]//16 in [8,9,10,11,14]:
                        msg_block += self.file.read(2)
                    else:
                        msg_block += self.file.read(1)
                    self.tracks[-1].append(Message(msg_block))
                
class Message():
    def __init__(self,bytes_str):
        point = 0
        delta_byte = bytes_str[0:1]; point += 1
        while delta_byte[-1] >= 128:
            delta_byte += bytes_str[point:point+1]; point += 1
        self.delta_tick = action_byte(delta_byte)
        ismeta_int = bytes_str[point]; point += 1
        if ismeta_int == 255:
            self.is_meta = True
            meta_type_int = bytes_str[point]; point += 2
            try:
                self.type = meta_type_dict[meta_type_int]
            except KeyError:
                self.type = 'unknown_meta_message'
            if meta_type_int == 0:
                pass
            elif meta_type_int == 1:
                self.text = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 2:
                self.copyright = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 3:
                self.track_name = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 4:
                self.instrument_name = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 5:
                self.lyric = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 6:
                self.marker = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 7:
                self.cue_marker = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 9:
                self.device_name = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 32:
                self.channel_prefix = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 33:
                self.midi_port = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 47:
                print('a')
            elif meta_type_int == 81:
                self.set_tempo = int.from_bytes(bytes_str[point:],'big')
            elif meta_type_int == 84:
                self.smpte_offset = bytes_str[point:]
            elif meta_type_int == 88:
                self.time_signature = (bytes_str[point],bytes_str[point+1])
            elif meta_type_int == 89:
                self.key_signature = str(bytes_str[point:],encoding='utf-8')
            elif meta_type_int == 127:
                pass
        else:
            self.is_meta = False
            type_int = ismeta_int // 16
            try:
                self.type = type_dict[type_int]
            except KeyError:
                self.type = 'unknown_message'
            self.channel = ismeta_int % 16
            if type_int == 8:
                self.note = bytes_str[point]
                self.velocity = bytes_str[point+1]
            elif type_int == 9:
                self.note = bytes_str[point]
                self.velocity = bytes_str[point+1]
            elif type_int == 10:
                self.note = bytes_str[point]
                self.velocity = bytes_str[point+1]
            elif type_int == 11:
                self.controller = bytes_str[1]
                self.value = bytes_str[2]
            elif type_int == 12:
                self.program = bytes_str[1]
            elif type_int == 13:
                self.pressure = bytes_str[1]
            elif type_int == 14:
                self.low = bytes_str[1]
                self.high = bytes_str[2]
    def __str__(self):
        return '<%s %s %s>'%(self.delta_tick,self.is_meta,self.type)

type_dict = {
                    8: 'note_off',  #音符结束
                    9: 'note_on',   #音符开始
                    10: 'aftertouch',   #触后
                    11: 'controller',    # 左右声道控制器
                    12: 'program', #乐器改变
                    13: 'pressure', # 通道压力
                    14: 'pitch_wheel',   # 滑音轮:滑呀滑,这玩意特tm傻逼,基本无法在mc还原
                    15: 'system_command'    #系统命令
                }
                # 元类型算整数
meta_type_dict = {
                        0: 'sequence_number', # 序号
                        1: 'text',            # 文本消息
                        2: 'copyright',       # 版权声明
                        3: 'track_name',      # 通道名称
                        4: 'instrument_name', # 乐器名称
                        5: 'lyric',           # 歌词
                        6: 'marker',          # 记号
                        7: 'cue_marker',      # 提示记号
                        9: 'device_name',     # 设备名
                        32: 'channel_prefix', # 通道代号
                        33: 'midi_port',      # midi端口
                        47: 'track_end',      # 通道结束
                        81: 'set_tempo',      # 微秒/节拍
                        84: 'smpte_offset',   # 我也不知道有啥用,应该和时间控制有关,但是做红石音乐不会用
                        88: 'time_signature',  # 拍式, 如4/4
                        89: 'key_signature',   # 调式, 如C大调
                        127: 'sequencer_specific' # 我也不知道是啥了
                     }
