import os

env = Environment(CPPPATH=".")
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
