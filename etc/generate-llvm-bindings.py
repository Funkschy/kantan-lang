from pprint import pprint
import requests
import re

links = [
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/core.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/linker.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/analysis.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/target.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/target_machine.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/bit_reader.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/bit_writer.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/transforms/pass_manager.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/transforms/pass_manager_builder.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/transforms/scalar.rs',
    'https://gitlab.com/taricorp/llvm-sys.rs/-/raw/master/src/transforms/util.rs',
]


# extracted from kantan-rust compiler
whitelist = set([
    'LLVMAddFunction',
    'LLVMAddGlobal',
    'LLVMAppendBasicBlockInContext',
    'LLVMArrayType',
    'LLVMBuildAdd',
    'LLVMBuildAlloca',
    'LLVMBuildAnd',
    'LLVMBuildBitCast',
    'LLVMBuildBr',
    'LLVMBuildCall',
    'LLVMBuildCondBr',
    'LLVMBuildFAdd',
    'LLVMBuildFCmp',
    'LLVMBuildFDiv',
    'LLVMBuildFMul',
    'LLVMBuildFPExt',
    'LLVMBuildFRem',
    'LLVMBuildFSub',
    'LLVMBuildFree',
    'LLVMBuildGEP',
    'LLVMBuildICmp',
    'LLVMBuildInBoundsGEP',
    'LLVMBuildIntCast',
    'LLVMBuildLoad',
    'LLVMBuildMalloc',
    'LLVMBuildMul',
    'LLVMBuildNeg',
    'LLVMBuildNot',
    'LLVMBuildOr',
    'LLVMBuildPointerCast',
    'LLVMBuildPtrToInt',
    'LLVMBuildRet',
    'LLVMBuildRetVoid',
    'LLVMBuildSDiv',
    'LLVMBuildSRem',
    'LLVMBuildStore',
    'LLVMBuildStructGEP',
    'LLVMBuildSub',
    'LLVMConstInt',
    'LLVMConstIntOfString',
    'LLVMConstNull',
    'LLVMConstRealOfString',
    'LLVMConstStringInContext',
    'LLVMContextCreate',
    'LLVMContextDispose',
    'LLVMCreateBuilderInContext',
    'LLVMCreateMessage',
    'LLVMDisposeBuilder',
    'LLVMDisposeMemoryBuffer',
    'LLVMDisposeMessage',
    'LLVMDisposeMessage',
    'LLVMDisposeModule',
    'LLVMDoubleTypeInContext',
    'LLVMDumpModule',
    'LLVMFloatTypeInContext',
    'LLVMFloatTypeInContext',
    'LLVMFunctionType',
    'LLVMGetNamedFunction',
    'LLVMGetNamedGlobal',
    'LLVMGetParam',
    'LLVMInt1TypeInContext',
    'LLVMInt32TypeInContext',
    'LLVMInt64TypeInContext',
    'LLVMInt8TypeInContext',
    'LLVMModuleCreateWithNameInContext',
    'LLVMPointerType',
    'LLVMPositionBuilderAtEnd',
    'LLVMSetGlobalConstant',
    'LLVMSetInitializer',
    'LLVMSetLinkage',
    'LLVMSetSourceFileName',
    'LLVMSetUnnamedAddress',
    'LLVMShutdown',
    'LLVMSizeOf',
    'LLVMStructCreateNamed',
    'LLVMStructSetBody',
    'LLVMTypeOf',
    'LLVMVoidTypeInContext',
    'LLVMGetFirstFunction',
    'LLVMGetNextFunction',

    # analysis
    'LLVMVerifyModule',

    # linker
    'LLVMLinkModules2',

    # target
    'LLVMInitializeX86AsmPrinter',
    'LLVMInitializeX86Target',
    'LLVMInitializeX86TargetInfo',
    'LLVMInitializeX86TargetMC',

    # target machine
    'LLVMCreateTargetMachine',
    'LLVMDisposeTargetMachine',
    'LLVMGetTargetFromTriple',
    'LLVMTargetMachineEmitToFile',
    'LLVMTargetMachineEmitToMemoryBuffer',

    # bit reader
    'LLVMParseBitcodeInContext2',

    # bit writer
    'LLVMWriteBitcodeToMemoryBuffer',

    # passes
    'LLVMAddDeadStoreEliminationPass',
    'LLVMAddPromoteMemoryToRegisterPass',
    'LLVMCreateFunctionPassManagerForModule',
    'LLVMCreatePassManager',
    'LLVMDisposePassManager',
    'LLVMFinalizeFunctionPassManager',
    'LLVMInitializeFunctionPassManager',
    'LLVMPassManagerBuilderCreate',
    'LLVMPassManagerBuilderDispose',
    'LLVMPassManagerBuilderPopulateFunctionPassManager',
    'LLVMPassManagerBuilderPopulateModulePassManager',
    'LLVMPassManagerBuilderSetOptLevel',
    'LLVMPassManagerBuilderUseInlinerWithThreshold',
    'LLVMRunFunctionPassManager',
    'LLVMRunPassManager',
])


