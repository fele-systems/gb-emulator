#!/usr/bin/python
import xml.etree.ElementTree as ET
import string
import sys
import copy

kinds = {
    "8BitRegister": "r8",
    "8BitImmediate": "8imm",
    "16BitImmediate": "16imm",
    "16BitRegisterPointer": "pr16"
}

def kind_isimmediate(kind):
    return kind == "8BitImmediate" or kind == "16BitImmediate"

def kind_isregister(kind):
    return kind == "8BitRegister" or kind == "16BitRegisterPointer"

class NonExaustiveFormatter(string.Formatter):
    def __init__(self, default='{{{0}}}'):
        self.default=default

    def get_value(self, key, args, kwds):
        if isinstance(key, str):
            return kwds.get(key, self.default.format(key))
        else:
            return string.Formatter.get_value(key, args, kwds)

class Category:
    @staticmethod
    def parse(category):
        subcats = []
        for subcategory in category.findall('sub-category'):
            subcats.append(SubCategory.parse(subcategory))
        return Category(category.get('name'), subcats)

    def __init__(self, name, subcat):
        self.name = name
        self.subcat = subcat

class SubCategory:
    @staticmethod
    def parse(subcat):
        opcodes=[]
        for opcode in subcat.findall('opcode'):
            opcodes = opcodes + Opcode.parse(opcode)
        return SubCategory(subcat.get('name'), opcodes)

    def __init__(self, name, opcodes):
        self.name = name
        self.opcodes = opcodes

class Arguments:
    @staticmethod
    def parse(args):
        a = []
        for arg in args.findall('arg'):
            a.append(Argument(arg.get('name'), arg.get('kind'), arg.get('default')))
        return Arguments(a)

    def __init__(self, args):
        self.args = args

    def resolvearg(self, name, value):
        for arg in self.args:
            if arg.name == name:
                arg.value = value
                break

    def rename(self, oldname, newname):
        for arg in self.args:
            if arg.name == oldname:
                arg.name = newname
                break

    def resolveargs(self):
        for i, arg in enumerate(self.args, start=0):
            arg.resolvedefault()
            if arg.name is None:
                raise Exception("Argument {} could not be resolved".format(i))

    def toStr(self):
        return list(map(lambda arg: arg.toStr(), self.args))

class Argument:
    def __init__(self, name, kind, default):
        self.name = name
        self.kind = kind
        self.default = default
        self.value = None

    def resolvedefault(self):
        if self.value is None:
            self.value = self.default

    def toStr(self):
        return "name: {}; kind: {}; value: {};".format(self.name, self.kind, self.value)

class Opcode:
    @staticmethod
    def parse(opcodexml):
        opcodes = []
        opcodemap = {
            "instruction":  opcodexml.get('instruction'),
            "value":        opcodexml.get('value'),
            "cycles":       int(opcodexml.get('cycles')),
            "format":       opcodexml.get('format'),
            "nargs":        int(opcodexml.get('nargs')),
            "size":         int(opcodexml.get('size')),
            "args":         Arguments.parse(opcodexml.find('args')) if opcodexml.find('args') is not None else Arguments([])
        }

        if opcodexml.get('template', default=False):
            for repeat in opcodexml.find('repeats').findall('repeat'):
                argtoreplace = repeat.get('argument')
                if argtoreplace is None:
                    raise Exception("argument is not defined: {}".format(ET.tostring(repeat)))

                for on in repeat.findall('on'):
                    tempmap = copy.copy(opcodemap)
                    if on.get(argtoreplace) is None:
                        raise Exception("argument '{}' does not exist in on-clause: {}".format(argtoreplace, ET.tostring(on)))
                    tempmap["args"].resolvearg(argtoreplace, on.get(argtoreplace))
                    for attr, val in on.attrib.items():
                        tempmap[attr] = val
                    opobject = Opcode(tempmap["instruction"], 
                                      tempmap["value"], 
                                      tempmap["cycles"], 
                                      tempmap["format"], 
                                      tempmap["nargs"],
                                      tempmap["size"],
                                      tempmap["args"])
                    opobject.cannyformat()
                    try:
                        opobject.args.resolveargs()
                    except Exception as e:
                        raise Exception("While parsing {}: ".format(opobject.toStr()) + e.args[0])
                    opcodes.append(opobject)
        
        return opcodes


    def __init__(self, instruction, value, cycles, fmt, nargs, size, args):
        self.instruction = instruction
        self.value = value
        self.cycles = cycles
        self.fmt = fmt
        self.nargs = nargs
        self.size = size
        self.args = args

    def assert_valid(self):
        if self.instruction is None:
            raise Exception("instruction is None")
        elif self.value is None:
            raise Exception("value is None")
        elif self.cycles is None:
            raise Exception("cycles is None")
        elif self.fmt is None:
            raise Exception("format is None")

    def cannyformat(self):
        format = NonExaustiveFormatter()
        args = dict(map(lambda arg: (arg.name, arg.value), self.args.args))
        print(args)
        self.fmt = format.format(self.fmt, **args)

    def getenum(self):
        parts = [ self.instruction ] + list(map(lambda arg: kinds[arg.kind], self.args.args))
        return '_'.join(parts)

    def toStr(self):
        return "instruction: {}; value: {}; cycles: {}; format: {}, nargs: {}, size: {}; args: {};".format(self.instruction, self.value, self.cycles, self.fmt, self.nargs, self.size, self.args.toStr())



def attrordefault(node, attr, default):
    val = node.get(attr)
    return (val if val is not None else default)

def textornull(node):
    if node is None:
        return None
    else:
        return node.text

def parse(root):
    list = []
    formatter = NonExaustiveFormatter()
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

def parse_category(category):
    print(category)

def parse_subcategories(subcategory):
    print(subcategory)

def main():
    tree = ET.parse("opcodes.xml")
    root = tree.getroot()

    for e in root:
        print(e)

    for category in root:
        for sc in Category.parse(category).subcat:
            for i in sc.opcodes:
                print(i.getenum())

    #fd = open("opcodes.h", "w")
    #build_enum(fd, categories)
    #fd.close()

    #build_switch(sys.stdout, categories)



if __name__ == "__main__":
    main()
