'''
    Created by Munawar Hasan
'''


def create_header_files(fname1, fname2):
    msg_length = []
    header_file_builder = "{\n"
    with open(fname1, 'r') as f:
        tvs = f.readlines()[7:-1]
        tvs = [i for i in tvs if i != '\n']
        tvs = [i.replace('\n','') for i in tvs]
        
        Msgs = [x.split()[-1] for i, x in enumerate(tvs) if "Msg" in x]
        MDs = [x.split()[-1] for i, x in enumerate(tvs) if "MD" in x]

        assert len(Msgs) == len(MDs), "mismatch ..."

        for index in range(len(Msgs)):
            tv = '\t{\n'
            msg = Msgs[index]
            msg_b = "{" +str(int(len(msg) / 2)) +"}"
            if index == 0:
                msg_b = "{" +str(0) +"}"
            msg = ','.join(msg[i:i+2] for i in range(0, len(msg), 2))
            msg = msg.split(',')
            msg = ['0x'+i for i in msg]
            
            msg_length.append(len(msg))

            msg = '{' +','.join(msg) +'}'

            md = MDs[index]
            md = ','.join(md[i:i+2] for i in range(0, len(md), 2))
            md = md.split(',')
            md = ['0x'+i for i in md]
            md = '{' +','.join(md) +'}'

            tv = tv +'\t\t' +msg_b +',\n' +'\t\t' +msg +',\n' +'\t\t' +md
            tv = tv +'\n\t}'
            if index == (len(Msgs) - 1):
                pass
            else:
                tv = tv +',\n'

            header_file_builder = header_file_builder + tv
        
        header_file_builder = header_file_builder +"\n};"

    
        header_file_builder = "uint8_t TESTVECTOR[][3][" +str(max(msg_length)) +"] = \n" +header_file_builder

        headers = "/*\n\tAuto Generated Header File\n*/\n\n"
        headers = headers + "#include <stdint.h>\n"
        headers = headers + "const int tv_length = " +str(len(Msgs)) +";\n\n" 
        header_file_builder = headers + header_file_builder

    with open(fname2, 'w') as f:
        f.write(header_file_builder)


if __name__ == '__main__':
    tv_files = [
        ['SHA256ShortMsg.rsp', 'SHA256ShortMsg.h']
    ]
    for tv_file in tv_files:
        create_header_files(tv_file[0], tv_file[1])