def consume_line(content):
    for i, c in enumerate(content):
        if c == '\n':
            return (content[i+1:].strip(), content[:i].strip())

    return (content, '')


def consume(content, token, ignore_error=False):
    content = content.strip()
    if content.startswith(token):
        return content[len(token):].strip()

    if not ignore_error:
        raise NameError(f'expected "{token}", but got "{content[:len(token)]}"')
    else:
        return content


def consume_args(content):
    content = consume(content, '(')

    args = []

    while content[0] != ')':
        (content, argname) = consume_ident(content)
        content = consume(content, ':')
        (content, argtype) = consume_type(content)
        args.append((argname, argtype))
        content = consume(content, ',', ignore_error=True)

    content = consume(content, ')')
    return (content, args)


def consume_ident(content):
    content = content.strip()
    for i, c in enumerate(content):
        if not c.isalnum() and not c == '_':
            return (content[i:].strip(), content[:i].strip())

    raise NameError('unexpected eof')


def consume_type(content):
    content = content.strip()

    for i, c in enumerate(content):
        if not c.isalnum() and not c in ['*', ':', '_', ' ']:
            return (content[i:].strip(), content[:i].strip())

    raise NameError('unexpected eof')


def parse_prefixed_line(content, prefix):
    content = consume(content, prefix)
    if not content.startswith(prefix):
        return consume_line(content)

    return (content, '')


def parse_decl(content):
    docs = []

    while content.startswith('///'):
        (content, doc) = parse_prefixed_line(content, '///')
        docs.append(doc)

    while content.startswith('//'):
        (content, doc) = parse_prefixed_line(content, '//')
        # there are some wrong comments in llvm-sys, but they usually don't
        # contain an arrow
        if not '->' in doc:
            docs.append(doc)

    deprecated = False
    if content.startswith('#[deprecated'):
        (content, _) = consume_line(content)
        deprecated = True

    content = consume(content, 'pub fn')
    (content, name) = consume_ident(content)
    (content, args) = consume_args(content)

    if content[0] == '-':
        content = consume(content, '->')
        (content, ret) = consume_type(content)
    else:
        ret = ''

    content = consume(content, ';')

    decl = {}
    if not deprecated:
        decl = {'name': name, 'args': args, 'return': ret, 'doc': docs}

    return (content, decl)


def parse_block(content):
    start = content.find('extern "C" {')

    if content[start-2:start] == '\n\n':
        doc = ''
    else:
        docstart = content[:start].rfind('//')
        doc = content[docstart+3:start-1]

    content = content[start:]
    content = consume(content, 'extern "C" {')
    decls = []

    while content[0] != '}':
        (content, decl) = parse_decl(content)
        decls.append(decl)

    consume(content, '}')

    block = {'doc': doc, 'decls': decls}
    return (content, block)

