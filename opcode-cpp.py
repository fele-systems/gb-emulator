#!/usr/bin/python3
import xml.etree.ElementTree as ET
import argparse
import string
import copy
import logging
from collections.abc import Sequence

kinds = {
    "8BitRegister": "r8",
    "16BitRegister": "r16",
    "8BitImmediate": "8imm",
    "16BitImmediate": "16imm",
    "8BitRegisterPointer": "pr8",
    "16BitRegisterPointer": "pr16",
    "16BitImmediatePointer": "p16imm",
    "8BitImmediatePointer": "p8imm"
}


def kind_isimmediate(kind):
    return kind == "8BitImmediate" or \
        kind == "8BitImmediatePointer" or \
        kind == "16BitImmediate" or \
        kind == "16BitImmediatePointer"


def kind_ispointer(kind: str):
    return kind.endswith("Pointer")


def kind_isregister(kind):
    return kind == "8BitRegister" or kind == "8BitRegisterPointer" or \
        kind == "16BitRegister" or kind == "16BitRegisterPointer"


class NonExaustiveFormatter(string.Formatter):
    def __init__(self, default='{{{0}}}'):
        self.default = default

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
        opcodes = []
        logging.debug('Current parsing sub-category %s', subcat.get('name'))
        for opcode in subcat.findall('opcode'):
            opcodes = opcodes + Opcode.parse(opcode)
        return SubCategory(subcat.get('name'), opcodes)

    def __init__(self, name, opcodes):
        self.name = name
        self.opcodes = opcodes


class Argument:
    name: str
    kind: str
    default: str
    resolvedname: str

    def __init__(self, name, kind, defaults):
        self.name = name
        self.kind = kind
        self.defaults = defaults
        self.resolvedname = None

    def resolvedefault(self):
        """
        If argument is a register, resolvedname will be based on
        defaults attribute.
        If argument is a immediate, resolvedname will be the same as
        name attribute.
        """
        if kind_isimmediate(self.kind):
            self.resolvedname = kinds[self.kind]
        elif self.resolvedname is None:
            self.resolvedname = self.defaults

    def getcppparam(self):
        """
        Returns the cpp parameter notation. Used in function names
        and parameters
        """
        if kind_ispointer(self.kind):
            return kinds[self.kind]
        elif self.kind == 'JumpCondition':
            return self.resolvedname
        else:
            return kinds[self.kind]

    def pointerprefixed(self):
        """
        Returns the resolved name plus the 'p' prefix if pointer
        """
        if kind_ispointer(self.kind):
            return "p" + self.resolvedname or kinds[self.kind]
        else:
            return self.resolvedname or kinds[self.kind]

    def getcppsymbol(self):
        if self.kind == "8BitRegister":
            return f"R8::{self.resolvedname}"
        elif self.kind == "16BitRegister":
            return f"R16::{self.resolvedname}"
        elif self.kind == "8BitImmediate":
            return "next_pc_byte()"
        elif self.kind == "16BitImmediate":
            return "next_pc_word()"
        elif self.kind == "8BitRegisterPointer":
            return f"R8::{self.resolvedname}"
        elif self.kind == "16BitRegisterPointer":
            return f"R16::{self.resolvedname}"
        elif self.kind == "16BitImmediatePointer":
            return "next_pc_word()"
        else:
            raise Exception(f"Unkown kind: {self.kind}")

    def toStr(self):
        return "name: {}; kind: {}; value: {};".format(self.name,
                                                       self.kind,
                                                       self.resolvedname)


