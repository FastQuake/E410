solution("E410")
	configurations("Debug","Release")
		
		project("E410")
			kind("COnsoleApp")
			language("C++")
			files({"**.hpp","**.cpp"})
			libdirs({os.findlib("gl","glew","lua")})
			if os.is("windows") == true then
				links({"sfml-system","sfml-window","sfml-graphics","glew32","opengl32","lua"})
			else 
				links({"sfml-system","sfml-window","sfml-graphics","GLEW","GL","lua"})
			end
			location("./build")

			configuration("Debug")
				defines({"DEBUG"})
				flags({"Symbols"})

			configuration("Release")
				defines({"NDEBUG"})
				flags({"Optimize"})
