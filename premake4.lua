solution("E410")
	configurations("Debug","Release")
		
		project("E410")
			kind("COnsoleApp")
			language("C++")
			files({"**.hpp","**.cpp"})
			libdirs({os.findlib("gl","glew","lua5.2")})
			if os.is("windows") == true then
				defines({"WINDOWS"})
				links({"sfml-system","sfml-window","sfml-graphics","glew32","opengl32","lua52","enet","ws2_32","wsock32","winmm"})
			elseif os.is("linux") then
				defines({"LINUX"})
				buildoptions({"`pkg-config --cflags lua5.2`"})
				linkoptions({"`pkg-config --libs lua5.2`","-Wl,-rpath=./libs "})
				links({"sfml-system","sfml-window","sfml-graphics","GL","GLEW","enet"})
			end
			location("./build")

			configuration("Debug")
				defines({"DEBUG"})
				flags({"Symbols"})

			configuration("Release")
				defines({"NDEBUG"})
				flags({"Optimize"})