# llvm-sys name -> (realty, kantanty, realty -> kantanty, kantanty -> realty)
typemap = {
    '': ('void', 'void', None, None),
    'LLVMIntPredicate': ('i32', 'i32', None, None), # enum
    'LLVMRealPredicate': ('i32', 'i32', None, None), # enum
    'LLVMValueKind': ('i32', 'i32', None, None), # enum
    'LLVMCallConv': ('i32', 'i32', None, None), # enum
    'LLVMDLLStorageClass': ('i32', 'i32', None, None), # enum
    'LLVMLinkage': ('i32', 'i32', None, None), # enum
    'LLVMVisibility': ('i32', 'i32', None, None), # enum
    'LLVMOpcode': ('i32', 'i32', None, None), # enum
    'LLVMUnnamedAddr': ('i32', 'i32', None, None), # enum
    'LLVMTypeKind': ('i32', 'i32', None, None), # enum
    'LLVMVerifierFailureAction': ('i32', 'i32', None, None), # enum
    'LLVMCodeGenOptLevel': ('i32', 'i32', None, None), # enum
    'LLVMRelocMode': ('i32', 'i32', None, None), # enum
    'LLVMCodeModel': ('i32', 'i32', None, None), # enum
    'LLVMCodeGenFileType': ('i32', 'i32', None, None), # enum

    'u8': ('char', 'i32', 'std.char_to_int', 'std.int_to_char'),
    '*mut ::libc::c_char': ('string', 'string', None, None),
    '*const ::libc::c_char': ('string', 'string', None, None),
    '*::libc::c_char': ('string', 'string', None, None),
    '*mut *mut ::libc::c_char': ('*string', '*string', None, None),
    '*const *const ::libc::c_char': ('*string', '*string', None, None),
    '**::libc::c_char': ('*string', '*string', None, None),

    '::libc::size_t': ('*void', 'i32', 'std.ptr_to_int', 'std.int_to_ptr'),   # too big
    '::libc::c_ulonglong': ('*void', 'i32', 'std.ptr_to_int', 'std.int_to_ptr'),   # too big
    '*mut ::libc::size_t': ('*i32', '*i32', None, None),
    '::libc::c_uint': ('i32', 'i32', None, None),
    '*mut ::libc::c_uint': ('*i32', '*i32', None, None),
    '::libc::c_int': ('i32', 'i32', None, None),
    '*mut ::libc::c_int': ('*i32', '*i32', None, None),
    'LLVMBool': ('i32', 'bool', 'std.int_to_bool', 'std.bool_to_int'),

    'LLVMMemoryBufferRef': ('*MemoryBuffer', '*MemoryBuffer', None, None),
    '*mut LLVMMemoryBufferRef': ('**MemoryBuffer', '**MemoryBuffer', None, None),

    'LLVMAttributeIndex': ('i32', 'i32', None, None),
    'LLVMContextRef': ('*Context', '*Context', None, None),
    'LLVMModuleRef': ('*Module', '*Module', None, None),
    '*mut LLVMModuleRef': ('**Module', '**Module', None, None),
    'LLVMTypeRef': ('*Type', '*Type', None, None),
    '*mut LLVMTypeRef': ('**Type', '**Type', None, None),
    'LLVMValueRef': ('*Value', '*Value', None, None),
    '*mut LLVMValueRef': ('**Value', '**Value', None, None),
    'LLVMBasicBlockRef': ('*BasicBlock', '*BasicBlock', None, None),
    'LLVMMetadataRef': ('*OpaqueMetadata', '*OpaqueMetadata', None, None),
    'LLVMNamedMDNodeRef': ('*OpaqueNamedMDNode', '*OpaqueNamedMDNode', None, None),
    'LLVMValueMetadataEntry': ('*OpaqueValueMetadataEntry', '*OpaqueValueMetadataEntry', None, None),
    'LLVMBuilderRef': ('*Builder', '*Builder', None, None),
    'LLVMDIBuilderRef': ('*OpaqueDIBuilder', '*OpaqueDIBuilder', None, None),
    'LLVMModuleProviderRef': ('*ModuleProvider', '*ModuleProvider', None, None),
    'LLVMPassManagerRef': ('*PassManager', '*PassManager', None, None),
    'LLVMPassRegistryRef': ('*PassRegistry', '*PassRegistry', None, None),
    'LLVMUseRef': ('*Use', '*Use', None, None),
    'LLVMDiagnosticInfoRef': ('*DiagnosticInfo', '*DiagnosticInfo', None, None),
    'LLVMComdatRef': ('*Comdat', '*Comdat', None, None),
    'LLVMModuleFlagEntry': ('*OpaqueModuleFlagEntry', '*OpaqueModuleFlagEntry', None, None),
    'LLVMJITEventListenerRef': ('*OpaqueJITEventListener', '*OpaqueJITEventListener', None, None),
    'LLVMAttributeRef': ('*OpaqueAttributeRef', '*OpaqueAttributeRef', None, None),
    'LLVMTargetMachineRef': ('*TargetMachine', '*TargetMachine', None, None),

    'LLVMTargetRef': ('*Target', '*Target', None, None),
    '*mut LLVMTargetRef': ('**Target', '**Target', None, None),

    'LLVMPassManagerBuilderRef': ('*PassManagerBuilder', '*PassManagerBuilder', None, None),
}
def map_typename(typename):
    typename = typename.strip()
    if typename in typemap:
        return typemap[typename]
    return (None, None, None, None)


