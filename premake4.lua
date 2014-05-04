solution("E410")
	configurations("Debug","Release")
		
		project("E410")
			kind("COnsoleApp")
			language("C++")
			files({"**.hpp","**.cpp"})
			libdirs({os.findlib("gl","glew","lua5.2")})
			includedirs({"/usr/include/bullet/","/usr/local/include/bullet/"})
			if os.is("windows") == true then
				includedirs({"x86_64-w64-mingw32/include/","/usr/i686-w64-mingw32/include/"})
				links({"sfml-system","sfml-window","sfml-graphics","glew32","opengl32","lua52","enet","ws2_32","wsock32","winmm","BulletCollision","BulletDynamics","LinearMath"})
			elseif os.is("linux") then
				buildoptions({"`pkg-config --cflags lua5.2`"})
				linkoptions({"`pkg-config --libs lua5.2`","-Wl,-rpath=./libs "})
				links({"sfml-system","sfml-window","sfml-graphics","GL","GLEW","enet","BulletCollision","BulletDynamics","LinearMath"})
			end
			location("./build")

			configuration("Debug")
				defines({"DEBUG"})
				flags({"Symbols"})

			configuration("Release")
				defines({"NDEBUG"})
				flags({"Optimize"})