class Arguments:
    @staticmethod
    def parse(args):
        arglist = []
        if args is None:
            return Arguments([])
        for arg in args.findall('arg'):
            argument = Argument(arg.get('name'),
                                arg.get('kind'),
                                arg.get('defaults'))
            arglist.append(argument)
        return Arguments(arglist)

    args: Sequence[Argument]

    def __init__(self, args):
        self.args = args

    def resolvearg(self, name: str, newname: str):
        """
        If @name is a existing argument object, replace its value attribute
        with @value. Otherwise, do nothing.
        """
        for arg in self.args:
            if arg.name == name:
                arg.resolvedname = newname
                break

    def rename(self, oldname, newname):
        for arg in self.args:
            if arg.name == oldname:
                arg.name = newname
                break

    def resolvedefaults(self):
        """
        Tries to resolve all of the arguments with defaults.
        A argument is considered resolved when its value attribute is not
        None.
        This should only be called after the repeats-on clauses have
        been resolved, because it will throw a Exception if any argument
        is not resolved.
        """
        for i, arg in enumerate(self.args, start=0):
            arg.resolvedefault()
            if arg.resolvedname is None:
                raise Exception(f"Arg {i}:{arg.kind} could not be resolved.")

    def toStr(self):
        return list(map(lambda arg: arg.toStr(), self.args))


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
            "template":     bool(opcodexml.get('template')),
            "args":         Arguments.parse(opcodexml.find('args')),
            "extended":     bool(opcodexml.get('extended'))
        }

        logging.debug('Currently parsing %s"%s"',
                      '*' if opcodemap['template'] else '-',
                      opcodemap['format'])

        if opcodemap['template']:

            onclauses = []
            repeatclauses = dict()
            for repeat in opcodexml.find('repeats').iter('repeat'):
                repeatclauses[repeat.get('argument')] = {
                    'kind': repeat.get('kind'),
                    'argument': repeat.get('argument')
                }
                onclauses += [on.attrib for on in repeat.findall('on')]

            repeatclauses = list(repeatclauses.values())
            for clause in repeatclauses:
                logging.debug('Repeat found: %s', clause)

            logging.debug('Found %s on-clauses', len(onclauses))

            for on in onclauses:
                tempmap = copy.deepcopy(opcodemap)
                for repeat in repeatclauses:
                    argreplace = repeat['argument']
                    argnewname = on[argreplace]
                    tempmap["args"].resolvearg(argreplace, argnewname)
                tempmap |= on
                opobject = Opcode.frommap(tempmap)
                opobject.cannyformat()
                try:
                    opobject.args.resolvedefaults()
                except Exception:
                    raise Exception("While parsing {}\nOn {}".format(
                        opobject.toStr(),
                        on))
                opcodes.append(opobject)
        else:
            opobject = Opcode.frommap(opcodemap)
            try:
                opobject.args.resolvedefaults()
            except Exception:
                raise Exception("While parsing {}".format(
                    opobject.toStr()))
            logging.debug('Generated Opcode %s', opobject.toStr())
            opcodes.append(opobject)
        return opcodes

    @staticmethod
    def frommap(opcodemap):
        return Opcode(opcodemap["instruction"],
                      opcodemap["value"],
                      opcodemap["cycles"],
                      opcodemap["format"],
                      opcodemap["nargs"],
                      opcodemap["size"],
                      opcodemap["args"],
                      opcodemap["extended"])

    instruction: str
    value: str
    cycles: int
    fmt: str
    nargs: int
    size: int
    args: Arguments
    extended: bool

    def __init__(self,
                 instruction: str,  # The instruction radical
                 value: str,        # The opcode byte value
                 cycles: int,       # Cycles of the opcode
                 fmt: str,          # Format of the instruction
                 nargs: int,        # Number of arguments
                 size: int,         # Size in bytes (incl immediate)
                 args: Arguments,   # Argument list
                 extended: bool):   # True when multi-byte opcode
        self.instruction = instruction
        self.value = value
        self.cycles = cycles
        self.fmt = fmt
        self.nargs = nargs
        self.size = size
        self.args = args
        self.extended = extended

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
        args = dict(map(lambda arg: (arg.name, arg.resolvedname or '{}'),
                        self.args.args))
        self.fmt = format.format(self.fmt, **args)

    def getenum(self):
        parts = [self.instruction] + \
                [arg.pointerprefixed() for arg in self.args.args]

        return '_'.join(parts)

    def getfunctionname(self):
        if len(self.args.args) == 0:
            return self.instruction

        return f"{self.instruction}_" + \
               "_".join([arg.getcppparam() for arg in self.args.args])

    def getfunctioncall(self):
        name = self.getfunctionname()
        args = [arg.getcppsymbol() for arg in self.args.args
                if not kind_isimmediate(arg.kind) and not arg.kind == 'JumpCondition']

        args = ", ".join(args)
        return f"{name} ({args})"

    def valueas8b(self):
        if self.extended == "1":
            return self.value.split(' ')[1]
        else:
            return self.value

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