def camel_to_snake(name):
    name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).lower()

keywords = set([
    'if', 'else'
])
def transform_name(name):
    name = name.strip()
    if name.startswith('LLVM'):
        name = name[4:]
    name = camel_to_snake(name)

    if name in keywords:
        name = name + '_value'

    return name


def transform_doc(doc):
    return '\n'.join(map(lambda d: f'// {d}', doc))


def generate_kantan(block):
    code = ''
    if block['doc'] != '':
        code = f'// {block["doc"]}\n'

    skipped = []

    for decl in filter(lambda d: 'name' in d, block['decls']):
        c_name = decl['name']

        if c_name not in whitelist:
            continue

        (realty, _, _, _) = map_typename(decl['return'])
        if realty is None:
            skipped.append(c_name)
            continue

        cont = False
        def map_arg(a):
            global cont
            (argname, argtype) = a
            (realty, _, _, _) = map_typename(argtype)
            if realty is None:
                skipped.append(c_name)
                cont = True
            return f'{transform_name(argname)}: {realty}'

        params = ', '.join(map(map_arg, decl['args']))
        if cont:
            continue

        code += f'extern def {c_name}({params}): {realty};\n\n'

    for decl in filter(lambda d: 'name' in d, block['decls']):
        c_name = decl['name']
        if c_name not in whitelist:
            continue

        (_, kantanty, _, _) = map_typename(decl['return'])
        if kantanty is None:
            continue
        kantan_name = transform_name(c_name)
        code += f'{transform_doc(decl["doc"])}\n'

        cont = False
        def map_param_kantan(a):
            global cont
            (argname, argtype) = a
            (_, kantanty, _, _) = map_typename(argtype)
            if kantanty is None:
                cont = True
            return f'{transform_name(argname)}: {kantanty}'

        params = ', '.join(map(map_param_kantan, decl['args']))
        if cont:
            continue

        code += f'def {kantan_name}({params}): {kantanty} ' + '{\n'

        def map_arg_kantan(a):
            (argname, argtype) = a
            (_, _, _, kantan_to_c) = map_typename(argtype)
            argname = transform_name(argname)
            if kantan_to_c is not None:
                return f'{kantan_to_c}({argname})'
            else:
                return argname

        args = ', '.join(map(map_arg_kantan, decl['args']))
        if decl['return'] == '':
            code += f'    {c_name}({args});'
        else:
            (_, _, c_to_kantan, _) = map_typename(decl['return'])
            if c_to_kantan is not None:
                code += f'    return {c_to_kantan}({c_name}({args}));'
            else:
                code += f'    return {c_name}({args});'

        code += '\n}\n'

    print(code)

    return skipped


def translate(content):
    blocks = []

    while content.find('extern "C"') > 0:
        (content, block) = parse_block(content)
        #pprint(block, width=120)
        blocks.append(block)
        #print()

    function_count = 0
    skipped = []

    for b in blocks:
        if 'decls' in b:
            skipped += generate_kantan(b)
            print()
            function_count += len(b['decls'])

    # print('Total:', function_count, 'functions')

    for name in skipped:
        print(name, 'was skipped, because a type could not be mapped')


if __name__ == '__main__':
    for link in links:
        content = requests.get(link)
        translate(content.text)
