import xml.etree.ElementTree as ET
import string
import sys

class Category:
    def __init__(self, name, opcodes):
        self.name = name
        self.opcodes = opcodes
class Opcode:
    def __init__(self, instruction, value, cycles, fmt):
        self.instruction = instruction
        self.value = value
        self.cycles = cycles
        self.fmt = fmt

    def assert_valid(self):
        if self.instruction is None:
            raise Exception("instruction is None")
        elif self.value is None:
            raise Exception("value is None")
        elif self.cycles is None:
            raise Exception("cycles is None")
        elif self.fmt is None:
            raise Exception("format is None")

class LazyFormatter(string.Formatter):
    def __init__(self, default='{{{0}}}'):
        self.default=default

    def get_value(self, key, args, kwds):
        if isinstance(key, str):
            return kwds.get(key, self.default.format(key))
        else:
            return string.Formatter.get_value(key, args, kwds)

def textornull(node):
    if node is None:
        return None
    else:
        return node.text

def parse(root):
    list = []
    formatter = LazyFormatter()
    for category in root.findall("category"):
        c = Category(category.get("name"), [])
        for opcode in category.findall("opcode"):
            opcodeobj = Opcode(opcode.get('instruction'),
                               textornull(opcode.find('value')),
                               textornull(opcode.find('cycles')),
                               textornull(opcode.find('format'))
                               )

            repeats = opcode.find("repeats")
            if repeats is not None:
                register8bit = repeats.find("register8bit")
                if register8bit is not None:
                    for register in register8bit.text.split(","):
                        copy = Opcode(opcodeobj.instruction,
                                  opcodeobj.value,
                                  opcodeobj.cycles,
                                  opcodeobj.fmt)
                        (regname, newvalue) = register.strip().split("=")
                        copy.instruction = formatter.format(copy.instruction, r8 = regname)
                        copy.fmt = formatter.format(copy.fmt, r8 = regname)
                        copy.value = newvalue
                        copy.assert_valid()
                        c.opcodes.append(copy)
                register16bit = repeats.find("register16bit")
                if register16bit is not None:
                    for register in register16bit.text.split(","):
                        copy = Opcode(opcodeobj.instruction,
                                  opcodeobj.value,
                                  opcodeobj.cycles,
                                  opcodeobj.fmt)
                        (regname, newvalue) = register.strip().split("=")
                        copy.instruction = formatter.format(copy.instruction, r16 = regname)
                        copy.fmt = formatter.format(copy.fmt, r16 = regname)
                        copy.value = newvalue
                        copy.assert_valid()
                        c.opcodes.append(copy)
            else:
                opcodeobj.assert_valid()
                c.opcodes.append(opcodeobj)
        list.append(c)
    return list

def build_enum(fd, categories):
    fd.write("#pragma once\n")
    fd.write("#include <cstdint>\n")
    fd.write("enum class Opcode : uint8_t\n")
    fd.write("{\n")


    for category in categories:
        fd.write("    //{:/<50}\n".format(''))
        fd.write("    //{}\n".format(category.name))
        fd.write("    //{:/<50}\n".format(''))
        fd.write("\n")
        for opcode in category.opcodes:
            fd.write("    {} = {}, // {} cycles \n".format(opcode.instruction, opcode.value, opcode.cycles))
    fd.write("};\n")

def build_switch(fd, categories):
    fd.write("switch(opcode)\n")
    fd.write("{\n")

    for category in categories:
        fd.write("    //{:/<50}\n".format(''))
        fd.write("    //{}\n".format(category.name))
        fd.write("    //{:/<50}\n".format(''))
        fd.write("\n")
        for opcode in category.opcodes:
            fd.write("    case Opcode::{}:\n".format(opcode.instruction))
            parts = opcode.instruction.split('_')

            if len(parts) == 1:
                fd.write("        return {}();\n".format(parts[0]))
            elif len(parts) == 2:
                g0 = genericit(parts[1])
                fd.write("        return {}_{}(".format(parts[0],
                                                           g0[0]))
                if g0[1] is not None:
                    fd.write("{}", g0[1])
                fd.write(");\n")
            else:
                g0 = genericit(parts[1])
                g1 = genericit(parts[2])
                fd.write("        return {}_{}_{}(".format(parts[0],
                                                           g0[0],
                                                           g1[0]))
                if g0[1] is not None and g1[1] is not None:
                    fd.write("{}, {}".format(g0[1], g1[1]))
                elif g0[1] is not None and g1[1] is None:
                    fd.write("{}".format(g0[1]))
                elif g0[1] is None and g1[1] is not None:
                    fd.write("{}".format(g1[1]))

                fd.write(");\n")


    fd.write("};\n")

def genericit(arg_spec):
    if arg_spec.startswith('p'):
        arg_spec = arg_spec[1:]
        ispointer = True
    else:
        ispointer = False
    register = None
    if arg_spec in [ 'A', 'B', 'C', 'D', 'E', 'F', 'H', 'H' ]:
        register = 'R8::' + arg_spec
        arg_spec = 'r8'
    elif arg_spec in [ 'AF', 'BC', 'DE', 'HL', 'SP', 'PC' ]:
        register = 'R16::' + arg_spec
        arg_spec = 'r16'

    if ispointer:
        return ("p" + arg_spec, register)
    else:
        return (arg_spec, register)

def tocppfunction(instruction):
    parts = instruction.split('_')
    if (len(parts) == 1):
        return parts[0]
    else:
        return "{}_{}_{}".format(parts[0],
                                 genericit(parts[1]),
                                 genericit(parts[2]))

def main():
    tree = ET.parse("opcodes.xml")
    root = tree.getroot()
    categories = parse(root)

    #fd = open("opcodes.h", "w")
    #build_enum(fd, categories)
    #fd.close()

    build_switch(sys.stdout, categories)



if __name__ == "__main__":
    main()
