import pwn

def int_to_bytes(i):
    return int.to_bytes(i,4,byteorder="little", signed=True)

assembly = open("challenge.vasm", "r")
bincode = open("challenge.vm", "wb")

for instr in assembly:
    if instr.strip().startswith("#"):
        continue

    params = instr.split()
    code = b""
    
    if len(params) == 0:
        continue

    match params[0]:
        case "add":
            code += int_to_bytes(1)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
            code += int_to_bytes(int(params[3]))
        case "sub":
            code += int_to_bytes(2)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
            code += int_to_bytes(int(params[3]))
        case "mul":
            code += int_to_bytes(3)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
            code += int_to_bytes(int(params[3]))
        case "div":
            code += int_to_bytes(4)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
            code += int_to_bytes(int(params[3]))
        case "rstin":
            code += int_to_bytes(5)
        case "rstout":
            code += int_to_bytes(6)
        case "popin":
            code += int_to_bytes(7)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
        case "popout":
            code += int_to_bytes(8)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
        case "read":
            code += int_to_bytes(9)
            code += int_to_bytes(int(params[1]))
        case "write":
            code += int_to_bytes(10)
            code += int_to_bytes(int(params[1]))
        case "jmp":
            code += int_to_bytes(11)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
        case "njmp":
            code += int_to_bytes(12)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
        case "len":
            code += int_to_bytes(13)
            code += int_to_bytes(int(params[1]))
            code += int_to_bytes(int(params[2]))
        case _:
            code += int_to_bytes(0)
    
    bincode.write(code)


assembly.close()
bincode.close()