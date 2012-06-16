env = Environment()

libs=['glfw', 'GLEW', 'assimp']

if env['PLATFORM'] == 'posix':
	env['LIBPATH']=['../glfw-2.7.5/lib/x11', '../glew-1.7.0/lib',		'../assimp/lib']
	env['CPPPATH']=['../glfw-2.7.5/include', '../glew-1.7.0/include',	'../assimp/include']
	libs += ["rt", "X11",  "GL", "pthread", "m"] 
	
conf = Configure(env)
	
for lib in libs:
	if not conf.CheckLib(lib):
		print lib + ' not found!';
		Exit(1)



env = conf.Finish()
env.MergeFlags(env.ParseFlags("-std=c++0x"))
env.Append(RPATH = './')
env.SharedLibrary('bin/engine', Glob('src/*.cc') + Glob('src/*.c'),LIBS=libs)


