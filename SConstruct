import os

env = Environment(
    CPPPATH=".",
    CCFLAGS=["-g", "-O0", "-m32"],
    LINKFLAGS=["-m32", '-zmuldefs'],
    LIBS=[]
)
system_obj = env.Object(Glob('system/*.c'))
data_obj = env.Object(Glob('data/*.c'))
main_obj = env.Object(Glob('*.c'))
output_obj = env.Object(Glob('output/*.c'))
expressions_obj = env.Object(Glob('expressions/*.c'))

objects = [
    data_obj, 
    main_obj, 
    system_obj, 
    output_obj,
    expressions_obj
]

env.Program('neshla', objects)

testEnv = env.Clone()
testEnv['LIBS'] += ['cunit']

all_objs = env.Object([f for f in Glob('*.c') if 'compiler.c' not in f.path])

testEnv.Program(
    'simple_test', 
[
    'test/test.c', 
    data_obj, 
    output_obj,
    expressions_obj,
    system_obj,
    all_objs
]
)
