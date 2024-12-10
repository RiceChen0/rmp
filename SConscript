from building import *
import os

cwd = GetCurrentDir()

src = Split('''
src/rmp.c
adapter/rtthread/rmp_mutex.c
adapter/rtthread/rmp_sem.c
''')

CPPPATH = [cwd + '/include']

group = DefineGroup('rmp', src, depend = [''], CPPPATH = CPPPATH)
Return('group')
