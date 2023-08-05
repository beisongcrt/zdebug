import os
import rtconfig
from building import *

cwd = GetCurrentDir()
src	= Glob('*.c')

CPPPATH = [cwd]

group = DefineGroup('zdebug', src, depend = ['PKG_USING_ZDEBUG'], CPPPATH = CPPPATH)

Return('group')