def build_enum(fd, categories: Sequence[Category]):
    fd.write("#pragma once\n")
    fd.write("#include <cstdint>\n")
    fd.write("enum class Opcode : uint16_t\n")
    fd.write("{\n")
    for category in categories:
        fd.write("\n")
        fd.write("    //{:/<50}\n".format(''))
        fd.write("    //{}\n".format(category.name))
        fd.write("    //{:/<50}\n".format(''))
        fd.write("\n")
        for subcategory in category.subcat:
            fd.write("\n")
            fd.write("    //{:-<50}\n".format(''))
            fd.write("    //{}\n".format(subcategory.name))
            fd.write("    //{:-<50}\n".format(''))
            fd.write("\n")
            for opcode in subcategory.opcodes:
                if not opcode.extended:
                    fd.write("    {} = {},\n".format(opcode.getenum(),
                                                     opcode.valueas8b()))
                else:
                    fd.write("    {} = {},\n".format(opcode.getenum(),
                                                     opcode.valueas8b()))
    fd.write("};\n")


def build_switch(fd, categories: Sequence[Category]):
    fd.write("#include <gbcpu.h>\n"
             "#include <opcodes-autogen.h>\n"
             "InstructionTrait GbCpu::execute_next()\n"
             "{\n"
             "    auto instruction = static_cast<Opcode> ( next_pc_byte() );\n"
             "    switch ( instruction ) {\n")
    for category in categories:
        fd.write("\n")
        fd.write("    //{:/<50}\n".format(''))
        fd.write("    //{}\n".format(category.name))
        fd.write("    //{:/<50}\n".format(''))
        fd.write("\n")
        for subcategory in category.subcat:
            fd.write("\n")
            fd.write("    //{:-<50}\n".format(''))
            fd.write("    //{}\n".format(subcategory.name))
            fd.write("    //{:-<50}\n".format(''))
            fd.write("\n")
            for opcode in subcategory.opcodes:
                if not opcode.extended:
                    fd.write("\tcase Opcode::{}:\n".format(opcode.getenum()))
                    fd.write("\t\treturn {};\n".format(opcode.getfunctioncall()))
                    fd.write("\n")
    fd.write("    }"  # End switch
             "}")  # End function


def main():
    parser = argparse.ArgumentParser(
            description='Opcode code generator for gb-emulator')
    parser.add_argument('-d', '--dryrun',
                        type=str,
                        default=False)
    parser.add_argument('-e', '--enumout',
                        type=str,
                        default='include/opcodes-autogen.h')
    parser.add_argument('-s', '--switchout',
                        type=str,
                        default='src/call_switch-autogen.cpp')

    args = parser.parse_args()
    tree = ET.parse("opcodes.xml")
    root = tree.getroot()
    FORMAT = "[%(filename)s:%(lineno)3s - %(funcName)10s()] "\
             "[%(levelname)s] %(message)s"
    logging.basicConfig(format=FORMAT, level=logging.WARN)

    categories = []
    for category in [Category.parse(cat) for cat in root]:
        categories.append(category)
        

    if args.dryrun:
        for category in categories:
            for sc in category.subcat:
                for i in sc.opcodes:
                    print("{} = {} // {}".format(i.getenum(),
                                             i.value,
                                             i.getfunctioncall()))
        return


    with open(args.enumout, "w") as fd:
        build_enum(fd, categories)

    with open(args.switchout, 'w') as fd:
        build_switch(fd, categories)


if __name__ == "__main__":
    main()
