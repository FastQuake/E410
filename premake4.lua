solution("E410")
	configurations("Debug","Release")
		
		project("E410")
			kind("COnsoleApp")
			language("C++")
			files({"**.hpp","**.cpp","liblua.a"})
			libdirs({os.findlib("gl","glew","lua5.2")})
			if os.is("windows") == true then
				links({"sfml-system","sfml-window","sfml-graphics","glew32","opengl32","lua52"})
			elseif os.is("linux") then
				buildoptions({"`pkg-config --cflags lua5.2`"})
				linkoptions({"../liblua.a","../libGLEW.a"})
				links({"sfml-system","sfml-window","sfml-graphics","GL"})
			end
			location("./build")

			configuration("Debug")
				defines({"DEBUG"})
				flags({"Symbols"})

			configuration("Release")
				defines({"NDEBUG"})
				flags({"Optimize"})
